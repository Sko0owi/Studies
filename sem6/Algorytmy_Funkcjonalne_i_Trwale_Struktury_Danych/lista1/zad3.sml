(* zad 3 NIE DZIAŁA :C *)

fun mergesort cmp (x::xs) = 
    let
        fun merge (nil, nil) = []
        | merge (left, nil) = left
        | merge (nil, right) = right
        | merge (l :: left, r :: right) = 
            if cmp l r 
                then l :: merge (left, r::right)
                else r :: merge (l::left, right)

        fun split (left, right) nil = (mergesort cmp left, mergesort cmp right)
        | split (left, right) (y :: ys) = split (y :: right, left) ys
    in 
        split (nil, nil) (x :: xs)
    end
    | mergesort cmp [] = ([],[])
