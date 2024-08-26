type digit = One | Two
type nat = digit list

let rec inc = function
        | [] -> [One]
        | One :: ds -> Two :: ds
        | Two :: ds -> One :: inc ds

let rec dec = function
        | [] -> failwith "zero"
        | Two :: ds -> One :: ds
        | One :: ds -> Two :: dec ds

let rec add dxs dys =
   match dxs, dys with
   | [],[] -> []
   | _ :: dx  ,[] -> dxs
   | [], _ :: dy  -> dys
   | One :: dx, One :: dy -> Two :: add dx dy
   | One :: dx, Two :: dy -> One :: add (inc dx) dy
   | Two :: dx, One :: dy -> One :: add (inc dx) dy
   | Two :: dx, Two :: dy -> Two :: add (inc dx) dy

