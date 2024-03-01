module MyArr : sig
  type 'a array
  val empty : 'a array
  val sub : 'a array -> int -> 'a option
  val update : 'a array -> int -> 'a -> 'a array

end = struct
  type 'a array =
  | Empty 
  | Node of 'a option * 'a array * 'a array

  let empty = Empty

  let rec sub array idx = 
    match array with
    | Empty -> None
    | Node(etykieta, left, right) ->
      match idx with
      | 0 -> etykieta
      | n -> if n mod 2 = 1
          then sub left (idx / 2)
          else sub right (idx / 2)

  let rec update array idx v =
    match array with
    | Empty -> 
      begin
        match idx with
        | 0 -> Node( Some v, Empty, Empty )
        | n -> if n mod 2 = 1
            then Node (None, update Empty (idx / 2) v, Empty)
            else Node (None, Empty, update Empty (idx / 2) v)
      end
    | Node(_, left, right) -> 
      match idx with
      | 0 -> Node( Some v, left, right )
      | n -> if n mod 2 = 1
        then update left (idx / 2) v
        else update right (idx / 2) v
    
end

let a = MyArr.empty;;
let a_updated = MyArr.update a 0 "val";;
let a_sub = MyArr.sub a_updated 0;;
let a_updated2 = MyArr.update a 0 "val1";;
let a_sub2 = MyArr.sub a_updated2 0;;
let a_updated3 = MyArr.update a 1 "val2";;
let a_sub3 = MyArr.sub a_updated3 1;;