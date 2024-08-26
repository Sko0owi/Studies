module type OrderedType = sig
  type t

  val leq : t -> t -> bool
  val eq : t -> t -> bool
end

module type T = sig
        type elt
        type rs =
        | Idle
        | Reversing of int * elt list * elt list * elt list * elt list
        | Appending of int * elt list * elt list
        | Done of elt list

        type t = int * elt list * rs * int * elt list

        val empty : t
        val isEmpty : t -> bool
        val snoc : elt -> t -> t
        val tail : t -> t
        val head : t -> elt
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

module HM (Ord : OrderedType) : T with type elt := Ord.t = struct
        type elt = Ord.t
        type rs =
        | Idle
        | Reversing of int * elt list * elt list * elt list * elt list
        | Appending of int * elt list * elt list
        | Done of elt list

        type t = int * elt list * rs * int * elt list

        let exec = function
        | Reversing(ok, x :: f, fs, y :: r, rs) -> Reversing(ok + 1, f, x :: fs, r, y :: rs)
        | Reversing(ok, [], fs, [y], rs) -> Appending(ok, fs, y :: rs)
        | Appending(0, fs, rs) -> Done rs
        | Appending(ok, x :: fs, rs) -> Appending(ok - 1, fs, x :: rs)
        | state -> state

        let invalidate = function
        | Reversing(ok, f, fs, r, rs) -> Reversing (ok - 1, f, fs, r, rs)
        | Appending(0, fs, x :: rs) -> Done rs
        | Appending(ok, x :: fs, rs) -> Appending (ok - 1, fs, rs) 
        | state -> state

        let exec2 (lenf, f, state, lenr, r) =
                match exec state |> exec with
                | Done newf -> (lenf, newf, Idle, lenr, r)
                | newstate -> (lenf, f, newstate, lenr, r)

        let check ((lenf, f, state, lenr, r) as q) =
                if lenr <= lenf then exec2 q
                else let newstate = Reversing (0,f,[],r,[]) in
                exec2 (lenf+lenr, f, newstate, 0, [])
        let empty = (0,[],Idle,0,[])
        let isEmpty (lenf,_,_,_,_) = (lenf = 0)
        let snoc x (lenf,f,state,lenr,r) = check (lenf,f,state,lenr+1,x::r)
        let head  = function
                | (lenf,[],state,lenr,r) -> failwith "empty"
                | (lenf,x :: f,state,lenr,r) -> x
        let tail = function
                | (lenf,[],state,lenr,r) -> failwith "empty"
                | (lenf,x :: f,state,lenr,r) -> check (lenf - 1, f, invalidate state, lenr, r)
end


module HMI = HM (Ordered_Ints)


