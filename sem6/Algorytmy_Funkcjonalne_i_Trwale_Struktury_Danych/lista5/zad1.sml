signature QUEUE =
sig 
    type 'a Queue
    
    val empty : 'a Queue 
    val isEmpty : 'a Queue -> bool
    
    val snoc : 'a Queue * 'a -> 'a Queue
    val head : 'a Queue -> 'a
    val tail : 'a Queue -> 'a Queue
end

structure BatchedQueue : QUEUE =
struct
    type 'a Queue = 'a list * 'a list

    val empty = ([],[])
    fun isEmpty (f,r) = null f
    
    fun checkf ([], r) = (List.rev r, [])
        | checkf q = q


    fun snoc ((f,r), x) = checkf(f, x :: r)

    fun head ([],_) = raise Empty
        | head (x :: f, r) = x

    fun tail ([],_) = raise Empty
        | tail (x :: f, r) = checkf(f, r) 
end

structure MisiaszekQueue : QUEUE =
struct

    datatype 'a Queue = E | Q of 'a * 'a list * 'a list

    val empty = E
    fun isEmpty E = true
        | isEmpty _ = false
    
    fun snoc (E, x) = Q(x,[],[])
        | snoc (Q(h,f,r), x) = Q(h,f, x :: r)

    fun head E = raise Empty 
        | head (Q(x, f, r)) = x


    fun tail_help (Q(_, [], [])) = E
        | tail_help (Q(_, [], r)) = 
        let val revR = rev r in 
            Q(hd revR, tl revR, []) 
        end
        | tail_help Q(_, x :: f, r) = Q(x, f, r)

    fun tail E = raise Empty 
        | tail q = tail_help q
end