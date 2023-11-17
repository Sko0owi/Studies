type 'a lazy_tree = Leaf | Node of 'a t * 'a * 'a t
and 'a t = unit -> 'a lazy_tree

let rec qtree (a, b) (c, d) () =
  Node (qtree (a, b) (a + c, b + d), (a + c, b + d), qtree (a + c, b + d) (c, d))

let rec dfs depth qt =
  if depth > 0 then (
    match qt with
    | Leaf -> failwith("doszedleś do nieskończoności Gratulacje!")
    | Node (l, (x, y), r) ->
        Printf.printf "%d / %d\n" x y;
        dfs (depth - 1) (l ());
        dfs (depth - 1) (r ()))
  else ()

let test = dfs 4 (qtree (0,1) (1,0) ())