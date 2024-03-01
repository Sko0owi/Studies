module IdM = struct
  type 'a t = 'a
  let return elem = elem
  let bind elem func = func elem
end

module PpM = struct
  type 'a t = unit -> 'a
  let return elem () = elem
  let bind elem func () = func (elem ()) ()
end