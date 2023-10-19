let rec sublists list = match list with
| [] -> [[]]
| x::[] -> [[]; [x]]
| x::list -> (fun y -> List.fold_left (fun a b -> ((x::b)::a)) y y) (sublists list) 