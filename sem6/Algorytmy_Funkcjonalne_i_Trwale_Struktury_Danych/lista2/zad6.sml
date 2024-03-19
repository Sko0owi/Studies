(* let rec merge cmp a b = match a,b with
| x :: a, [] -> x :: a
| [], x :: b -> x :: b
| [],[] -> []
| x :: a, y :: b -> if cmp x y 
                        then x :: merge cmp a (y :: b) 
                        else y :: merge cmp (x :: a) b  *)

fun merge cmp ([],[]) = []
  | merge cmp (a,[])  = a
  | merge cmp ([],b)  = b
  | merge cmp (a as (x :: xs), b as (y :: ys)) = 
    if cmp x y then x :: merge cmp (xs, b)
    else y :: merge cmp (a, ys)

fun revMerge rev cmp ([],[]) acc            = acc
    | revMerge rev cmp (x :: xs, []) acc    = revMerge rev cmp (xs,[]) (x :: acc)
    | revMerge rev cmp ([],b) acc           = revMerge rev cmp (b,[]) acc
    | revMerge rev cmp (a as (x :: xs), b as (y :: ys)) acc =
        if rev then
            if not (cmp x y) then revMerge rev cmp (xs, b) (x :: acc)
                             else revMerge rev cmp (a, ys) (y :: acc)
            else if cmp x y then revMerge rev cmp (xs, b) (x :: acc)
                            else revMerge rev cmp (a, ys) (y :: acc)
    

fun generateList [] acc = acc
  | generateList (x :: xs) acc = generateList xs ([x] :: acc)

fun createNewLayer order cmp ([], acc) = acc
  | createNewLayer order cmp ((x :: (y :: xs)), acc) =    createNewLayer order cmp (xs, ((revMerge order cmp (x, y) []) :: acc))
  | createNewLayer order cmp ((x :: xs), acc) =           createNewLayer order cmp (xs, ((revMerge order cmp (x,[]) []) :: acc))

fun le a b = if a < b then true else false

fun countLevels n acc = 
    if n = 0 then acc
    else countLevels (n div 2) acc + 1

fun mergesort (cmp,[]) = []
  | mergesort (cmp, (l as (x :: xs))) =
  let
    val levels = countLevels (length l) ((length l) mod 2)
    val heapsOneElem = generateList l []
    fun generateUntilOne order [] = []
      | generateUntilOne order (x :: nil) = x
      | generateUntilOne order (currentLayer as (x :: (y :: xs))) = generateUntilOne (not order) (createNewLayer order cmp (currentLayer, []))
  in
    generateUntilOne ((levels mod 2) = 0) heapsOneElem
    
  end