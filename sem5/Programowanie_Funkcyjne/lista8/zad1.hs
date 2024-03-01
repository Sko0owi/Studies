import System.IO( isEOF )
import Data.Char(toLower)

main :: IO ()
main = echoLower :: IO ()
echoLower = do
    end <- isEOF
    if end then 
        return ()
    else do
        x <- getChar
        putChar $ toLower x
        echoLower