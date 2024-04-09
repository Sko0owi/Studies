signature DEQUE =
sig 
    type 'a Queue 
    val empty : 'a Queue 
    val isEmpty : 'a Queue -> bool 

    val cons : 'a * 'a Queue -> 'a Queue 
    val head : 'a Queue -> 'a 
    val tail : 'a Queue -> 'a Queue 

    val snoc : 'a Queue * 'a -> 'a Queue 
    val last : 'a Queue -> 'a 
    val init : 'a Queue -> 'a Queue 
end 

structure MyDeque : DEQUE =
struct 
    type 'a Queue = 'a list * 'a list * int

    val empty = ([], [], 0) 
    fun isEmpty ([], [], _) = true 
        | isEmpty _ = false 


    fun checkf (f, r, 0) = (f, r, 0)
        | checkf (f, r, 1) = (f, r, 1)
        | checkf ([], r, cnt) = 
        let val listLength = List.length r val firstHalf = List.take (r, listLength div 2) in 
            (List.rev (List.drop (r, listLength - (List.length firstHalf))), firstHalf, cnt)
        end   
        | checkf (r, [], cnt) =
        let val listLength = List.length r val firstHalf = List.take (r, listLength div 2) in 
            (firstHalf, List.rev (List.drop (r, listLength - (List.length firstHalf))), cnt)
        end   
        | checkf q = q

    fun cons (x, (f, r, cnt)) = checkf(x :: f, r, cnt + 1)

    fun head (_,_,0) = raise Empty 
        | head ([], x :: [], 1) = x
        | head (x :: f, r, _) = x 

    fun tail (_,_,0) = raise Empty  
        | tail (x :: f, r, cnt) = checkf(f,r,cnt-1)

    fun snoc ((f, r, cnt), x) = checkf(f, x :: r, cnt + 1) 

    fun last (_,_,0) = raise Empty 
        | last (x :: [], [], 1) = x
        | last (f, x :: r, _) = x 

    fun init (_, _, 0) = raise Empty
        | init (f, x :: r, cnt) = checkf(f, r, cnt-1)

end 

val test1 = MyDeque.empty
val test2 = MyDeque.cons(1,test1)
val test3 = MyDeque.cons(5,test2)
val test4 = MyDeque.cons(6,test3)
val test5 = MyDeque.cons(8,test4)
val test6 = MyDeque.cons(10,test5)