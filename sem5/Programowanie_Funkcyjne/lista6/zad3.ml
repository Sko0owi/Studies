exception Error

let for_all xs pred = 
  let f acc elem = 
    if pred elem then acc else raise Error in
  try List.fold_left f true xs with Error -> false

let mult_list xs = 
  let f acc elem = 
    if elem <> 0 then acc * elem else raise Error in 
  try List.fold_left f 1 xs with Error -> 0

let sorted xs =
  let f acc elem = 
    if acc < elem then elem else raise Error in 
  try let _ = List.fold_left f Int.min_int xs in
  true with Error -> false

let pred x = if x > 1 then true else false
let test_for_all = for_all [5;2;3;4] pred
let test_mult = mult_list [1;2;0;4]
let test_sorted = sorted [1;2;5;4]