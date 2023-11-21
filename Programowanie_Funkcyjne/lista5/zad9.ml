type empty = |

type _ fin_type =
  | Unit : unit fin_type
  | Bool : bool fin_type
  | Empty : empty fin_type
  | Pair : 'a fin_type * 'b fin_type -> ('a * 'b) fin_type
  | Eith :'a fin_type * 'b fin_type -> (('a , 'b) Either.t) fin_type


let rec all_values: type a. a fin_type -> a Seq.t = function
  | Unit -> Seq.return ()
  | Bool -> Seq.cons true (Seq.return false)
  | Pair (a,b) -> Seq.product (all_values a) (all_values b)
  | Empty -> Seq.empty
  | Eith (c, d) -> Seq.append 
    (Seq.map (fun x-> Either.Left x) (all_values c)) 
    (Seq.map (fun x-> Either.Right x) (all_values d))

let test = List.of_seq (all_values (Pair(Bool, Pair(Bool, Bool))));;  