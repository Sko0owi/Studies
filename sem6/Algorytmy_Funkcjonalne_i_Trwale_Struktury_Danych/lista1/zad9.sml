(* zad 9 *)

fun suffixes (x::xs) = (x::xs) :: (suffixes xs)
    | suffixes [] = [[]]