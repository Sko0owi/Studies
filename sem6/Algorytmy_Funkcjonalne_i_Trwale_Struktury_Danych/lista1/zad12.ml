type 'a halfPersistent_ref = 
  | CurrentVersion of ('a option) array
  | PastVersion of (int * 'a option) * 'a halfPersistent
and 'a halfPersistent = 'a halfPersistent_ref ref

let empty len = CurrentVersion(Array.make len None)

let rec sub (hp_arr, id) =
  match !hp_arr with
  | CurrentVersion (arr) -> Array.get arr id
  | PastVersion ((_id, _val), nextVersion) -> if _id = id 
      then _val else sub (nextVersion, id)

let update (hp_arr, id, value) =
  match !hp_arr with
  | PastVersion(_) -> failwith "not current"
  | CurrentVersion(arr) -> 
    let prevValue = Array.get arr id in 
    Array.set arr id (Some value);
    let new_Current = ref (CurrentVersion(arr)) in 
    hp_arr := PastVersion((id, prevValue), new_Current);
    new_Current