{-# LANGUAGE OverloadedStrings #-}

module BotStateHandler
  ( initState,
    loadState,
    saveState,
    addWordlistToState,
    addUserDataToState,
    removeWordFromState,
    removeUserDataFromState,
    getWordlistState,
    getUserDataState,
  )
where

import BotState
  ( BotState (..),
    UserData (..),
  )
import BotUtils
  ( getUserDataFromFile,
    getWordlistFromFile,
    saveUserDataToFile,
    saveWordlistToFile,
    updateUserData,
  )
import Data.IORef
import qualified Data.Text as T

-- Zarządzanie Stanem --

initState :: [T.Text] -> [UserData] -> IO BotState
initState startWordlist startUserData = do
  ref1 <- newIORef startWordlist
  ref2 <- newIORef startUserData
  return BotState {wordList = ref1, userData = ref2}

addWordlistToState :: BotState -> [T.Text] -> IO ()
addWordlistToState botState content = do
  modifyIORef (wordList botState) (++ content)

addUserDataToState :: BotState -> T.Text -> IO ()
addUserDataToState botState content = do
  modifyIORef (userData botState) (updateUserData content)

removeWordFromState :: BotState -> T.Text -> IO ()
removeWordFromState botState content = do
  modifyIORef (wordList botState) $ filter (/= content)

removeUserDataFromState :: BotState -> T.Text -> IO ()
removeUserDataFromState botState userId' = do
  modifyIORef (userData botState) $ filter (\x -> userId x /= userId')

getWordlistState :: BotState -> IO [T.Text]
getWordlistState botState = do
  readIORef (wordList botState)

getUserDataState :: BotState -> IO [UserData]
getUserDataState botState = do
  readIORef (userData botState)

-- Zapisywanie/Wczytywanie Stanu --

loadState :: IO BotState
loadState = do
  wordlist <- getWordlistFromFile "./src/wordlist.txt"
  userDataList <- getUserDataFromFile "./src/users.txt"
  putStrLn $ "Wczytałem dane z plików: \n" ++ show wordlist ++ "\n" ++ show userDataList
  initState wordlist userDataList

saveState :: BotState -> IO ()
saveState botState = do
  wordlist <- getWordlistState botState
  userDataList <- getUserDataState botState

  saveWordlistToFile wordlist "./src/wordlist.txt"
  saveUserDataToFile userDataList "./src/users.txt"

  putStrLn $ "Zapisuje dane do plików: \n" ++ show wordlist ++ "\n" ++ show userDataList
