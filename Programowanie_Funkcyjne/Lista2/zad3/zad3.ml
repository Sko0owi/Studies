let rec sufixes list = match list with
| [] -> [[]]
| x :: list -> (x :: list) :: (sufixes list)

let rec prefixes list = match list with
| [] -> [[]]
| x :: list -> [] :: (fun y -> List.fold_right (fun a b -> ((x :: a) :: b)) y [] ) (prefixes list)