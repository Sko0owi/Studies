type 'a dllist = 'a dllist_data Lazy.t
(* Note: lazy_t is the built-in type constructor used by the compiler for the lazy keyword. You should not use it directly. Always use Lazy.t instead. *)
and 'a dllist_data ={ elem : 'a; next : 'a dllist; prev : 'a dllist;  }

let prev (prev : 'a dllist) = 
  match prev with
| lazy {prev; elem; next} -> prev
let next = function
  | lazy {prev; elem; next} -> next
let elem (prev : 'a dllist) = 
  match prev with
  | lazy {prev; elem; next} -> elem

let sing x =
  let rec s = lazy {next = s; elem = x; prev = s} in
  s

let last = ref (sing 4)

let rec gen (prev : 'a dllist) (xs : 'a list) (first : 'a dllist) =
  match xs with
  | [] -> 
          last := prev;
          first
  | (x :: xs) -> let rec current = 
                  lazy begin {prev = prev; elem = x; next = gen current xs first} end in 
                    let res = (Lazy.force current) in
                    lazy res

let rec of_list (xs : 'a list) =
  match xs with
  | [] -> failwith "empty"
  | x :: xs -> 
    let rec first = lazy begin let wyn = gen first xs first in
      {next = wyn; elem = x; prev = !last}
    end in first


let x = (of_list [1 ; 2 ; 3;]);;

assert ((prev (next x)) == (next (prev x)));;

