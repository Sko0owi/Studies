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

    datatype Tree = Node of Elem.T * Tree list
    type Heap = (int * Tree) list

    val empty = []
    fun isEmpty ts = null ts

    fun rank (r, _) = r
    fun tree (_, t) = t
    fun root (_, Node (x, c)) = x
    fun link (t1 as Node (x1, c1), t2 as Node (x2, c2)) = 
        if Elem.leq (x1, x2) then Node (x1, t2 :: c1)
        else Node (x2, t1 :: c1)

    fun insTree (t, []) = [t]
        | insTree (t, ts as t' :: ts') = 
        if rank t < rank t' then t :: ts else insTree ((rank t + 1, link (tree t, tree t')), ts')

    fun insert (x, ts) = insTree ((0, Node (x, [])), ts)

    fun merge (ts1, []) = ts1
        | merge ([], ts2) = ts2
        | merge (ts1 as t1 :: ts1', ts2 as t2 :: ts2') =
            if rank t1 < rank t2 then t1 :: merge (ts1', ts2)
            else if rank t2 < rank t1 then t2 :: merge (ts1, ts2')
            else insTree ((rank t1 + 1, link (tree t1, tree t2)), merge (ts1', ts2'))

    fun removeMinTree [] = raise Empty
        | removeMinTree [t] = (t, [])
        | removeMinTree (t :: ts) =
            let val (t', ts') = removeMinTree ts
            in if Elem.leq (root t, root t') then (t, ts) else (t', t :: ts') end
    

    fun findMin [] = raise Empty
        | findMin (ts as (x :: xs)) = foldl (fn (x,acc) => if Elem.leq (root x,acc) then root x else acc) (root x) ts
    

    fun deleteMin ts =
        let val ((r, Node (x, ts1)), ts2) = removeMinTree ts
        fun helper (r, (x :: xs)) = (r,x) :: (helper (r+1, xs)) (* możliwie da się lepiej *)
        in merge (helper (0, (rev ts1)), ts2) end
end



structure MyBinomialHeap = BinomialHeap(ORDERED_INTS)
val myHeap = MyBinomialHeap.empty;
val myHeapWithElement1 = MyBinomialHeap.insert(5, myHeap);
val myHeapWithElement2 = MyBinomialHeap.insert(3, myHeapWithElement1);
val myHeapWithElement3 = MyBinomialHeap.insert(10, myHeapWithElement2)
val minimum = MyBinomialHeap.findMin(myHeapWithElement3);