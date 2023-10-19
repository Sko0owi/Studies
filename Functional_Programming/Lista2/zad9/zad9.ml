

type 'a clist = { clist : 'z. ('a -> 'z -> 'z) -> 'z -> 'z }


let cnil : 'a clist = {clist = fun f -> fun z -> z}


let ccons (a : 'a) (list : 'a clist) : 'a clist = {clist = fun f -> fun z -> f a (list.clist f z)}

let map x f = List.fold_right (fun x y -> (f x) :: y) x []
let map (func : 'a -> 'b) (clis : 'a clist) : 'b clist = clis.clist (fun x y -> ccons (func x) y) cnil


(* let append x y = List.fold_right (::) x y *)
let append (clis1 : 'a clist) (clis2 : 'a clist) : 'a clist = clis1.clist (ccons) clis2


let prod (clis1 : 'a clist) (clis2 : 'b clist) : ('a * 'b) clist = clis1.clist (fun x1 y1 -> append (clis2.clist (fun x2 y2 -> ccons (x1,x2) y2) cnil) y1) cnil


(* 
let exp (val1 : cnum) (val2 : cnum) : cnum = 
  {cnum = fun f x -> val1.cnum(val2.cnum) f x};; *)

(* let exp (clis1 : 'a clist) (clis2 : 'b clist) = clis1.clist (fun x1 y1 -> let newclist = (map (fun x -> x1) clis2) in (newclist.clist (fun x2 y2 -> ccons x2 y2) cnil) y1) cnil *)


let clist_to_list (clis : 'a clist) : 'a list = clis.clist (fun x y -> x :: y) []

let rec clist_of_list (list : 'a list) : 'a clist = match list with 
| [] -> cnil
| x :: xs -> (ccons x (clist_of_list xs))




let test_implementation = (ccons 4 (ccons 3 (ccons 2 (cnil))))
let test_convert = clist_to_list test_implementation

let test_toclist = clist_of_list test_convert
let test_check = clist_to_list test_toclist

let test_map = map (fun x -> x+1) test_toclist 
let test_check_map = clist_to_list test_map

let test_append = append test_toclist test_toclist 
let test_check_append = clist_to_list test_append

let test_prod = prod test_toclist test_toclist 
let test_check_prod = clist_to_list test_prod
(* 
let test_exp = exp test_toclist test_toclist 
let test_check_exp = clist_to_list test_exp *)