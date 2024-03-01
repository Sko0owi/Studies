{-# LANGUAGE OverloadedStrings #-}
{-# OPTIONS_GHC -Wno-name-shadowing #-}
{-# OPTIONS_GHC -Wno-type-defaults #-}
{-# OPTIONS_GHC -Wno-overlapping-patterns #-}

module DiscordHandler
  ( startServer,
  )
where

import BotHandler
  ( containsBadWord,
    getResponseForUser,
    getUserWarningCounter,
    isCommand,
  )
import BotState
  ( BotState (..),
    OperationType (..),
    Response (..),
  )
import BotStateHandler
  ( addUserDataToState,
    addWordlistToState,
    getUserDataState,
    getWordlistState,
    loadState,
    removeUserDataFromState,
    removeWordFromState,
    saveState,
  )
import Control.Monad (void)
import Data.List (find)
import qualified Data.Text as T
import qualified Data.Text.IO as TIO
import Discord
import qualified Discord.Requests as R
import Discord.Types
import DiscordUtils
  ( actionWithChannelId,
    getGuildId,
    getToken,
  )
import Text.Read (readMaybe)
import Text.Regex.TDFA.Text ()
import UnliftIO (liftIO)
import UnliftIO.Concurrent

startServer :: IO ()
startServer = do
  botState <- loadState
  removeWordFromState botState ""

  tok <- getToken
  testserverid <- getGuildId

  err <-
    runDiscord $
      def
        { discordToken = tok,
          discordOnStart = startHandler testserverid,
          discordOnEnd = endHandler botState,
          discordOnEvent = eventHandler botState testserverid,
          discordOnLog = \s -> TIO.putStrLn s >> TIO.putStrLn "",
          discordGatewayIntent = def {gatewayIntentMembers = True, gatewayIntentPresences = True}
        }

  TIO.putStrLn err

handleHelp :: Message -> DiscordHandler ()
handleHelp message = do
  let helpMessage =
        "Dostępne komendy dla bota to:\n"
          ++ "!add <word> -> dodaje nowe słowo do listy słów zakazanych\n"
          ++ "!remove <word> -> usuwa słowo z listy\n"
          ++ "!unban <userID> -> resetuje użytkownikowi o podanym userID jego przewinienia\n"
          ++ "!check <userID> -> sprawdza ile ostrzeżeń ma użytkownik o podanym userID\n"
          ++ "!help wyświetla tą komende :>\n"
  void $ restCall (R.CreateMessage (messageChannelId message) (T.pack helpMessage))

-- printRoles :: GuildId -> UserId -> DiscordHandler ()
-- printRoles guildId userId = do
--   guildMember <- restCall (R.GetGuildMember guildId userId)
--   case guildMember of
--     Left _ -> liftIO $ putStrLn $ "Error"
--     Right m -> do
--       let roles = head (memberRoles m)
--       guildRoles <- restCall (R.GetGuildRoles guildId)
--       case guildRoles of
--         Left _ -> liftIO $ putStrLn $ "Error"
--         Right a -> do
--           liftIO $ putStrLn $ show $ a
--           liftIO $ putStrLn $ show $ find (\x -> roleId x == roles) a

--       liftIO $ putStrLn $ show roles

traverseRoles :: [Role] -> [RoleId] -> Bool
traverseRoles _ [] = False
traverseRoles guildRoles (x : xs) = do
  case find (\z -> roleId z == x) guildRoles of
    Just _ -> True
    Nothing -> traverseRoles guildRoles xs

isAdmin :: GuildId -> UserId -> DiscordHandler Bool
isAdmin guildId userId = do
  mguildMember <- restCall (R.GetGuildMember guildId userId)
  case mguildMember of
    Left _ -> return False
    Right guildMember -> do
      let userRoles = memberRoles guildMember
      mguildRoles <- restCall (R.GetGuildRoles guildId)
      case mguildRoles of
        Left _ -> return False
        Right guildRoles -> do
          return $ traverseRoles guildRoles userRoles

handleOperation :: OperationType -> BotState -> GuildId -> Message -> DiscordHandler ()
handleOperation operationType botState guildId message = do
  let wholeCommand = T.splitOn (T.pack " ") (messageContent message)

  userAdmin <- isAdmin guildId (userId (messageAuthor message))

  if not userAdmin
    then void $ restCall (R.CreateMessage (messageChannelId message) "Komenda tylko dla administratorów!")
    else do
      if length wholeCommand /= 2
        then void $ restCall (R.CreateMessage (messageChannelId message) "Komenda przyjmuje tylko dwa argumenty!")
        else do
          userData <- liftIO $ getUserDataState botState

          let wordForOperation = wholeCommand !! 1
          _ <- case operationType of
            Add -> liftIO $ addWordlistToState botState [wordForOperation]
            Remove -> liftIO $ removeWordFromState botState wordForOperation
            Unban -> do
              liftIO $ removeUserDataFromState botState wordForOperation
              case readMaybe (T.unpack wordForOperation) of
                Just n -> void $ restCall (R.RemoveGuildBan guildId (DiscordId (Snowflake (fromIntegral n))))
                Nothing -> liftIO $ putStrLn "Zły userId został podany"
            Check -> liftIO $ putStrLn $ "Sprawdzam ile ostrzeżeń miał użytkownik"
            _ -> liftIO $ putStrLn $ "Podany zły typ operacji!"

          let operationMessage = case operationType of
                Add -> "Dodano nowe słowo: "
                Remove -> "Usunięto słowo: "
                Unban -> "Odbanowano użytkownika "
                Check -> "Status ostrzeżeń użytkownika "
                _ -> "Podany zły typ operacji!"

          let additionalInfo = case operationType of
                Check -> " " <> T.pack (show $ getUserWarningCounter userData wordForOperation)
                _ -> ""
          void $ restCall (R.CreateMessage (messageChannelId message) (operationMessage <> wordForOperation <> additionalInfo))

handleWarning :: Message -> DiscordHandler ()
handleWarning m = do
  void $ restCall (R.CreateMessage (messageChannelId m) ("Ostrzeżenie dla " <> userName (messageAuthor m) <> " " <> T.pack (show (userId (messageAuthor m)))))

handleKick :: GuildId -> Message -> DiscordHandler ()
handleKick guildId m = do
  void $ restCall (R.RemoveGuildMember guildId (userId (messageAuthor m)))
  void $ restCall (R.CreateMessage (messageChannelId m) ("Wyrzucam użytkownika " <> userName (messageAuthor m) <> " " <> T.pack (show (userId (messageAuthor m)))))

handleBan :: GuildId -> Message -> DiscordHandler ()
handleBan guildId m = do
  void $ restCall (R.CreateGuildBan guildId (userId (messageAuthor m)) (R.CreateGuildBanOpts {R.createGuildBanOptsDeleteLastNMessages = Nothing, R.createGuildBanOptsReason = Just (T.pack "Zostałeś zbanowany przez wardena uwu")}))
  void $ restCall (R.CreateMessage (messageChannelId m) ("Banuje użytkownika " <> userName (messageAuthor m) <> " " <> T.pack (show (userId (messageAuthor m)))))

handleBadWord :: BotState -> GuildId -> Message -> DiscordHandler ()
handleBadWord botState guildId m = do
  void $ restCall (R.CreateReaction (messageChannelId m, messageId m) "x")

  threadDelay (2 * 10 ^ (6 :: Int))

  userDataList <- liftIO $ getUserDataState botState
  let userId' = T.pack $ show $ userId $ messageAuthor m
  let botResponse = getResponseForUser userDataList userId'

  liftIO $ addUserDataToState botState userId'

  case botResponse of
    Warning -> handleWarning m
    Kick -> handleKick guildId m
    Ban -> handleBan guildId m

  void $ restCall (R.DeleteMessage (messageChannelId m, messageId m))

eventHandler :: BotState -> GuildId -> Event -> DiscordHandler ()
eventHandler botState guildId event = do
  wordList <- liftIO $ getWordlistState botState
  case event of
    MessageCreate m
      | userIsBot (messageAuthor m) -> return ()
      | isCommand "!add" (messageContent m) -> handleOperation Add botState guildId m
      | isCommand "!remove" (messageContent m) -> handleOperation Remove botState guildId m
      | isCommand "!unban" (messageContent m) -> handleOperation Unban botState guildId m
      | isCommand "!check" (messageContent m) -> handleOperation Check botState guildId m
      | isCommand "!help" (messageContent m) -> handleHelp m
      | containsBadWord wordList (messageContent m) -> handleBadWord botState guildId m
      | otherwise -> return ()
    _ -> return ()

endHandler :: BotState -> IO ()
endHandler botState = do
  liftIO $ threadDelay (round (0.4 * 10 ^ 6)) >> putStrLn "Bot zakończył działanie"
  saveState botState

startHandler :: GuildId -> DiscordHandler ()
startHandler guildId = do
  liftIO $ putStrLn "Bot Warden został uruchomiony"

  let activity = (mkActivity "!help" ActivityTypeGame) {activityState = Just "Wardening time :D"}
  let opts =
        UpdateStatusOpts
          { updateStatusOptsSince = Nothing,
            updateStatusOptsActivities = [activity],
            updateStatusOptsNewStatus = UpdateStatusOnline,
            updateStatusOptsAFK = False
          }
  sendCommand (UpdateStatus opts)

  actionWithChannelId guildId $ \cid ->
    void $
      restCall $
        R.CreateMessage
          cid
          "Wardening time hehe"