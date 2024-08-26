module type OrderedType = sig
  type t

  val leq : t -> t -> bool
  val eq : t -> t -> bool
end

module type T = sig
  type elt
  type color = R | B
  type tree = E | N of color * tree * elt * tree * bool
  type t = tree * int

  val insert : elt -> t -> t
  val delete : elt -> t -> t
  val mem : elt -> t -> bool
  val empty : t
  val is_empty : t -> bool
  val fromOrdList : elt list -> t
  val toList : t -> elt list
end

module Ordered_Ints : sig
  type t = int

  val leq : int -> int -> bool
  val eq : int -> int -> bool
end = struct
  type t = int

  let leq a b = a <= b
  let eq a b = a = b
end

module RB (Ord : OrderedType) : T with type elt := Ord.t = struct
  type elt
  type color = R | B
  type tree = E | N of color * tree * Ord.t * tree * bool
  type t = tree * int

  let is_empty (t,_)=
    match t with
    | E -> true
    | _ -> false

  let empty = E,0

  let rec mem x (t,d) =
    match t with
    | E -> false
    | N (_, l, e, r, v) when (Ord.eq e x) && v -> true
    | N (_, l, e, r, _) when Ord.leq x e -> mem x (l,d)
    | N (_, l, e, r, _) -> mem x (r,d)

  let balance t =
    match t with
    | N (B, N (R, N (R, a, x, b,v1), y, c,v2), z, d,v3)
    | N (B, N (R, a, x, N (R, b, y, c,v2),v1), z, d,v3)
    | N (B, a, x, N (R, N (R, b, y, c,v2), z, d,v3),v1)
    | N (B, a, x, N (R, b, y, N (R, c, z, d,v3),v2),v1) ->
        N (R, N (B, a, x, b,v1), y, N (B, c, z, d,v3),v2)
    | body -> body

  let insert x (t,d) =
    let rec ins t =
      match t with
      | E -> N (R, E, x, E, true)
      | N (c, l, e, r,_) when Ord.eq x e -> t
      | N (c, l, e, r,v) when Ord.leq x e -> balance (N (c, ins l, e, r,v))
      | N (c, l, e, r,v) -> balance (N (c, l, e, ins r,v))
    in
    match ins t with
    | E -> failwith "insert: unknown"
    | N (_, l, e, r,_) -> N (B, l, e, r,true),d



  let halve xs =
    let rec _h acc xs ys =
      match (xs, ys) with
      | xs, [] | xs, [ _ ] -> (List.rev acc, xs)
      | x :: xs, y :: z :: ys -> _h (x :: acc) xs ys
    in
    _h [] xs xs

  let ceil_log n = float_of_int n |> Float.log2 |> Float.ceil |> int_of_float

  let fromOrdList xs =
    let rec _fol h xs =
      match halve xs with
      | _, [] -> E
      | l, m :: r when h > 1 -> N (B, _fol (h - 1) l, m, _fol (h - 1) r,true)
      | _, m :: _ -> N (R, E, m, E,true)
    in
    let ll = List.length xs in
    (_fol (ll + 1 |> ceil_log) xs),(ll / 2)

  let rec foldl f acc t =
    match t with
    | E -> acc
    | N (_,l, e, r,v) when v -> foldl f (f (foldl f acc r) e) l
    | N (_,l, e, r,_)  -> foldl f (foldl f acc r) l


  let toList (t,_) = foldl (fun acc e -> e :: acc) [] t

  let delete x (t,d) =
      let rec del t =
      match t with
      | E -> N (R, E, x, E, true)
      | N (c, l, e, r,_) when Ord.eq x e -> N (c, l, e, r,false)
      | N (c, l, e, r,v) when Ord.leq x e -> N (c, del l, e, r,v)
      | N (c, l, e, r,v) -> N (c, l, e, del r,v)
      in
      if d == 0 then toList (del t,d) |> fromOrdList
      else (del t, d-1)
end



module RBI = RB (Ordered_Ints)

