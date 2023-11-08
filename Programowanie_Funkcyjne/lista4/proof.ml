open Logic

type goal = Logic.formula * (string * Logic.formula) list

type proof_tree = 
| Hole of goal
| Theorem of Logic.theorem
| ImplI of proof_tree * goal
| ImplE of proof_tree * goal * proof_tree
| BotE of proof_tree * goal

type proof_tree_ctx = 
| Root
| Left of proof_tree_ctx * proof_tree * goal 
| Right of proof_tree_ctx * goal * proof_tree
| ChildB of proof_tree_ctx * goal 
| ChildI of proof_tree_ctx * goal 


type proof =
| Complete of Logic.theorem
| Incomplete of proof_tree_ctx * goal

let proof named_ass formula =
    Incomplete (Root, (formula, named_ass))

let qed pf =
  match pf with 
  | Complete (th) -> th
  | Incomplete (_, _) -> failwith "dziura w spodniach"

let goal pf =
  match pf with  
  | Complete (th) -> None
  | Incomplete (_, goal) -> Some goal


let rec traverse_down ctx node =
  match node with
  | Hole goal -> Incomplete(ctx, goal)
  | Theorem _ -> failwith "dowiedzione wszystko uwu"
  | ImplI(n1, goal) ->  traverse_down (ChildI(ctx, goal)) n1
  | ImplE(n1,goal,n2) -> traverse_down (Left(ctx, n2, goal)) n1
  | BotE(n1, goal) -> traverse_down (ChildB(ctx, goal)) n1

let rec traverse_up ctx node =
  match ctx with
  | Root -> traverse_down ctx node
  | Left(ctx, right, goal) -> traverse_down (Right(ctx, goal, node)) right
  | Right(ctx, goal, left) -> traverse_up ctx (ImplE(left, goal, node))
  | ChildI(ctx, goal) -> traverse_up ctx (ImplI(node, goal)) 
  | ChildB(ctx, goal) -> traverse_up ctx (BotE(node, goal)) 


let next pf =
  match pf with
  | Complete (th) -> failwith "nie ma kolejnego celu ;c"
  | Incomplete (ctx, goal) -> traverse_up ctx (Hole(goal))

let intro name pf =
  match pf with 
  | Incomplete (ctx, (formula, named_ass)) -> 
    begin match formula with
    | Implication(a, b) -> 
      let new_named = (name, a) :: named_ass in
      let new_ctx = ChildI(ctx, (formula, named_ass)) in
      let new_goal = (b, new_named) in
      Incomplete (new_ctx, new_goal)

    | _ -> failwith "nie implikacja stary" end
  | _ -> failwith "dowiedzione dawno mordko"

let rec impToList (formula : Logic.formula) (goalFormula : Logic.formula) =
  match formula with
  | Neg -> [ formula ]
  | Implication(a, b) -> if b = goalFormula then a :: [goalFormula] else a :: impToList b goalFormula
  | Variable s -> if goalFormula = formula then [formula] else failwith "nie ten format byczqu"

let helpCreate formula proof =
  match proof with 
  | Complete(_) -> failwith "dowiedzione a miało nie być >:("
  | Incomplete(ctx, (formula_goal, named_ass)) ->
    let new_right = Hole (formula, named_ass) in
    let new_ctx = Left(ctx, new_right, (formula_goal, named_ass)) in
    Incomplete(new_ctx, (Implication(formula, formula_goal), named_ass))

let rec complete_goal proof_tree ctx th = 
  match ctx with
  | Root -> Complete(th)
  | Left(ctx, right, (formula_goal, named_ass)) ->
    begin match right with
    | Theorem l -> complete_goal (ImplE(Theorem(th), (formula_goal, named_ass), right)) ctx (Logic.imp_e l th)
    | _ -> traverse_down (Right (ctx, (formula_goal, named_ass), (Theorem th))) right
    end
  | Right(ctx, (formula_goal, named_ass), left) -> 
    begin match left with
    | Theorem l -> complete_goal (ImplE(left, (formula_goal, named_ass), Theorem(th))) ctx (Logic.imp_e l th)
    | _ -> traverse_down (Left (ctx, (Theorem th), (formula_goal, named_ass))) left
    end
  | ChildI(ctx, (formula_goal, named_ass)) -> complete_goal (ImplI(Theorem(th), (formula_goal, named_ass))) ctx (Logic.imp_i formula_goal th) 
  | ChildB(ctx, (formula_goal, named_ass)) -> complete_goal (BotE(Theorem(th), (formula_goal, named_ass))) ctx (Logic.bot_e formula_goal th) 

let rec complete_hole proof theorem = 
  match proof with
  | Complete _ -> proof
  | Incomplete (ctx, (formula_goal, named_ass)) ->
    if Logic.consequence theorem = formula_goal
      then complete_goal (Hole((formula_goal, named_ass))) ctx theorem
      else failwith "bad theorem" 


let apply formula proof =
  match proof with
  | Complete (pf) -> proof
  | Incomplete(ctx, (formula_goal, named_ass)) -> 
    let implication_list = List.rev (impToList formula formula_goal) in
      if implication_list = [formula_goal]
      then
        complete_hole proof (by_assumption formula_goal)
      else
      let (start, rest) = begin match implication_list with
      | Neg :: rs -> (Incomplete( ChildB(ctx, (formula_goal, named_ass) ), (Neg, named_ass)), rs)
      | p :: rs when p = formula_goal -> (Incomplete (ctx, (formula_goal, named_ass)), rs)
      | _ -> failwith "not good" 
      end in
      let rec createProofTree imp_list proof = 
        match proof with
        | Complete(_) -> failwith "dowiedzione a miało nie być >:("
        | Incomplete(ctx,(goal, named_ass)) -> match imp_list with
          | [] -> proof
          | x :: xs -> createProofTree xs (helpCreate x proof)
      
      in createProofTree rest start


let apply_thm thm pf =
  match pf with
  | Complete _ -> pf
  | Incomplete (ctx, (formula_goal, named_ass)) ->
    if (Logic.consequence thm) = formula_goal
      then
        complete_hole pf thm
      else
        let nproof = apply (Logic.consequence thm) pf in
        complete_hole nproof thm

let apply_assm name pf =
  match pf with
  | Complete _ -> pf
  | Incomplete (ctx, (formula_goal, named_ass)) ->
      let af = List.assoc name named_ass in
      apply_thm (Logic.by_assumption af) pf

(* let pp_print_proof fmtr pf =
  match goal pf with
  | None -> Format.pp_print_string fmtr "No more subgoals"
  | Some(g, f) ->
    Format.pp_open_vbox fmtr (-100);
    g |> List.iter (fun (name, f) ->
      Format.pp_print_cut fmtr ();
      Format.pp_open_hbox fmtr ();
      Format.pp_print_string fmtr name;
      Format.pp_print_string fmtr ":";
      Format.pp_print_space fmtr ();
      pp_print_formula fmtr f;
      Format.pp_close_box fmtr ());
    Format.pp_print_cut fmtr ();
    Format.pp_print_string fmtr (String.make 40 '=');
    Format.pp_print_cut fmtr ();
    pp_print_formula fmtr f;
    Format.pp_close_box fmtr () *)
