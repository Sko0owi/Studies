signature ORDERED =
sig
  type T
  val eq : T * T -> bool
  val lt : T * T -> bool
  val leq : T * T -> bool
end


signature SET =
    sig
        structure Elem : ORDERED
        type Set
        val empty : Set
        val singleton : Elem.T -> Set
        val toList : Set -> Elem.T list
        val fromList : Elem.T list -> Set
        val fromOrdList : Elem.T list -> Set
        val size : Set -> int
        val member : Elem.T * Set -> bool
        val find : Elem.T * Set -> Elem.T option
        val add : Elem.T * Set -> Set
        val delete : Elem.T * Set -> Set
        val union : Set * Set -> Set
        val intersection : Set * Set -> Set
        val difference : Set * Set -> Set
    end


functor UnbalancedTreeSet (Element : ORDERED) : SET =
struct
    structure Elem = Element
    datatype Tree = E | T of Tree * Elem.T * Tree
    type Set = Tree

    val empty = E 
    fun singleton x = T(E,x,E)

    fun size E = 0
        | size (t as T(h1, x, h2)) = size h1 + size h2 + 1

    fun member (x, E) = false
        | member (x,T(h1, y, h2)) = if Elem.leq (x,y) then 
                                            if Elem.leq (y,x) then 
                                                true 
                                            else member (x, h1) 
                                        else member(x,h2)
    
    fun find (x, h) = if member (x, h) then SOME x else NONE

    fun add (x, E) = singleton x
        | add (x, h as T(h1, y, h2)) =  
            let 
                fun add_helper (x, h as T(h1, y, h2)) =
                    if Elem.leq (x,y) then
                        T(add_helper(x,h1),y,h2)
                    else 
                        T(h1,y,add_helper(x,h2))
            in 
            if member (x, h) then 
                h 
            else
                add_helper(x, h)
            end
    fun delete (x, E) = raise Empty
        | delete(x, h as T(h1, y, h2)) = 
            let 
                fun goRight (h as T(h1, x, E)) = (x, h1) 
                    | goRight (h as T(h1, y, h2)) = 
                    let 
                        val (maksLeft, newTree) = goRight h2 
                    in 
                        (maksLeft, T(h1, y, newTree))
                    end
                fun del_helper(x, h as T(h1, y, h2)) =
                    if Elem.leq (x, y) then
                        if Elem.leq (y, x) then 
                            let val (newElem, newH1) = goRight h1 in T(newH1, newElem, h2) end
                        else
                            T(del_helper(x, h1),y,h2)
                    else 
                        T(h1,y,del_helper(x,h2))
            in 
                if member (x, h) then 
                    del_helper(x, h) 
                else
                    h
            end

    fun toList E = []
        | toList (t as T(t1, x, t2)) = toList t1 @ (x :: toList t2)
    
    fun fromList [] = E
        | fromList (x::xs) = add (x, (fromList xs))
   
    fun fromOrdList xs = fromList xs

    fun union (h1,h2) = fromList (toList h1 @ toList h2)
    fun intersection (x,y) = foldl (fn (x, acc) => if member(x, y) then add(x, acc)  else acc) empty (toList x) 
    fun difference (h1,h2) = foldl (fn (x, acc) => delete (x, acc)) h1 (toList h2) 
            
end


signature NONEMPTY =
    sig
        type t
        val someElem : t
    end

signature MAP =
sig
    structure Key : ORDERED
    type Value
    type Map
    val empty : Map
    val bind : Key.T * Value * Map -> Map
    val lookup : Key.T * Map -> Value option (* NONE gdy klucz nie występuje w słowniku *)
end

functor Map
    (functor Set (Elem : ORDERED) : SET
            structure Key : ORDERED
            structure Value : NONEMPTY) : MAP =
struct 
    structure Key = Key
    type Value = Value.t


    structure Ordered_Pair : ORDERED =
    struct
        type T = Key.T * Value
        fun lt  (a as (k,v),b as (k', v')) = k < k'
        fun eq  (a as (k,v),b as (k', v')) = k = k'
        fun leq (a as (k,v),b as (k', v')) = k <= k'
    end
    

    structure Set_key = Set(Ordered_Pair)
    datatype test = E | T of Set_key.Set

    type Map = test

    val empty = E
    fun bind (key, value, E) = Set_key.add((key,value))
        | bind (key, value, t) = Set_key.add((key,value)) 
    fun lookup (key, map) = NONE
         
end
    