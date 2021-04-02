(* Bartłomiej Grochowski 300951 *)
(* Lambda.ml *)

(* 
  Rozważamy termy w postaci nameless!
  
  Jeśli przy złapaniu wyjątku chcemy użyć jego argumentu, to procedura obsługi
  MUSI BYĆ EXPLICITE lambdą a nie tylko obliczać się do lambdy - świadome łapanie wyjątku

  Konwencja nazewnicza:
    e - nazwa wyjątku
    t - termy
    tyT, tyS - typy
    es - estate

*)

type ty =
  | Nat
  | Bool
  | Unit
  | Arr of ty * ty

type term = 
  (* Wyrażenia boolowskie *)
  | True
  | False
  (* Liczby naturalne *)
  | Num of int
  | Add of term * term
  | Sub of term * term
  | Mul of term * term
  | Div of term * term
  (* Unit *)
  | UnitVal
  (* Predykaty *)
  | Equal of term * term
  | Greater of term * term
  | IsZero of term
  (* Wyrażenia warunkowe *)
  | If of term * term * term
  (* Rachunek lambda *)
  | Var of int
  | Lam of ty * term
  | App of term * term
  (* Fix *)
  | Fix of term
  (* Wyjątki *)
  | Exception of string * ty * term
  | Throw of string * term * ty
  | TryCatch of term * ((string * term) list)

exception TypeError of string
exception EvalException
exception UnhandledException of string * term

type gamma = ty list
type delta = (string * ty) list
type context = gamma * delta

(* Stan obliczeń
    > Albo doliczyliśmy się do wartości,
    > Albo został zgłoszony wyjątek z jakimś argumentem
      i leci w górę aż zostanie złapany
*)
type estate =
  | Val of term
  | Err of string * term

let err_of_estate (es : estate) : string * term =
  match es with
  | Err(e,t) -> (e,t)
  | _ -> raise EvalException

let val_of_estate (es : estate) : term =
  match es with
  | Val(v) -> v
  | _ -> raise EvalException

let iserr (es : estate) : bool =
  match es with
  | Err(_,_) -> true
  | _ -> false


let newctx = ([],[])

let rec string_of_type (t : ty) : string =
  match t with
  | Nat -> "Nat"
  | Bool -> "Bool"
  | Unit -> "Unit"
  | Arr(tyT1,tyT2) -> Printf.sprintf "{%s->%s}"
      (string_of_type tyT1) (string_of_type tyT2)

let rec string_of_term (t : term) : string =
  match t with
  | True -> "true"
  | False -> "false"
  | Num(n) -> string_of_int n
  | Add(t1,t2) -> Printf.sprintf "(%s + %s)"
      (string_of_term t1) (string_of_term t2)
  | Sub(t1,t2) -> Printf.sprintf "(%s - %s)"
      (string_of_term t1) (string_of_term t2)
  | Mul(t1,t2) -> Printf.sprintf "(%s * %s)"
      (string_of_term t1) (string_of_term t2)
  | Div(t1,t2) -> Printf.sprintf "(%s / %s)"
      (string_of_term t1) (string_of_term t2)
  | UnitVal -> "unit"
  | Equal(t1,t2) -> Printf.sprintf "(%s == %s)"
      (string_of_term t1) (string_of_term t2)
  | Greater(t1,t2) -> Printf.sprintf "(%s > %s)"
      (string_of_term t1) (string_of_term t2)
  | IsZero(t1) -> Printf.sprintf "iszero(%s)"
      (string_of_term t1)
  | If(t0,t1,t2) -> Printf.sprintf "[if %s then %s else %s]"
      (string_of_term t0) (string_of_term t1) (string_of_term t2)
  | Var(i) -> "x" ^ (string_of_int i)
  | Lam(tyT1,t2) -> Printf.sprintf "λ:%s.[%s]"
      (string_of_type tyT1) (string_of_term t2)
  | App(t1,t2) -> Printf.sprintf "(%s %s)"
      (string_of_term t1) (string_of_term t2)
  | Fix(t1) -> Printf.sprintf "Fix{%s}" (string_of_term t1)
  | Exception(e,tyT,t) -> Printf.sprintf "exception %s of %s in\n%s"
      e (string_of_type tyT) (string_of_term t)
  | Throw(e,t,tyT) -> Printf.sprintf "THROW{%s %s as %s}"
      e (string_of_term t) (string_of_type tyT)
  | TryCatch(t,lst) -> 
      let rec str_of_catch lst =
        match lst with
        | [] -> ""
        | (e,t) :: tl -> Printf.sprintf "\nCATCH{%s => %s}%s"
            e (string_of_term t) (str_of_catch tl)
      in "\nTRY{" ^ (string_of_term t) ^ "}" ^ (str_of_catch lst)


let get_type (ctx : context) (i : int) : ty =
  match ctx with (gamma,_) -> List.nth gamma i

let add_binding (ctx : context) (tp : ty) : context =
  match ctx with (gamma,delta) -> (tp::gamma,delta)

let add_exn (ctx : context) (e : string) (tyT : ty) =
  match ctx with (gamma,delta) -> (gamma,(e,tyT)::delta)

let arrerr_msg msg tm currentT fullterm : string = 
  (Printf.sprintf "\n-----\n%s > Type error in term: %s\n > term: %s\n > was expected to be of ARRAY TYPE\n > but its type is: %s\n-----\n"  
      msg (string_of_term fullterm) (string_of_term tm) (string_of_type currentT))

let fverr_msg msg i : string =
  (Printf.sprintf "\n-----\n%s > Free variable detected.\n > Value: %s\n-----\n"
      msg (string_of_int i))

let tyerr_msg msg tm expectedT currentT fullterm : string =
  (Printf.sprintf "\n-----\n%s > Type error in term: %s\n > term: %s\n > was expected to be of type: %s\n > but its type is: %s\n-----\n"
      msg (string_of_term fullterm) (string_of_term tm) (string_of_type expectedT) (string_of_type currentT))

let exnotf_msg msg e tyS fullterm : string = 
  (Printf.sprintf "\n-----\n%s > Cannot find exception %s of type %s\n > in term: %s\n-----\n"
      msg e (string_of_type tyS) (string_of_term fullterm))

let catcherr_msg msg e ti tyT fullterm : string = 
  (Printf.sprintf "\n-----\n%s > Unable to find valid type for catch statement.\n > CATCH{%s => %s}\n > Expected type of ti is: %s\n > Full term: %s\n-----\n"
      msg e (string_of_term ti) (string_of_type tyT) (string_of_term fullterm))


(* Czy wyjątki są w delcie? 
    > Czasem chcemy sprawdzić tylko po nazwie (catch, gdzie ti nie jest lambdą)
    > Chcemy sprawdzać, czy konkretny wyjątek (name,type) należy do delty
*)
let exception_name_in_ctx (ctx : context) (e : string ) =
  let (_,delta) = ctx in
  let rec aux ds =
    match ds with
      | [] -> false
      | (ei,_)::tl when (ei = e) -> true
      | hd :: tl -> aux tl
  in aux delta

let exception_in_ctx (ctx : context) (e : string ) (tyT : ty) =
  let (_,delta) = ctx in
  let rec aux ds =
    match ds with
      | [] -> false
      | (ei,tyTi)::tl when
          (ei = e && tyTi = tyT) -> true
      | hd :: tl -> aux tl
  in aux delta


(* Substitution nie różni się od tego, co było na pracowni 1 dla indeksów de'Bruijna 
  > Shift rekurencyjnie zmienia cały term przesuwając o d wszystkie zmienne, które są wolne
  > Zmienne wolne są te, które mają indeksy większe niż ilość przekraczanych lambda abstrakcji (c)
*)
let shift (d : int) (t : term) : term =
  let rec aux c t =
    match t with

    | Add(t1,t2) -> Add(aux c t1, aux c t2)
    | Sub(t1,t2) -> Sub(aux c t1, aux c t2)
    | Mul(t1,t2) -> Mul(aux c t1, aux c t2)
    | Div(t1,t2) -> Div(aux c t1, aux c t2)
    | Equal(t1,t2) -> Equal(aux c t1, aux c t2)
    | Greater(t1,t2) -> Greater(aux c t1, aux c t2)
    | IsZero(t1) -> IsZero(aux c t1)
    | If(t0,t1,t2) -> If(aux c t0, aux c t1, aux c t2)

    | Var(i) -> if i >= c then Var(i+d) else Var(i)
    | Lam(tyT,t') -> Lam(tyT,aux (c+1) t')
    | App(t1,t2) -> App(aux c t1, aux c t2)

    | Fix(t1) -> Fix(aux c t1)

    | Exception(e,tyT,t) -> Exception(e, tyT, aux c t)
    | Throw(e,t,tyT) -> Throw(e, aux c t, tyT)
    | TryCatch(t,cs) -> TryCatch(aux c t, 
        List.map (fun (ei,ti) -> (ei,aux c ti)) cs)

    | t -> t (* reszta - wartości, takie same *)

  in aux 0 t

(* [j -> t']t  albo  [t'/j]t *)
let substitution (j : int) (t' : term) (t : term) : term =
  let rec aux c t =
    match t with

    | Add(t1,t2) -> Add(aux c t1, aux c t2)
    | Sub(t1,t2) -> Sub(aux c t1, aux c t2)
    | Mul(t1,t2) -> Mul(aux c t1, aux c t2)
    | Div(t1,t2) -> Div(aux c t1, aux c t2)
    | Equal(t1,t2) -> Equal(aux c t1, aux c t2)
    | Greater(t1,t2) -> Greater(aux c t1, aux c t2)
    | IsZero(t1) -> IsZero(aux c t1)
    | If(t0,t1,t2) -> If(aux c t0, aux c t1, aux c t2)

    | Var(i) -> if i = j+c then shift c t' else Var(i)
    | Lam(tyT,t1) -> Lam(tyT,aux (c+1) t1)
    | App(t1,t2) -> App(aux c t1, aux c t2)

    | Fix(t1) -> Fix(aux c t1)

    | Exception(e,tyT,t) -> Exception(e, tyT, aux c t)
    | Throw(e,t,tyT) -> Throw(e, aux c t, tyT)
    | TryCatch(t,cs) -> TryCatch(aux c t, 
        List.map (fun (ei,ti) -> (ei,aux c ti)) cs)

    | t -> t (* Reszta - wartości, takie same *)

  in aux 0 t

let subst_top s t =
  shift (-1) (substitution 0 (shift 1 s) t)

(* Typeof, semantyka naturalna, według reguł typowania *)
let rec assert_type (ctx : context) (tm : term) (tp : ty) (fullterm : term) =
  let tp' = typeof ctx tm in
    if tp' = tp then ()
    else raise (TypeError (tyerr_msg "" tm tp tp' fullterm))

and typeof (ctx : context) (t : term) : ty =
  match t with
  | True -> Bool
  | False -> Bool
  | Num(_) -> Nat
  | Add(t1,t2) -> 
      assert_type ctx t1 Nat t;
      assert_type ctx t2 Nat t; Nat;
  | Sub(t1,t2) -> 
      assert_type ctx t1 Nat t;
      assert_type ctx t2 Nat t; Nat;
  | Mul(t1,t2) -> 
      assert_type ctx t1 Nat t;
      assert_type ctx t2 Nat t; Nat;
  | Div(t1,t2) -> 
      assert_type ctx t1 Nat t;
      assert_type ctx t2 Nat t; Nat;
  | Equal(t1,t2) ->
      let tyT1 = typeof ctx t1 in
      assert_type ctx t2 tyT1 t; Bool;
  | Greater(t1,t2) ->
      assert_type ctx t1 Nat t;
      assert_type ctx t2 Nat t; Bool;
  | IsZero(t1) -> assert_type ctx t1 Nat t; Bool;
  | If(t0,t1,t2) -> assert_type ctx t0 Bool t;
      let tyT1 = typeof ctx t1 in
      assert_type ctx t2 tyT1 t; tyT1;
  
  | Var(i) -> ( 
      try get_type ctx i
      with Failure(_) -> raise (TypeError (fverr_msg "" i))
    )
  | Lam(tyT1,t2) ->
      let ctx' = add_binding ctx tyT1 in
      let tyT2 = typeof ctx' t2 in
        Arr(tyT1,tyT2)
  | App(t1,t2) ->
      let tyT1 = typeof ctx t1 in
      let tyT2 = typeof ctx t2 in
      (match tyT1 with
         | Arr(tyT11,tyT12) ->
             if tyT2 = tyT11 then tyT12
             else raise (TypeError (tyerr_msg 
              " > Application failed.\n" t2 tyT11 tyT2 t))
          | _ -> raise (TypeError (arrerr_msg
              " > Application failed.\n" t1 tyT1 t)))

  | Fix(t1) ->
      let tyT1 = typeof ctx t1 in
      (match tyT1 with
      | Arr(tyT11,tyT12) when tyT11 = tyT12 -> tyT11
      | _ -> raise (TypeError (arrerr_msg
            " > Wrong type for fix construction.\n" t1 tyT1 t)))

  | Throw(e,t',tyT) ->
      let tyS = typeof ctx t' in
        if (exception_in_ctx ctx e tyS) then tyT
        else raise (TypeError (exnotf_msg "" e tyS t))
  | Exception(e,tyS,t) ->
      let ctx' = add_exn ctx e tyS in
        typeof ctx' t
  | TryCatch(t',cs) ->
      let tyT = typeof ctx t' in
        (* dla każdego handlera musimy sprawdzić: *)
        List.iter (fun (e,ti) ->
          match ti with
          | Lam(tyTi',ti') ->
              (* wyjątek e:tyTi' musi należeć do kontekstu *)
              if not (exception_in_ctx ctx e tyTi')
              then raise (TypeError (exnotf_msg "" e tyTi' t)) else
              let ctx' = add_binding ctx tyTi' in
              let tyS = typeof ctx' ti' in
              if tyS = tyT then () (* typ po podstawieniu musi się zgadzać *)
              else raise (TypeError (tyerr_msg
                " > In catch-lambda handler.\n" ti' tyT tyS t))
          | ti' ->
              (* wyjątek e musi należeć do kontekstu *)
              if not (exception_name_in_ctx ctx e)
              then raise (TypeError (exnotf_msg 
                " > Any type, Unit is an example.\n" e Unit t)) else
              let tyS = typeof ctx ti' in
              if tyS = tyT then () (* typ musi się zgadzać, nie robimy podstawienia z argumentu *)
              else raise (TypeError (tyerr_msg
                " > In catch-term handler.\n" ti' tyT tyS t))
        ) cs; tyT;
  | UnitVal -> Unit

(* Ewaluacja: semantyka naturalna, call by value
    > Jeśli nadaliśmy typ termowi, to mamy gwarancję, że doliczymy się do wartości
    > 'evalit' zwraca estate, czyli albo error albo wartość
    > Podchodzimy do tego defensywnie - sprawdzamy czy jakieś wywołanie rekurencyjne nie wyrzuciło wyjątku
*)
let eval (t : term) : (term * ty) =

  let tyT = typeof newctx t in

    let rec evalit_arith ctx t1 t2 op : estate =
      let es1 = (evalit ctx t1) in
      if iserr es1 then es1 else
      let es2 = (evalit ctx t2) in
      if iserr es2 then es2 else
      match es1,es2 with
      | Val(Num(v1)), Val(Num(v2)) -> Val(Num(op v1 v2))
      | _ -> raise EvalException

    and evalit (ctx : context) (t : term) : estate =
      match t with
      (* Arith *)
      | Add(t1,t2) -> evalit_arith ctx t1 t2 ( + )
      | Sub(t1,t2) -> evalit_arith ctx t1 t2 ( - )
      | Mul(t1,t2) -> evalit_arith ctx t1 t2 ( * )
      | Div(t1,t2) -> evalit_arith ctx t1 t2 ( / )
      (* Predykaty i if *)
      | Equal(t1,t2) ->
          let es1 = evalit ctx t1 in
          if iserr es1 then es1 else
          let es2 = evalit ctx t2 in
          if iserr es2 then es2 else
            if (es1 = es2) then Val(True) else Val(False)
      | Greater(t1,t2) ->
          let es1 = evalit ctx t1 in
          if iserr es1 then es1 else
          let es2 = evalit ctx t2 in
          if iserr es2 then es2 else
          (match (es1,es2) with
          | Val(Num(n1)), Val(Num(n2)) ->
              if (n1 > n2) then Val(True) else Val(False)
          | _ -> raise EvalException)
      | IsZero(t1) ->
          let es1 = evalit ctx t1 in
          if iserr es1 then es1 else
          if (es1 = Val(Num(0))) then Val(True) else Val(False)
      | If(t0,t1,t2) ->
          let es0 = evalit ctx t0 in
          if iserr es0 then es0 else
          (match es0 with
           | Val(True) -> evalit ctx t1
           | Val(False) -> evalit ctx t2
           | _ -> raise EvalException)
      (* Rachunek lambda *)
      | App(t1,t2) ->
          let es2 = evalit ctx t2 in
          if iserr es2 then es2 else
          let es1 = evalit ctx t1 in
          if iserr es1 then es1 else
          let v1, v2 = val_of_estate es1, val_of_estate es2 in
          (match v1 with
           | Lam(tyT1,t12) -> 
               let t12' = subst_top v2 t12 in
                 evalit ctx t12'
           | _ -> raise EvalException)

      (* Fix *)
      | Fix(t1) ->
          let est1 = evalit ctx t1 in
          if iserr est1 then est1 else
          let v1 = val_of_estate est1 in          
          (match v1 with
           | Lam(tyT11,t12) -> 
               let t12' = subst_top (Fix(t1)) t12 in
                 evalit ctx t12'
           | _ -> raise EvalException)

      (* Wyjątki *)
      | Exception(e,tyT,t) ->
          let ctx' = add_exn ctx e tyT in
            evalit ctx' t
      | Throw(e,t,tyT) ->
          Err(e,t)
      | TryCatch(t,cs) ->
          let es = evalit ctx t in
          if not (iserr es) then es else
          let (e,x) = err_of_estate es in
          let tyX = typeof ctx x in
          (* szukamy, czy jest jakiś handler dla tego wyjątku:
             - musi się zgadzać nazwa wyjątku i ti ma być typu tyT w kontekście rozszerzonym o tyX
             - jesli zgadza się nazwa wyjątku i typ argumentu, to obsługujemy
             - jesli nie znaleźliśmy nic, to przepuszczamy wyjątek dalej *)
          let handler = List.find_opt (fun (ei,ti) ->
              if e <> ei then false else
              match ti with
              | Lam(tyTi',ti') ->
                  if tyTi' = tyX then true else false
              | _ -> true
            ) cs in 
          (match handler with
          | Some(ei,ti) ->
              (* obliczamy x i ti (przy czym jeśli ti to lambda to aplikujemy)
                 ale ostrożnie, bo też mogą rzucać wyjątki!
                 np. gdy chcemy złapać jakiś wyjątek, zmienić jego typ i rzucić dalej *)
              let esx = evalit ctx x in
              if iserr esx then esx else
              let esti = evalit ctx ti in
              if iserr esti then esti else
              let vx = val_of_estate esx in
              let vti = val_of_estate esti in
              (match vti with
               | Lam(_,_) -> (* zaaplikuj argument wyjątku do ciała handlera i oblicz *)
                    evalit ctx (App(vti,vx))
               | _ -> Val(vti))
          | None -> es) (* wyjątek nieobsłużony - rzuć wyżej *)
      (* Reszta termów to wartości *)
      | v -> Val(v)

    in let es = evalit newctx t in
  
  match es with
  | Val(v) -> (v,tyT)
  | Err(e,x) -> raise (UnhandledException(e,x))


let run (t : term) : unit =
  try 
    let (v,tyT) = eval t in
    print_string (Printf.sprintf 
      "\n-----\n > Value of given term is: %s\n > Type of given term is: %s\n-----\n"
        (string_of_term v) (string_of_type tyT))
  with
  | TypeError(msg) ->
    print_string msg
  | UnhandledException(e,x) ->
    print_string (Printf.sprintf 
      "\n-----\n > Unhandled exception: %s\n > Argument: %s\n-----\n"
        e (string_of_term x))

