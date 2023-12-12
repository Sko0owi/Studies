
import System.IO( isEOF )
import Data.Char (toLower)

data StreamTrans i o a
    = Return a
    | ReadS (Maybe i -> StreamTrans i o a)
    | WriteS o (StreamTrans i o a)


toLowerMy :: StreamTrans Char Char a
toLowerMy = 
    ReadS
    (
        \x -> 
            case x of
                Nothing -> toLowerMy
                Just x -> WriteS (toLower x) (toLowerMy)
    )

runIOStreamTrans :: StreamTrans Char Char a -> IO a
runIOStreamTrans (Return a) = do
    return a
runIOStreamTrans (ReadS f) = do
    end <- isEOF
    if end
        then runIOStreamTrans(f Nothing)
        else do
            x <- getChar
            runIOStreamTrans $ f (Just x)

runIOStreamTrans (WriteS x c) = do
    putChar x
    runIOStreamTrans c        

runToLower :: IO a
runToLower = runIOStreamTrans toLowerMy


-- zad3

listTrans :: StreamTrans i o a -> [i] -> ([o], a)
listTrans (Return a) _ = ([], a)
listTrans (ReadS f) [] = listTrans (f Nothing) []
listTrans (ReadS f) (x : xs) = listTrans (f (Just x)) xs
listTrans (WriteS o cnt) is = (o : os, a) 
    where
        (os, a) = listTrans cnt is

abc = take 3 $ fst $ listTrans toLowerMy ['A' ..]

-- zad5
(|>|) :: StreamTrans i m a -> StreamTrans m o b -> StreamTrans i o b
(|>|) _ (Return b) = (Return b)
(|>|) st (WriteS o bt) = WriteS o (st |>| bt)
(|>|) (Return a) (ReadS f) = Return a |>| (f Nothing)
(|>|) (ReadS k) (ReadS f) = ReadS (\input -> (k input) |>| (ReadS f))
(|>|) (WriteS o k) (ReadS f) = k |>| (f (Just o))

-- zad6
catchOutput :: StreamTrans i o a -> StreamTrans i b (a, [o])
catchOutput stream = 
    _captureOutput stream []
    where _captureOutput st acc = case st of {
        Return a -> Return (a, acc);
        ReadS cont -> ReadS (\input -> _captureOutput (cont input) acc);
        WriteS o cps -> _captureOutput cps (o : acc)}

-- zad7
data BF
    = MoveR -- >
    | MoveL -- <
    | Inc -- +
    | Dec -- -
    | Output -- .
    | Input -- ,
    | While [BF] -- [ ]

brainfuckParser :: StreamTrans Char BF ()