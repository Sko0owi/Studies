type ('a, 'b) format = (string -> 'b) -> (string -> 'a)

let int cont = 
  fun str x -> cont(str ^ string_of_int x)

let str cont = 
  fun str x -> cont(str ^ x)

let lit xs = 
  fun cont str -> cont(str ^ xs)

let (^^) =
  fun a b cont-> a(b cont)

let sprintf format =
format (fun x -> x) ""

let test = (sprintf (lit "Ala ma " ^^ int ^^ lit " kot" ^^ str ^^ lit ".")) 5 "ów"