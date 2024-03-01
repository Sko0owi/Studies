let x = Implication( (Variable "p"), Implication (Implication((Variable "p"), (Variable "q")), (Variable "q")));;

let test1 = proof [] x 
  |> intro "H1"
  |> intro "H2"
  |> apply_assm "H2"
  |> apply_assm "H1"
  (* |> qed *)


  let pr = Implication( (Variable "p"), (Variable "r"))
  let pq = Implication( (Variable "p"), (Variable "q"))
  let pqpr = Implication(pq, pr)
  
  let qr = Implication( (Variable "q"), (Variable "r"))
  let pqr = Implication( (Variable "p"), qr)
  let x = Implication(pqr, pqpr)
  let test2 = proof [] x 
    |> intro "H1"
    |> intro "H2"
    |> intro "H3"
    |> apply_assm "H1"
    |> apply_assm "H3"
    |> apply_assm "H2" 
    |> apply_assm "H3";;

let pB = Implication( (Variable "p"), Neg)
let pBp = Implication( pB, (Variable "p"))
let pBpp = Implication(pBp, (Variable "p"))

let pBB = Implication(pB, Neg)
let pBBp = Implication(pBB, (Variable "p"))

let x = Implication(pBpp, pBBp)

let test3 = proof [] x 
  |> intro "H1"
  |> intro "H2"
  |> apply_assm "H1"
  |> intro "H3"
  |> apply_assm "H1"
  |> apply_assm "H2"
  |> apply_assm "H3"

let x = Implication(pBBp, Implication(pBp, (Variable "p"))) 


let test4 = proof [] x 
  |> intro "H1"
  |> intro "H2"
  |> apply_assm "H1"
  |> intro "H3"
  |> apply_assm "H3"
  |> apply_assm "H2"
  |> apply_assm "H3"

  