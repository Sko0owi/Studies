type 'a tree = L of 'a | N of 'a tree * 'a tree
type 'a digit = Zero | One of 'a tree
type 'a RList = 'a digit list

let empty = []
let isEmpty l = match l with [] -> true | _ -> false

let link t1 t2 = N(t1,t2)
let consTree t = function
        | [] -> [One t]
        | Zero :: ts -> One t :: ts
        | One tx :: ts -> Zero :: consTree (link t tx) ts

let unconsTree = function
        | [] -> failwith "empty"
        | One t :: [] -> t,[]
        | One t :: ts -> t,Zero::ts
        | Zero  :: ts -> 
                (match unconsTree ts with
                | L _,_ -> failwith "impossible"
                | N (t1,t2),ts -> (t1,One t2 :: ts))

let cons x ts = consTree (Leaf x) ts
let head ts =
  match unconsTree ts |> snd with
  | L x -> x
  | _ -> failwith "impossible"
let tail ts = unconsTree ts |> snd

let rec lookupTree i t s =
        match i,t with
        | 0,L(x) -> x
        | _,L(_) -> raise "impossible"
        | i,N(t1, t2) -> if i < (s / 2) then lookupTree i t1 (s/2)
        else lookupTree (i - (s/2)) t2 (s/2)

let rec lookup idx s = function
        | [] -> failwith "empty"
        | Zero :: ts -> lookup idx ts (2*s)
        | One t :: ts -> if i < s then lookupTree i t s else lookup (i - s) (2*s) ts

let rec updateTree i t s y=
        match i,t with
        | 0,L(x) -> L(y)
        | _,L(_) -> raise "impossible"
        | i,N(t1, t2) -> if i < (s / 2) then N(updateTree i t1 (s/2) y,t2)
        else N(t1, updateTree (i - (s/2)) t2 (s/2) y)

let rec update idx s y = function
        | [] -> failwith "empty"
        | Zero :: ts -> Zero :: update idx ts (2*s) y
        | One t :: ts -> if i < s then One(updateTree i t s y) :: ts else One t :: update (i - s) (2*s) y ts






        

