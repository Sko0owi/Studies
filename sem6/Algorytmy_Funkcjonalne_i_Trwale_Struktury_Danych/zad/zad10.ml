let rec drop k ts =
        match k,ts with
        | _,[] -> []
        | 0,t -> t
        | k, Zero :: ts -> drop k ts
        | k, One t :: ts -> 
                if k < size t then dropTree k t ts else drop (k - size t) ts

let rec dropTree k t acc =
        if k = 0 then acc
        else
        match t with
        | L(_) -> failwith "impossible"
        | N(w,t1,t2) -> if k <= (w/2) then dropTree k t1 (t2 :: acc)
                        else dropTree (k - (w/2)) t2 acc

(* n = 2^k *)
let rec bt n a = if (n/2) = 0 then L(a) else 
                let t = bt (n/2) a in
                N(n,t,t)

let rec create n a =
        let _create n a s =
        if (n % 2) = 1 then One (bt s a) :: _create (n/2) a (2*s)
        else Zero :: _create (n/2) a (2*s)
  in _create n a 1
