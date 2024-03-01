module Main (main) where
import DiscordHandler (startServer)

main :: IO ()
main = warden

warden :: IO ()
warden = do 
  startServer

