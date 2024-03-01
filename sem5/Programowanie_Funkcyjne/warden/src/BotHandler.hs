{-# LANGUAGE OverloadedStrings #-}

module BotHandler
  ( isCommand,
    containsBadWord,
    getResponseForUser,
    getUserWarningCounter,
  )
where

import BotState
  ( Response (..),
    UserData (..),
  )
import qualified Data.Text as T
import Text.Regex.TDFA
import Text.Regex.TDFA.Text ()

isCommand :: T.Text -> T.Text -> Bool
isCommand command = (== command) . head . T.splitOn (T.pack " ") . T.toLower

getUserWarningCounter :: [UserData] -> T.Text -> Int
getUserWarningCounter [] _ = 0
getUserWarningCounter ((UserData userId' warningCounter') : xs) lookUserId
  | lookUserId == userId' = warningCounter'
  | otherwise = getUserWarningCounter xs lookUserId

getResponse :: Int -> Response
getResponse warningCounter'
  | warningCounter' < 4 = Warning
  | warningCounter' < 7 = Kick
  | otherwise = Ban

getResponseForUser :: [UserData] -> T.Text -> Response
getResponseForUser userData user =
  let test = getUserWarningCounter userData user
   in getResponse test

traverseList :: [T.Text] -> T.Text -> Bool
traverseList [] _ = False
traverseList (x : xs) message =
  -- let regex = "\\b" <> x <> "\\b" in
  let regex = x
   in ((message =~ (regex :: T.Text)) || traverseList xs message)

containsBadWord :: [T.Text] -> T.Text -> Bool
containsBadWord wordlist message = do
  traverseList (map T.toLower wordlist) (T.toLower message)
