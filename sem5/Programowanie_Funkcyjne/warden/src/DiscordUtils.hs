{-# LANGUAGE OverloadedStrings #-}

module DiscordUtils
  ( getToken,
    getGuildId,
    actionWithChannelId,
  )
where

import qualified Data.Text as T
import qualified Data.Text.IO as TIO
import Discord
import qualified Discord.Requests as R
import Discord.Types
import Text.Read (readMaybe)

getToken :: IO T.Text
getToken = TIO.readFile "./src/auth-token.secret"

getGuildId :: IO GuildId
getGuildId = do
  gids <- readFile "./src/guildid.secret"
  case readMaybe gids of
    Just g -> pure g
    Nothing -> error "could not read guild id from `guildid.secret`"

actionWithChannelId :: GuildId -> (ChannelId -> DiscordHandler a) -> DiscordHandler a
actionWithChannelId testserverid f = do
  Right chans <- restCall $ R.GetGuildChannels testserverid
  (f . channelId) (head (filter isTextChannel chans))
  where
    isTextChannel :: Channel -> Bool
    isTextChannel ChannelText {} = True
    isTextChannel _ = False
