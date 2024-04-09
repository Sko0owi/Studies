signature ORDERED =
sig 
    type T 
    val eq  : T * T -> bool
    val lt  : T * T -> bool
    val leq : T * T -> bool
    val printElem : T -> string
end

structure ORDERED_INTS : ORDERED =
struct
  type T = int
  fun lt (a,b) = a < b
  fun eq (a,b) = a = b
  fun leq (a,b) = a <= b
  fun printElem a = Int.toString a
end

signature HEAP =
sig 
    structure Elem : ORDERED

    type Heap

    val empty       : Heap
    val isEmpty     : Heap -> bool

    val insert      : Elem.T * Heap -> Heap
    val smaller     : Elem.T * Heap -> Heap
    val bigger     : Elem.T * Heap -> Heap

    val partition     : Elem.T * Heap -> Heap*Heap

    val printTree : Heap -> unit
    val sort : Elem.T list -> Elem.T list
    (* val merge       : Heap * Heap -> Heap

    val findMin     : Heap -> Elem.T 
    val deleteMin   : Heap -> Heap *)
end


functor SplayHeap (Element : ORDERED) = 
struct
    structure Elem = Element

    datatype Tree = E | T of Tree * Elem.T * Tree
    type Heap = Tree

    val empty = E
    fun isEmpty E = true 
        | isEmpty _ = false

    fun bigger(pivot,E) = E
        | bigger(pivot, T(a,x,b)) = 
            if Elem.leq(x,pivot) then bigger(pivot,b)
            else case a of 
                E => T(E,x,b)
                | T(a1, y, a2) =>
                    if (Elem.leq(y,pivot)) then T(bigger(pivot,a2), x, b)
                    else T(bigger(pivot,a1), y, T(a2,x,b))

    fun smaller (pivot, E) = E 
        | smaller (pivot, T(a,x,b)) = 
            if Elem.leq(x, pivot) then 
                case b of 
                    E => T(a,x,E)
                    | T(a1, y, a2) =>
                        if Elem.leq(y, pivot) then T(T(a, x, a1), y, smaller(pivot, a2))
                        else T(a, x, smaller(pivot, a2)) 
            else smaller (pivot, a)

    fun partition (pivot, E) = (E,E)
        | partition (pivot, t as T(a,x,b)) = 
            if Elem.leq(x,pivot) then 
                case b of 
                    E => (t,E)
                    | T(b1,y,b2) => 
                        if Elem.leq(y,pivot) then 
                            let val (small, big) = partition(pivot, b2)
                            in (T(T(a,x,b1),y,small),big) end
                        else 
                            let val (small, big) = partition(pivot, b1)
                            in (T(a,x,small), T(big,y,b2)) end 
            else  
                case a of 
                    E => (E,t)
                    | T(a1, y, a2) => 
                        if Elem.leq(y,pivot) then 
                            let val (small, big) = partition(pivot, a2)
                            in (T(a1,y,small), T(big,x,b)) end 
                        else 
                            let val (small, big) = partition(pivot, a1)
                            in (small, T(big,y,T(a2,x,b))) end

    fun insert (x,t) = T(smaller(x,t),x,bigger(x,t))

    fun heapTraverse t =
        let fun aux (E, acc)= acc
                | aux (T(a,x,b), acc) = 
                    aux (a, (x :: aux(b,acc)))
        in aux (t, []) end

    fun sort xs = 
        let val splayHeap = List.foldl insert empty xs
        in heapTraverse splayHeap end 

    fun printTree t =
    let
        fun spaces 0 = ""
            | spaces n = " " ^ spaces (n-1)
        fun aux lvl E = print (spaces lvl ^ "E\n")
            | aux lvl (T(l,x,r)) =
            (
                aux (lvl+1) r;
                spaces lvl;
                print (spaces lvl ^ (Elem.printElem x) ^ "\n");
                aux (lvl+1) l
            )
    in
        aux 0 t
    end
end


structure MySplayTree = SplayHeap(ORDERED_INTS)

val test1 = MySplayTree.empty

val add1 = MySplayTree.insert(1,test1)
val add2 = MySplayTree.insert(2,add1)
val add3 = MySplayTree.insert(3,add2)
val add4 = MySplayTree.insert(4,add3)
val add5 = MySplayTree.insert(5,add4)
val add6 = MySplayTree.insert(6,add5)
val add7 = MySplayTree.insert(7,add6)
val add8 = MySplayTree.insert(3,add7)

val small = MySplayTree.smaller(3,add8)
val big = MySplayTree.bigger(3,add8)


datatype Tree2 = E2 | T2 of Tree2 * int * Tree2

fun printTree t =
    let
        fun spaces 0 = ""
            | spaces n = " " ^ spaces (n-1)
        fun aux lvl E2 = print (spaces (lvl+3) ^ "E\n")
            | aux lvl (T2(l,x,r)) =
            (
                aux (lvl+1) r;
                (* spaces lvl; *)
                print (spaces (lvl+3) ^ (Int.toString x) ^ "\n");
                aux (lvl+1) l
            )
    in
        aux 0 t
    end

val a = T2(E2,500,T2(T2(E2,750,T2(T2(E2,770,E2),800,E2)),999,E2));;
