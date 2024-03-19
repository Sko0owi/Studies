signature ORDERED =
sig
  type T
  val eq : T * T -> bool
  val lt : T * T -> bool
  val leq : T * T -> bool
end

structure IntOrdered : ORDERED =
struct
  type T = int
  fun eq (x, y) = x = y
  fun lt (x, y) = x < y
  fun leq (x, y) = x <= y
end

functor RedBlackSet (Element : ORDERED) = 
struct
    type Elem = Element.T

    datatype Color = R | B
    datatype Tree = E | T of Color * Tree * Elem * Tree
    type Set = Tree

    val empty = E

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
end

structure MyRB = RedBlackSet(IntOrdered)

val myRB = MyRB.empty;
val myRBWithElement1 = MyRB.insert(5, myRB);
val myRBWithElement2 = MyRB.insert(3, myRBWithElement1);
val myRBWithElement3 = MyRB.insert(10, myRBWithElement2)
val myRBWithElement4 = MyRB.insert(5, myRBWithElement3)