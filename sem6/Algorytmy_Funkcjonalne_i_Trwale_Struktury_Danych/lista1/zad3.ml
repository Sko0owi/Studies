let rec merge cmp a b = match a,b with
| x :: a, [] -> x :: a
| [], x :: b -> x :: b
| [],[] -> []
| x :: a, y :: b -> if cmp x y 
                        then x :: merge cmp a (y :: b) 
                        else y :: merge cmp (x :: a) b 

let  merge_ogon func a b = 
  let rec _merge a b acc = match a,b with
  | [],[] -> acc
  | x :: a, [] -> _merge  a [] (x :: acc)
  | [], x :: b -> _merge  [] b (x :: acc)
  | x :: a, y :: b -> if (func x y) 
                        then _merge a (y :: b) (x :: acc) 
                        else _merge (x :: a) b (y :: acc)
in List.rev(_merge a b [])

let rec halve list = 
  match list with
  | [] -> [],[]
  | x :: [] -> [x],[]
  | x :: y :: list -> (fun (left, right) -> (x :: left, y :: right)) (halve list)

let rec merge_sort cmp a = 
  match (halve a) with
  | [],[] -> []
  | [x],[] -> [x]
  | x,y -> merge cmp (merge_sort cmp x) (merge_sort cmp y)