type 'a tree = Leaf of 'a | Node of int * 'a tree * 'a tree
type 'a digit = One of 'a tree | Two of 'a tree * 'a tree
type 'a rlist = 'a digit list

let empty = []
let isEmpty = function [] -> true | _ -> false

let size = function
        | Leaf(_) -> 1
        | Node(s,_,_) -> s

let link t1 t2 = Node(size t1 + size t2,t1,t2)

let rec consTree t = function
        | [] -> [One(t)]
        | One(tx) :: ts -> Two(t,tx) :: ts
        | Two(tx,ty) :: ts -> One(t) :: consTree (link tx ty) ts

let rec unconsTree = function
        | [] -> failwith "empty: unconsTree"
        | [One(t)] -> t,[]
        | [Two(tx,ty)] -> tx,[One(ty)]
        | One(t) :: ts -> (match unconsTree ts with
                           | Node(_,t1,t2),tsp -> t,Two(t1,t2) :: tsp
                           | _,_ -> failwith "impossible: unconsTree")
        | Two(tx,ty) :: ts -> tx,One(ty) :: ts

let cons x ts = consTree (Leaf(x)) ts
let tail ts = let _,tsp = unconsTree ts in tsp
let head = function One(Leaf(x)) :: _ -> x | Two(Leaf(x),_) :: _ -> x | _ -> failwith "impossible: head"

let rec lookupTree idx = function
        | Leaf(x) -> x
        | Node(w,t1,t2) when idx < (w / 2) -> lookupTree idx t1
        | Node(w,t1,t2) -> lookupTree (idx - (w/2)) t2

let rec updateTree el idx = function
        | Leaf(x) -> Leaf(el)
        | Node(w,t1,t2) when idx < (w / 2) -> Node(w,updateTree el idx t1,t2)
        | Node(w,t1,t2) -> Node(w,t1,updateTree el (idx - (w/2)) t2)

let rec lookup idx = function
        | [] -> failwith "not found: lookup"
        | One(t) :: ts -> if idx < size t then lookupTree idx t else lookup (idx - size t) ts
        | Two(tx,ty) :: ts -> if idx < size tx then lookupTree idx tx
                                               else if idx < (2 * size ty)
                                               then lookupTree (idx - size ty) ty
                                               else lookup (idx - (2 * size ty)) ts
let rec update el idx = function
        | [] -> failwith "not found: update"
        | One(t) :: ts -> if idx < size t then One(updateTree el idx t) :: ts else One(t) :: update el (idx - size t) ts
        | Two(tx,ty) :: ts -> if idx < size tx then Two(updateTree el idx tx,ty) :: ts
                                               else if idx < (2 * size ty)
                                               then Two(tx,updateTree el (idx - size ty) ty) :: ts
                                               else Two(tx,ty) :: update el (idx - (2 * size ty)) ts



