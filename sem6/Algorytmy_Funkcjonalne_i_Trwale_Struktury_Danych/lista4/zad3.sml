open Lazy;

signature STREAM =
sig
    datatype 'a streamCell = Cons of 'a * 'a stream | Nil
    withtype 'a stream = 'a streamCell susp
    exception SHd and STl and Nth
    (* val sHd : 'a stream -> 'a
    val sTl : 'a stream -> 'a stream *)
    (* val ++ : 'a stream * 'a stream -> 'a stream *)
    val take : int * 'a stream -> 'a stream
    (* val constStream : 'a -> 'a stream
    val mkStream : (int -> 'a) -> 'a stream
    val from : int -> int stream
    val sMap : ('a -> 'b) -> 'a stream -> 'b stream
    val sDrop : int -> 'a stream -> 'a stream
    val sTake : int -> 'a stream -> 'a list
    val zip : 'a stream * 'b stream -> ('a*'b) stream
    val unzip : ('a*'b) stream -> 'a stream * 'b stream
    val splice : 'a stream * 'b stream -> ('a*'b) stream
    val filter : ('a -> 'b option) -> 'a stream -> 'b stream
    val find : ('a -> bool) -> 'a stream -> 'a stream
    val remove : ('a -> bool) -> 'a stream -> 'a stream
    val findOne : ('a -> bool) -> 'a stream -> 'a option
    val flatten : 'a stream stream -> 'a stream
    val nth : int -> 'a stream -> 'a
    val prefix : ('a -> bool) -> 'a stream -> 'a stream
    val suffix : ('a -> bool) -> 'a stream -> 'a stream
    val split : int -> 'a stream -> 'a stream * 'a stream
    val splitp : ('a -> bool) -> 'a stream -> 'a stream * 'a stream *)
end;

structure Stream : STREAM =
struct
    datatype 'a streamCell = Cons of 'a * 'a stream | Nil
    withtype 'a stream = 'a streamCell susp
    exception SHd and STl and Nth

    fun lazy take   (0, _) = $Nil
            | take  (n, $Nil) = $Nil
            | take  (n, $Cons(x,s)) = $Cons(x, take(n-1,s))
end