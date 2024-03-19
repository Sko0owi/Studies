signature ORDERED =
sig 
    type T 
    val eq  : T * T -> bool
    val lt  : T * T -> bool
    val leq : T * T -> bool
end

structure ORDERED_INTS : ORDERED =
struct
  type T = int
  fun lt (a,b) = a < b
  fun eq (a,b) = a = b
  fun leq (a,b) = a <= b
end

signature HEAP =
sig 
    structure Elem : ORDERED

    type Heap

    val empty       : Heap
    val isEmpty     : Heap -> bool

    val insert      : Elem.T * Heap -> Heap
    val merge       : Heap * Heap -> Heap

    val findMin     : Heap -> Elem.T 
    val deleteMin   : Heap -> Heap
end


functor BinomialHeap (Element : ORDERED) =
struct
    structure Elem = Element

    datatype Tree = Node of int * Elem.T * Tree list
    type Heap = Tree list

    val empty = []
    fun isEmpty ts = null ts

    fun rank (Node (r, x, c)) = r
    fun root (Node (r, x, c)) = x
    fun link (t1 as Node (r, x1, c1), t2 as Node (_, x2, c2)) = 
        if Elem.leq (x1, x2) then Node (r+1, x1, t2 :: c1)
        else Node (r+1, x2, t1 :: c1)

    fun insTree (t, []) = [t]
        | insTree (t, ts as t' :: ts') = 
        if rank t < rank t' then t :: ts else insTree (link (t, t'), ts')

    fun insert (x, ts) = insTree (Node (0, x, []), ts)
    fun merge (ts1, []) = ts1
        | merge ([], ts2) = ts2
        | merge (ts1 as t1 :: ts1', ts2 as t2 :: ts2') =
            if rank t1 < rank t2 then t1 :: merge (ts1', ts2)
            else if rank t2 < rank t1 then t2 :: merge (ts1, ts2')
            else insTree (link (t1, t2), merge (ts1', ts2'))

    fun removeMinTree [] = raise Empty
        | removeMinTree [t] = (t, [])
        | removeMinTree (t :: ts) =
            let val (t', ts') = removeMinTree ts
            in if Elem.leq (root t, root t') then (t, ts) else (t', t :: ts') end
    

    fun findMin [] = raise Empty
        | findMin (ts as (x :: xs)) = foldl (fn (x,acc) => if Elem.leq (root x,acc) then root x else acc) (root x) ts


    fun deleteMin ts =
        let val (Node (_, x, ts1), ts2) = removeMinTree ts
        in merge (rev ts1, ts2) end
end

functor ExplicitMin (H : HEAP) : HEAP =
struct
    structure Elem = H.Elem
    datatype Heap = E | NE of Elem.T * H.Heap

    val empty = E
    fun isEmpty E = true
        | isEmpty _ = false

    fun insert (x, E) = NE(x, H.insert(x, H.empty))
        | insert (x, NE(curr_min, heap)) = if Elem.leq (x,curr_min) then NE(x, H.insert(x, heap)) else NE(curr_min, H.insert(x, heap)) 

    fun merge(h1, E) = h1
        | merge(E, h2) = h2
        | merge (NE(m1, h1), NE(m2, h2)) = if Elem.leq(m1,m2) then 
                                                NE(m1, H.merge(h1,h2)) 
                                            else 
                                                NE(m2, H.merge(h1,h2))

    fun findMin E = raise Empty
        | findMin (NE(x, h)) = x
    
    fun deleteMin E = raise Empty
        | deleteMin (NE(_, h)) = 
            let val newH = H.deleteMin h 
            in 
                if H.isEmpty newH then 
                    E 
                else
                    NE(H.findMin newH, newH) 
            end
end

structure MyBinomialHeap = BinomialHeap(ORDERED_INTS)

structure test = ExplicitMin(MyBinomialHeap)


val myHeap = MyBinomialHeap.empty;
val myHeapWithElement1 = MyBinomialHeap.insert(5, myHeap);
val myHeapWithElement2 = MyBinomialHeap.insert(3, myHeapWithElement1);
val myHeapWithElement3 = MyBinomialHeap.insert(10, myHeapWithElement2)
val minimum = MyBinomialHeap.findMin(myHeapWithElement3);