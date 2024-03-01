let rec fold_left_cps (f : ('a -> 'b -> ('a -> 'c) -> 'c)) (acc : 'a) (xs : 'b list) (cont : 'a -> 'c) =
  match xs with
  | [] -> cont acc
  | x :: xs -> f acc x (fun new_acc -> fold_left_cps f new_acc xs cont) 

let fold_left f acc xs =
  fold_left_cps (fun acc x cont -> cont (f acc x)) acc xs (fun x -> x)