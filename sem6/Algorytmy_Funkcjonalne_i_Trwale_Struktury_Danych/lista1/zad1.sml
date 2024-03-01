(* zad 1 *)

fun sublist (x::xs) =
    let
        fun addx (ys::yss) = (x::ys) :: addx yss
        | addx nil = nil
        val xss = sublist xs
    in
        xss @ addx xss
    end
    | sublist [] = [[]]


fun sublistBetter (x::xs) =
    let
        fun addx (ys::yss) = (x::ys) :: ys :: addx yss
        | addx nil = nil
        val xss = sublistBetter xs
    in
        addx xss
    end
    | sublistBetter [] = [[]]


datatype tree = & of int * (tree * tree) | %
infix &


fun flatten (x & (t1,t2)) = flatten t1 @ [x] @ flatten t2
    | flatten % = nil

fun flattenBetter (x & (t1,t2)) = 
    let
        fun flattenAcc (x & (t1,t2)) acc = flattenAcc t1 (x :: flattenAcc t2 acc)
        | flattenAcc % acc = acc
    in
        flattenAcc (x & (t1,t2)) nil
    end
    | flattenBetter % = nil


val exampleTree = (10&(2&(1&(%,%),3&(25&(%,%),%)),4&(%,%)))


fun rev (x::xs) = rev xs @ [x]
    | rev nil = nil

fun revBetter (x::xs) = 
    let
        fun revx (y::ys) acc = revx ys (y::acc)
        | revx nil acc = acc
    in
        revx (x::xs) []
    end
    | revBetter [] = []
