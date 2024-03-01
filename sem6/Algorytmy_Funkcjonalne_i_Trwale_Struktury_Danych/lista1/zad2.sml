(* zad 2 *)

fun quicksort cmp (x::xs) = 
    let
        fun split (small,big) (y::ys) = 
        if cmp x y 
            then split (y :: small, big) ys
            else split (small, y :: big) ys
        | split (small, big) nil = quicksort cmp small @ (x :: quicksort cmp big) (* troche meh append *)
    in 
        split (nil,nil) xs
    end
    | quicksort cmp [] = []
