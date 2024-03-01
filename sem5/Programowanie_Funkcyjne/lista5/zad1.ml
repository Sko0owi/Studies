let rec fix_with_limit limit f x =
  if limit > 0 then f (fix_with_limit (limit - 1) f) x
  else failwith "limit reached! ending"

let fix_memo f x =
  let tbl = Hashtbl.create 100 in
  let rec _fix f x =
    match Hashtbl.find_opt tbl x with
    | Some x -> x
    | None ->
        let res = f (_fix f) x in
        Hashtbl.add tbl x res;
        res
  in
  _fix f x


let fib_f fib n =
  if n <= 1 then n
  else fib (n-1) + fib (n-2)


let fibb_limit = fix_with_limit 101 fib_f

(* let test1 = fibb_limit 50 *)

let fibb_memo = fix_memo fib_f

(* let test2 = fibb_memo 50 *)