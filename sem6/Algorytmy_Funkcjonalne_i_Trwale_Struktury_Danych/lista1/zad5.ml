let rec find_min cmp xs curr_min prev_min = 
  match xs with
  | [] -> curr_min
  | (x :: xs) -> 
      if (cmp x curr_min) && (not (cmp x prev_min)) && prev_min != x
                    then find_min cmp xs x prev_min
                    else find_min cmp xs curr_min prev_min

let rec find_max cmp xs curr_max =
  match xs with
  | [] -> curr_max
  | (x :: xs) -> if not (cmp x curr_max)
                  then find_max cmp xs x
                  else find_max cmp xs curr_max

let rec find_min_no_prev cmp xs curr_min = 
  match xs with
  | [] -> curr_min
  | (x :: xs) -> if cmp x curr_min
                    then find_min_no_prev cmp xs x
                    else find_min_no_prev cmp xs curr_min

let rec cnt_min xs minn cnt = 
  match xs with 
  | [] -> cnt
  | (x :: xs) -> if x == minn then cnt_min xs minn (cnt+1)
                              else cnt_min xs minn cnt

let rec add_to_acc cnt x acc = 
  if cnt == 0 then acc
  else add_to_acc (cnt-1) x (x :: acc)


let insert_sort cmp list =
  (* wywołanie rekurencyjne *)
  let rec _insert_sort cmp list acc prev_min maks =
    match list with
    | [] -> acc
    | (x :: xs) -> 
    let minn = find_min cmp list maks prev_min in
      if prev_min = minn
        then List.rev acc
        else
          let cnt = cnt_min list minn 0 in 
            _insert_sort cmp list (add_to_acc cnt minn acc) minn  maks in
    
  (* pierwsze wywołanie insert sorta (nie znamy poprzedniego minimum) *)
  let rec _insert_sort_first cmp list acc maks =
    match list with
    | [] -> acc
    | (x :: xs) -> 
    let minn = find_min_no_prev cmp list maks in 
      let cnt = cnt_min list minn 0 in 
      _insert_sort cmp list (add_to_acc cnt minn acc) minn maks in

  match list with 
  | [] -> []
  | (x :: xs) -> let maks = find_max cmp list x in 
    _insert_sort_first cmp list [] maks

let less a b = 
  a < b
;;