let rec fold_left_cps (f : ('a -> 'b -> ('a -> 'c) -> 'c)) (acc : 'a) (xs : 'b list) (cont : 'a -> 'c) =
  match xs with
  | [] -> cont acc
  | x :: xs -> f acc x (fun new_acc -> fold_left_cps f new_acc xs cont) 


let for_all xs pred =
  let f acc elem cont =
    if pred elem then cont acc else false in 
  fold_left_cps f true xs (fun x -> x)

let mult_list xs =
  let f acc elem cont =
    if elem <> 0 then cont (acc * elem) else 0 in 
  fold_left_cps f 1 xs (fun x -> x)

let sorted xs = 
  let f (res, prev) elem cont =
    begin match prev with
    | None -> cont (res, Some elem)
    | Some prev_elem -> if elem <= prev_elem then (false, None) else cont (res, (Some elem)) end in
    fold_left_cps f (true, None) xs (fun x -> x) |> fst

let pred x = if x > 1 then true else false
let test_for_all = for_all [5;2;3;4] pred
let test_mult = mult_list [1;2;0;4]
let test_sorted = sorted [1;2;5;4]