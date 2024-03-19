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
(* 
functor RedBlackSet (Elem : ORDERED) : SET = 
struct
    type Elem = Element.T

    datatype Color = R | B
    datatype Tree = E | T of Color * Tree * Elem * Tree
    type Set = Tree

    val empty = E
    val singleton = 

    fun member (x, E) = false
        | member (x, T(_,a,y,b)) = 
            if Element.lt (x, y) then member (x, a)
            else if Element.lt (y, x) then member (x, b)
            else true

    fun balance ((B,T (R, T(R,a,x,b),y,c),z,d)
        | (B,T (R,a,x,T(R,b,y,c)),z,d)
        | (B,a,x,T (R,T(R,b,y,c),z,d))
        | (B,a,x,T (R,b,y,T(R,c,z,d)))) = T(R, T(B,a,x,b),y,T(B,c,z,d))
        | balance body = T body

    fun insert (x, s) =
        let fun ins E = T(R, E, x, E)
            | ins (s as T (color, a, y, b)) = 
                if Element.lt (x, y) then balance (color, ins a, y, b)
                else if Element.lt (y, x) then balance (color, a, y, ins b)
                else s
            val T(_,a,y,b) = ins s 
        in T(B,a,y,b) end
    
     fun fromOrdList xs =
        let
            fun buildTreeFromSorted (tree, [], _) = tree
              | buildTreeFromSorted (tree, xs, curr_len) =
                  let
                    val halfLength = curr_len div 2
                    val (leftHalf, y::rightHalf) = List.splitAt (xs, halfLength)
                    val newTree = insert (y, tree)
                  in
                    buildTreeFromSorted (buildTreeFromSorted (newTree, leftHalf, halfLength), rightHalf, curr_len - halfLength - 1)
                  end
        in
            buildTreeFromSorted (empty, xs, length xs)
        end
end *)