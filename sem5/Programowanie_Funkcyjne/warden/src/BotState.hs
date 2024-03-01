{-# LANGUAGE OverloadedStrings #-} 

module BotState (
                    BotState(..),
                    UserData(..),
                    Response(..),
                    OperationType(..)
                ) where

import qualified Data.Text as T
import Data.IORef

data BotState = BotState { wordList :: IORef [T.Text], userData :: IORef [UserData]}

data UserData = UserData { userId :: T.Text, warningCounter :: Int} deriving (Eq)
instance Show UserData where
  show (UserData userId' warningCounter') = "UserData: " <> T.unpack userId' <> " " <> show warningCounter'

data OperationType = Add | Remove | Unban | Check

data Response = Warning | Kick | Ban deriving (Show,Eq)

