type tree =
  | Leaf
  | Node of tree * int * tree

let rec cbt h value =
  if h = 0
    then Leaf
    else let tmp = cbt (h-1) value in
      Node(tmp, value, tmp)

let rec bt n value =
  if n = 0
    then Leaf
    else let child1 = bt (n/2) value in
      if n mod 2 = 1
        then Node(child1, value, child1)
        else let child2 = bt ((n/2) + 1) value in
          Node( child2, value, child1)