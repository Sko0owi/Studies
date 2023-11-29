type ('a, 'b) format =
| Int : (int -> 'a, 'a) format
| Str : (string -> 'a, 'a) format
| Lit : string -> ('a, 'a) format
| Cat : ('a, 'b) format * ('b, 'c) format -> ('a, 'c) format 


let int cont = 
  fun str x -> cont(str ^ string_of_int x)

let str cont = 
  fun str x -> cont(str ^ x)

let lit xs = 
  fun cont str -> cont(str ^ xs)

let (^^) =
  fun a b -> Cat(a, b)


let rec ksprintf : type a b. (a,b) format -> (string -> b) -> string -> a = 
function
  | Int -> fun cont str x -> cont(str ^ string_of_int x)
  | Str -> fun cont str x -> cont(str ^ x)
  | Lit tekst -> fun cont str -> cont(str ^ tekst)
  | Cat (fmt1, fmt2) -> fun cont str -> ksprintf fmt1 (ksprintf fmt2 cont) str

let sprintf format =
  ksprintf format (fun x -> x) ""


let test = (sprintf (Lit "Ala ma " ^^ Int ^^ Str)) 2 " psy"



let rec kprintf : type a b. (a, b) format-> (unit -> b) -> (unit -> a) =
  function 
  | Int -> fun  cont () x -> cont (print_int x)
  | Str -> fun  cont () x -> cont (print_string x)
  | Lit l -> fun  cont () -> cont (print_string l)
  | Cat (a, b) -> fun cont ()-> kprintf a (kprintf b cont ) ()


let printf z =
  kprintf z (fun () -> ()) () 


let test = (printf (Lit "Ala ma " ^^ Int ^^ Str)) 2 " psy"

