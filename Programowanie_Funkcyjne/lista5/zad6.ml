type 'a my_lazy_node =
  | Lazy  of (unit -> 'a)
  | Done of 'a
  | Working
type 'a my_lazy = ('a my_lazy_node) ref

let force x =
  match !x with
  | Done res -> res
  | Working -> failwith "you cant force force padawan"
  | Lazy f -> 
    x := Working;
    let res = f () in
    x := Done res; res

