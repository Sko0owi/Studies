type 'a tree = Leaf of 'a | Node  of int  * 'a tree * 'a tree
type 'a rlist = 'a tree list

let empty : 'a rlist = []
let isEmpty = function [] -> true | _ -> false

let size = function
        | Leaf(_) -> 1
        | Node(s,_,_) -> s

let link t1 t2 = Node(size t1 + size t2,t1,t2)

let rec consTree t = function
        | [] -> [t]
        | tx :: txs when size t < size tx -> t :: (tx :: txs)
        | tx :: txs -> consTree (link t tx) txs

let splitToLeaf t =
        let rec _split acc = function
                | Leaf(x) -> Leaf(x),acc
                | Node(_,t1,t2) -> _split (t2 :: acc) t1
        in
        _split [] t

let unconsTree = function
        | [] -> failwith "empty: unconsTree"
        | tx :: txs -> let lf,acc = splitToLeaf tx in
                        lf,List.append acc txs

let cons x ts = consTree (Leaf(x)) ts
let head ts = match unconsTree ts with Leaf(x),_ -> x | _,_ -> failwith "impossible"
let tail ts = let _,tss = unconsTree ts in tss

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
        | tx :: txs when idx < size tx -> lookupTree idx tx
        | tx :: txs -> lookup (idx - size tx) txs

let rec update el idx = function
        | [] -> failwith "not found: lookup"
        | tx :: txs when idx < size tx -> (updateTree el idx tx) :: txs
        | tx :: txs -> tx :: (update el (idx - size tx) txs)

                        






