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

        type t = int * elt list * rs  * elt list

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

        type t = int * elt list * rs * elt list

        let exec nd = function
        | Reversing(ok, x :: f, fs, y :: r, rs) ->  nd+1, Reversing(ok + 1, f, x :: fs, r, y :: rs)
        | Reversing(ok, [], fs, [y], rs) -> nd + 1, Appending(ok, fs, y :: rs)
        | Appending(0, fs, rs) -> nd, Done rs
        | Appending(ok, x :: fs, rs) -> nd+1,Appending(ok - 1, fs, x :: rs)
        | state -> nd,state

        let invalidate = function
        | Reversing(ok, f, fs, r, rs) -> Reversing (ok - 1, f, fs, r, rs)
        | Appending(0, fs, x :: rs) -> Done rs
        | Appending(ok, x :: fs, rs) -> Appending (ok - 1, fs, rs) 
        | state -> state

        let exec2 (diff, f, state,  r) =
                let (nd,st) = exec diff state in
                match exec nd st with
                | (nd, Idle) -> (nd, f, state, r)
                | (nd, Done newf) -> (nd, newf, Idle,  r)
                | (nd, newstate) -> (nd, f, newstate,  r)

        let check ((diff, f, state,  r) as q) =
                if diff >= 0 then exec2 q
                else let newstate = Reversing (0,f,[],r,[]) in
                exec2 (-1,f, newstate, [])
        let empty = (0,[],Idle,[])
        let isEmpty (_,f,_,_) = match f with [] -> true | _ -> false
        let snoc x (diff,f,state,r) = check (diff - 1,f,state,x::r)
        let head  = function
                | (diff,[],state,r) -> failwith "empty"
                | (diff,x :: f,state,r) -> x
        let tail = function
                | (diff,[],state,r) -> failwith "empty"
                | (diff,x :: f,state,r) -> check (diff - 1, f, invalidate state, r)
end


module HMI = HM (Ordered_Ints)
