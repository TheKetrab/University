(* Bartłomiej Grochowski 300951 *)
(* Lambda.ml *)

type term = 
  | Var of string
  | Lam of string * term
  | App of term * term

exception ConversionFailed

let rec string_of_term (t : term) : string =
  match t with
  | Var(str) -> str
  | Lam(str,t) ->
      Printf.sprintf "λ%s.[%s]" str (string_of_term t)
  | App(t1,t2) ->
      Printf.sprintf "(%s %s)" (string_of_term t1) (string_of_term t2)

let print_term t =
  print_string (string_of_term t)

(* Church numeral *)

let int_of_church (ch : term) : int =
  let rec aux sz acc =
    match sz with
    | Var _ -> acc
    | App(Var _, sz') 
      -> aux sz' acc+1
    | _ -> raise ConversionFailed
  in match ch with
     | Lam(_, Lam(_, sz))
       -> aux sz 0
     | _ -> raise ConversionFailed

let church_of_int (n : int) : term =
  let rec aux k acc : term =
    match k with
    | 0 -> acc
    | _ -> aux (k-1) (App(Var "s", acc))
  in Lam("s", Lam("z", aux n (Var "z")))

(* c0 = λs. λz. z *)
let c0 = Lam("s", Lam("z", Var "z" ))
(* c1 = λs. λz. (s z) *)
let c1 = Lam("s", Lam("z", App(Var "s", Var "z")))
(* c2 = λs. λz. s (s z) *)
let c2 = Lam("s", Lam("z", App(Var "s", App(Var "s", Var "z"))))
(* c3 = λs. λz. s (s (s z)) *)
let c3 = Lam("s", Lam("z", App(Var "s", App(Var "s", App(Var "s", Var "z")))))
(* c4 = λs. λz. s (s (s (s z))) *)
let c4 = Lam("s", Lam("z", App(Var "s", App(Var "s", App(Var "s", App(Var "s", Var "z"))))))

(* Logic *)

(* tru = λt. λf. t *)
let tru = Lam("t", Lam("f", Var("t")))
(* fls = λt. λf. f *)
let fls = Lam("t", Lam("f", Var("f")))
(* if = λl. λm. λn. l m n *)
let iF = Lam("l", Lam("m", Lam("n", App(App(Var "l", Var "m"), Var "n"))))
(* iszro = λm. m (λx. fls) tru *)
let iszro = Lam("m", App(App(Var"m",Lam("x",fls)),tru))
(* and = λb. λc. b c fls *)
let aNd = Lam("b",Lam("c",App(App(Var "b",Var "c"), fls)))
(* or = λb. λc. b tru c *)
let oR = Lam("b",Lam("c",App(App(Var "b", tru), Var "c")))
(* not = λb. b fls tru *)
let not = Lam("b",App(App(Var "b", fls), tru))

(* Pair *)

(* pair = λf.λs.λb. b f s *)
let pair = Lam("f",Lam("s",Lam("b", App(App(Var "b", Var "f"),Var "s") )))
(* fst = λp. p tru *)
let fst = Lam("p", App(Var "p", tru))
(* snd = λp. p fls *)
let snd = Lam("p",App(Var "p", fls))

(* Arith *)

(* scc = λn. λs. λz. s (n s z) *)
let scc = Lam("n", Lam("s", Lam("z",
  App(Var "s", App(App(Var "n", Var "s"), Var "z")))))
(* add = λm. λn. λs. λz. m s (n s z) *)
let add = Lam("m", Lam("n", Lam("s", Lam("z",
  App(App (Var "m", Var "s"),
      App (App (Var "n", Var "s"), Var "z"))))))
(* zz = pair c0 c0 *)
let zz = App(App(pair,c0),c0)
(* ss = λp. pair (snd p) (add c1 (snd p)) *)
let ss = Lam("p", App(
    App(pair, App(snd, Var "p")),
    App(App(add,c1),App(snd,Var "p"))))
(* prd = λm. fst (m ss zz) *)
let prd = Lam("m", App (fst, App(App(Var "m", ss),zz)))
(* sub = λm. λn. n prd m *)
let sub = Lam("m",Lam("n",App(App(Var "n",prd),Var "m")))
(* mul = λm. λn. λs. m (n s) *)
let mul = Lam("m",Lam("n",Lam("s",App(Var "m",App(Var "n",Var "s")))))
(* pow = λm. λn. m n *)
let pow = Lam("m",Lam("n",App(Var "m",Var "n")))
(* eq = λm. λn. and (iszro (m prd n)) (iszro (n prd m)) *)
let eq = Lam("m",Lam("n",App(
  App(aNd,App(iszro,App(App(Var "m",prd),Var "n"))),
  App(iszro,App(App(Var "n",prd),Var "m")))))

(* List *)

(* nil = pair tru tru *)
let nil = App(App(pair,tru),tru)
(* cons = λh. λt. pair fls (pair h t) *)
let cons = Lam("h",Lam("t",App( App(pair,fls) , App(App(pair,Var "h"), Var "t") )))
(* head = λz. fst (snd z) *)
let head = Lam("z",App(fst,App(snd,Var "z")))
(* tail = λz. snd (snd z) *)
let tail = Lam("z",App(snd,App(snd,Var "z")))
(* isnil = fst *)
let isnil = fst


(* omega = λx. x x *)
let omega = Lam("x",App(Var "x",Var "x"))

(* (cbn) omega_f = λx. f (x x) *)
let omega_f = Lam("x",App(Var "f",App(Var "x",Var "x")))

(* (cbv) omega_f' = λx. f (λa. x x a) *)
let omega_f' = Lam("x",App(Var "f",Lam("a",App(App(Var "x",Var "x"), Var "a"))))

(* Y = λf. omega_f omega_f *)
let y = Lam("f",App(omega_f,omega_f))

(* Y' = λf. omega_f' omega_f' *)
let y' = Lam("f",App(omega_f',omega_f'))

let fix = y
