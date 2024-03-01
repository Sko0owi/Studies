{-# LANGUAGE OverloadedStrings #-}
{-# OPTIONS_GHC -Wno-name-shadowing #-}
{-# OPTIONS_GHC -Wno-unrecognised-pragmas #-}
{-# HLINT ignore "Redundant ==" #-}
{-# OPTIONS_GHC -Wno-unused-imports #-}

module TestBot
  ( testBot,
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
    UserData (..),
  )
import BotStateHandler
  ( addUserDataToState,
    addWordlistToState,
    getUserDataState,
    getWordlistState,
    initState,
    loadState,
    removeUserDataFromState,
    removeWordFromState,
    saveState,
  )
import BotUtils
  ( getUserDataFromFile,
    getWordlistFromFile,
    saveUserDataToFile,
    saveWordlistToFile,
    updateUserData,
  )
import Control.Monad (unless)
import qualified Data.Text as T

testBot :: IO ()
testBot = do
  unless (isCommand (T.pack "!add") (T.pack "!add word") == True) (error "isCommand error")
  unless (isCommand (T.pack "!add") (T.pack "!ada word") == False) (error "isCommand error")

  unless (containsBadWord ["bob", "kot"] "ala ma kota" == True) (error "containsBadWord error")
  unless (containsBadWord ["bob", "kot"] "ala ma psa" == False) (error "containsBadWord error")

  unless (getResponseForUser [UserData {userId = "testID", warningCounter = 0}] "testID" == Warning) (error "getResponseForUser error")
  unless (getResponseForUser [UserData {userId = "testID", warningCounter = 4}] "testID" == Kick) (error "getResponseForUser error")
  unless (getResponseForUser [UserData {userId = "testID", warningCounter = 10}] "testID" == Ban) (error "getResponseForUser error")

  unless (getUserWarningCounter [UserData {userId = "testID", warningCounter = 3}] "testID" == 3) (error "getUserWarningCounter error")
  unless (getUserWarningCounter [UserData {userId = "testID", warningCounter = 3}] "newTestID" == 0) (error "getUserWarningCounter error")

  botState <- initState ["word1", "word2"] [UserData {userId = "testID", warningCounter = 3}]

  addWordlistToState botState ["word3"]
  test <- getWordlistState botState
  unless (test == ["word1", "word2", "word3"]) (error "addWordlistToState error")

  removeWordFromState botState "word3"
  test <- getWordlistState botState
  unless (test == ["word1", "word2"]) (error "removeWordFromState error")

  addUserDataToState botState "69420"
  test <- getUserDataState botState
  unless (test == [UserData {userId = "testID", warningCounter = 3}, UserData {userId = "69420", warningCounter = 1}]) (error "addUserDataToState error")

  removeUserDataFromState botState "69420"
  test <- getUserDataState botState
  unless (test == [UserData {userId = "testID", warningCounter = 3}]) (error "removeUserDataFromState error")

  putStrLn "Bot Tests Passed! :>"