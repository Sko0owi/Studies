let rec foldl func acc xs =
  match xs with 
  | [] -> acc
  | (x :: xs) -> foldl func (func x acc) xs

let rec foldr func acc xs =
  match xs with 
  | [] -> acc
  | (x :: xs) -> func x (foldr func acc xs)

let rec foldr_ogon func acc xs =
  match xs with 
  | [] -> acc
  | (x :: xs) -> foldl func acc (List.rev (x :: xs))