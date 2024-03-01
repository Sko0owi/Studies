module BotUtils
  ( getWordlistFromFile,
    getUserDataFromFile,
    saveWordlistToFile,
    saveUserDataToFile,
    updateUserData,
  )
where

import BotState
  ( UserData (..),
  )
import qualified Data.Text as T
import qualified Data.Text.IO as TIO
import Text.Read (readMaybe)

updateUserData :: T.Text -> [UserData] -> [UserData]
updateUserData lookUserId [] = [UserData {userId = lookUserId, warningCounter = 1}]
updateUserData lookUserId ((UserData userId' warningCounter') : xs)
  | lookUserId == userId' = UserData {userId = lookUserId, warningCounter = warningCounter' + 1} : xs
  | otherwise = UserData {userId = userId', warningCounter = warningCounter'} : updateUserData lookUserId xs

getWordlistFromFile :: [Char] -> IO [T.Text]
getWordlistFromFile fileName = do
  fileContent <- TIO.readFile fileName
  return (T.splitOn (T.pack "\n") fileContent)

stringToUserData :: String -> [UserData]
stringToUserData line = do
  [userId', warningCounter'] <- words <$> lines line
  Just warningCounter'' <- return (readMaybe warningCounter')
  return (UserData {userId = T.pack userId', warningCounter = warningCounter''})

userDataToString :: UserData -> T.Text
userDataToString (UserData userId' warningCounter') = T.pack $ T.unpack userId' <> " " <> show warningCounter'

getUserDataFromFile :: [Char] -> IO [UserData]
getUserDataFromFile fileName = do
  stringToUserData <$> readFile fileName

saveWordlistToFile :: [T.Text] -> [Char] -> IO ()
saveWordlistToFile wordlist fileName = do
  let textToWrite = T.intercalate (T.pack "\n") wordlist
  TIO.writeFile fileName textToWrite

saveUserDataToFile :: [UserData] -> [Char] -> IO ()
saveUserDataToFile userData fileName = do
  let textToWrite = T.intercalate (T.pack "\n") (map userDataToString userData)
  TIO.writeFile fileName textToWrite
