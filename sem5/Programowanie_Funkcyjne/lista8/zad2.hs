
import Data.Char (toLower)
import System.IO
import Control.Monad


data StreamTrans i o a
    = Return a
    | ReadS (Maybe i -> StreamTrans i o a)
    | WriteS o (StreamTrans i o a)


instance Functor (StreamTrans i o) where
  fmap f m = m >>= return . f

instance Applicative (StreamTrans i o) where
  pure = Return
  (<*>) = ap

instance Monad (StreamTrans i o) where
    return = pure
    (Return a) >>= f = f a
    (ReadS cps) >>= f = ReadS (\ inp -> cps inp >>= f)
    (WriteS o cps) >>= f = WriteS o (cps >>= f)


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

main :: IO()
main = runToLower


-- zad3

listTrans :: StreamTrans i o a -> [i] -> ([o], a)
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

instance Show BF where  
    show MoveR = "MoveR"
    show MoveL = "MoveL"
    show Inc = "Inc"
    show Dec = "Dec"
    show Output = "Output"
    show Input = "Input"
    show (While bfList) = "While: " ++ show bfList


brainfuckParser :: StreamTrans Char BF ()
brainfuckParser =
    _brainfuckParser [] []
    where 
        _brainfuckParser acc while_stack =
            ReadS 
                (\input -> case input of
                    Nothing -> Return()
                    Just x -> case x of
                        '[' -> _brainfuckParser [] (acc : while_stack)
                        ']' -> case while_stack of
                            [[]] -> WriteS (While (reverse acc)) brainfuckParser
                            x : xs -> _brainfuckParser ((While (reverse acc)) : x) xs
                        '>' -> case while_stack of 
                            [] -> WriteS MoveR brainfuckParser
                            _ -> _brainfuckParser (MoveR : acc) while_stack 
                        '<' -> case while_stack of 
                            [] -> WriteS MoveL brainfuckParser
                            _ -> _brainfuckParser (MoveL : acc) while_stack 
                        '+' -> case while_stack of 
                            [] -> WriteS Inc brainfuckParser
                            _ -> _brainfuckParser (Inc : acc) while_stack 
                        '-' -> case while_stack of 
                            [] -> WriteS Dec brainfuckParser
                            _ -> _brainfuckParser (Dec : acc) while_stack 
                        '.' -> case while_stack of 
                            [] -> WriteS Output brainfuckParser
                            _ -> _brainfuckParser (Output : acc) while_stack 
                        ',' -> case while_stack of 
                            [] -> WriteS Input brainfuckParser
                            _ -> _brainfuckParser (Input : acc) while_stack 
                        _ -> _brainfuckParser acc while_stack
                    
                    )

test_parser = take 8 $ fst $ listTrans brainfuckParser ['<', '[', '.', '+', '[', '+', ']', ']', 'a', '.', '.']


coerceEnum :: (Enum a, Enum b) => a -> b
coerceEnum = toEnum . fromEnum

type Tape = ([Int], Int, [Int])
evalBF :: Tape -> BF -> StreamTrans Char Char Tape
evalBF (l, p, h : r) MoveR = Return (p : l, h, r) -- w prawo
evalBF (h : l, p, r) MoveL = Return (l, h, p : r) -- w lewo
evalBF (l,p,r) Inc = Return (l, p+1, r) -- dodaj do p
evalBF (l,p,r) Dec = Return (l, p-1, r) -- odejmij od p
evalBF (l,p,r) Output = WriteS (coerceEnum p) (Return (l,p,r)) -- wypisywanie
evalBF (l,p,r) Input =  -- wpisanie liczby
    ReadS 
        ( \input -> case input of
            Just x -> Return (l, (coerceEnum x), r)
            Nothing -> error "Error"
        )
evalBF (l,p,r) (While bfList) = -- czarna magia
    if p == 0
        then Return (l, p, r)
        else do
            t <- evalBFBlock (l, p, r) bfList
            evalBF t (While bfList)


evalBFBlock :: Tape -> [BF] -> StreamTrans Char Char Tape
evalBFBlock = foldM evalBF -- jeśli rozwiązałeś zadanie 9

runBF :: [BF] -> StreamTrans Char Char ()
runBF bfList = do
    x <- evalBFBlock (repeat 0, 0, repeat 0) bfList
    Return ()