(* Bartłomiej Grochowski 300951 *)
(* Test.ml *)

(*
    Testy automatyczne
  
    Moduł testowy to skrypt dla interpretera OCamla.
    Należy uruchamiać go: ocaml < Test.ml

    Procedura 'test' wywoływana jest na 'częściach testowych'.
    Każda taka część to nazwa oraz lista funkcji, które są testami.
    Podczas testowania każdy test na liście jest wywoływany
    a oczekiwany rezultat sprawdzany jest asercją.

*)

#load "str.cma" ;;
#load "Util.cmo" ;;
#load "Lambda.cmo" ;;
#load "DeBruijn.cmo" ;;
#load "Parser.cmo" ;;

open Lambda ;;
open DeBruijn ;;
open Parser ;;

let test (name,fun_list) : unit =
  try List.iter (fun f -> f()) fun_list
  with e -> print_string
    (Printf.sprintf "\n\nAssertion failed at \"%s\"\nMsg: %s\n\n"
      name (Printexc.to_string e))
;;

(* ----- --------------- ----- *)
(* ----- Testy konwersji ----- *)
(* ----- --------------- ----- *)
test ("Conversion", [

(* exp1 = λx (λy y x) x *)
(fun () -> begin
  let exp = Lam("x", App ( Lam("y", App(Var "y", Var "x") ), Var "x"))
  in let (br, fv) = bruijn_of_term exp
  in let tr = term_of_bruijn_ctx br fv
  in print_string (Printf.sprintf "\n1. %s , %s"
    (string_of_bruijn br) (string_of_term tr)) ;
  assert (string_of_bruijn br = "λ[(λ[(0 1)] 0)]") ;
end);

(* exp2 = (λ x. x x) (λ x. x) *)
(fun () -> begin
  let exp = App(Lam("x", App (Var "x", Var "x")),
                Lam("x", Var "x"))
  in let (br, fv) = bruijn_of_term exp
  in let tr = term_of_bruijn_ctx br fv
  in print_string (Printf.sprintf "\n2. %s , %s"
    (string_of_bruijn br) (string_of_term tr)) ;
  assert (string_of_bruijn br = "(λ[(0 0)] λ[0])")
end);

(* exp3 = (λx. (λx. x)) (λx. x) *)
(fun () -> begin
  let exp = App(Lam("x", Lam("x", Var "x")),
                Lam("x",Var "x"))
  in let (br, fv) = bruijn_of_term exp
  in let tr = term_of_bruijn_ctx br fv
  in print_string (Printf.sprintf "\n3. %s , %s"
    (string_of_bruijn br) (string_of_term tr)) ;
  assert (string_of_bruijn br = "(λ[λ[0]] λ[0])") ;
end);

(* exp4 = add *)

(fun () -> begin
  let (br, fv) = bruijn_of_term add
  in let tr = term_of_bruijn_ctx br fv
  in print_string (Printf.sprintf "\n4. %s , %s"
    (string_of_bruijn br) (string_of_term tr)) ;
  assert (string_of_bruijn br = "λ[λ[λ[λ[((3 1) ((2 1) 0))]]]]") ;
end);

(* exp5 = y' = fix *)
(fun () -> begin
  let (br, fv) = bruijn_of_term y'
  in let tr = term_of_bruijn_ctx br fv
  in print_string (Printf.sprintf "\n5. %s , %s"
    (string_of_bruijn br) (string_of_term tr)) ;
  assert (string_of_bruijn br = "λ[(λ[(1 λ[((1 1) 0)])] λ[(1 λ[((1 1) 0)])])]") ;
end)

]);;

(* ----- ---------------- ----- *)
(* ----- Testy środowiska ----- *)
(* ----- ---------------- ----- *)
test ("Environment", [

(* testy dla termów otwartych *)

(* exp1 = (λx. x y)(λx. x z) *)
(fun () -> begin
  let exp = App(
    Lam("x",(App(Var "x", Var "y"))),
    Lam("x",(App(Var "x", Var "z"))))
  in let (br, fv) = bruijn_of_term exp
  in let tr = term_of_bruijn_ctx br fv
  in print_string (Printf.sprintf "\n1. %s , %s"
    (string_of_bruijn br) (string_of_term tr)) ;
  assert (string_of_bruijn br = "(λ[(0 1)] λ[(0 2)])") ;
end);

(* exp2 = λx. (λx. x y) λy k λx. k x y *)
(fun () -> begin
  let exp = Lam("x", App(
    Lam("x", App(Var "x", Var "y")),
    Lam("y", App(Var "k",
      Lam("x", App(App(Var "k", Var "x"), Var "y"))))))
  in let (br, fv) = bruijn_of_term exp
  in let tr = term_of_bruijn_ctx br fv
  in print_string (Printf.sprintf "\n2. %s , %s"
    (string_of_bruijn br) (string_of_term tr)) ;
  assert (string_of_bruijn br = "λ[(λ[(0 2)] λ[(3 λ[((4 0) 1)])])]") ;
end);

(* exp3 = λa. b a λb. a c λa. λb. λc. λd. d *)
(fun () -> begin
  let exp = Lam("a", App(App(Var "b", Var "a"),
    Lam ("b", App(App(Var "a", Var "c"),
      Lam("a",Lam("b",Lam("c",Lam("d",Var "d"))))))))
  in let (br, fv) = bruijn_of_term exp
  in let tr = term_of_bruijn_ctx br fv
  in print_string (Printf.sprintf "\n3. %s , %s"
    (string_of_bruijn br) (string_of_term tr)) ;
  assert (string_of_bruijn br = "λ[((1 0) λ[((1 3) λ[λ[λ[λ[0]]]])])]") ;
end);

(* testy ze środowiskiem na wejściu (TAPL str. 77) *)

(* exp4 = x (y z) , fv = [b,a,z,y,x] *)
(fun () -> begin
  let exp = App(Var "x", (App( Var "y", Var "z")))
  in let (br, fv) = bruijn_of_term_ctx exp [] ["b";"a";"z";"y";"x"]
  in let tr = term_of_bruijn_ctx br fv
  in print_string (Printf.sprintf "\n4. %s , %s"
    (string_of_bruijn br) (string_of_term tr)) ;
  assert (string_of_bruijn br = "(4 (3 2))") ;
end);

(* exp5 = λw. y w , fv = [b,a,z,y,x] *)
(fun () -> begin
  let exp = Lam("w", App(Var "y", Var "w"))
  in let (br, fv) = bruijn_of_term_ctx exp [] ["b";"a";"z";"y";"x"]
  in let tr = term_of_bruijn_ctx br fv
  in print_string (Printf.sprintf "\n5. %s , %s"
    (string_of_bruijn br) (string_of_term tr)) ;
  assert (string_of_bruijn br = "λ[(4 0)]") ;
end);

(* exp6 = λw. λa. x , fv = [b,a,z,y,x] *)
(fun () -> begin
  let exp = Lam("w", Lam("a", Var "x"))
  in let (br, fv) = bruijn_of_term_ctx exp [] ["b";"a";"z";"y";"x"]
  in let tr = term_of_bruijn_ctx br fv
  in print_string (Printf.sprintf "\n6. %s , %s"
    (string_of_bruijn br) (string_of_term tr)) ;
  assert (string_of_bruijn br = "λ[λ[6]]") ;
end);
]);;

(* ----- -------------- ----- *)
(* ----- Testy redukcji ----- *)
(* ----- -------------- ----- *)
test ("ß-reduction", [

(* exp1 = (λx. x) y *)
(fun () -> begin
  
  let exp = App(Lam("x",Var "x"),Var "y")
  in let br = reduce_term_to_bruijn cbn_natural_eval exp
  in print_string (Printf.sprintf "\n1. %s" (string_of_bruijn br)) ;
  assert (string_of_bruijn br = "0") ;
  
end);

(* exp2 = (λy. (λx. x) y) z *)
(fun () -> begin
  let exp = App(Lam("y",App(Lam("x",Var "x"),Var "y")), Var "z")
  in let br = reduce_term_to_bruijn cbn_natural_eval exp
  in print_string (Printf.sprintf "\n2. %s" (string_of_bruijn br)) ;
  assert (string_of_bruijn br = "0") ;
end);

(* exp3 = (λx. x) (λy. y) *)
(fun () -> begin
  let exp = App(Lam("x",Var "x"),Lam("y",Var "y"))
  in let br = reduce_term_to_bruijn cbn_natural_eval exp
  in print_string (Printf.sprintf "\n3. %s" (string_of_bruijn br)) ;
  assert (string_of_bruijn br = "λ[0]")
end);

(* exp4 = iszro c0 *)
(fun () -> begin
  let exp = App(iszro,c0) in
  print_string "\n4" ;
  assert (betaequal exp tru) ;
end);

(* exp5 = iszro c1 *)
(fun () -> begin
  let exp = App(iszro,c1) in
  print_string "\n5" ;
  assert (betaequal exp fls) ;
end);

(* exp6 = (λx. x x)(λx. x) *)
(fun () -> begin
  let exp = App(Lam("x",App(Var "x", Var "x")),Lam("x",Var "x"))
  in let br = reduce_term_to_bruijn cbn_natural_eval exp
  in print_string (Printf.sprintf "\n6. %s" (string_of_bruijn br)) ;
  assert (string_of_bruijn br = "λ[0]") ;
end);

(* exp7 = ((λx. x)(λy. y))(λz. z) *)
(fun () -> begin
  let exp = App(App(Lam("x",Var "x"),Lam("y",Var "y")), Lam("z",Var "z"))
  in let br = reduce_term_to_bruijn cbn_natural_eval exp
  in print_string (Printf.sprintf "\n7. %s" (string_of_bruijn br)) ;
  assert (string_of_bruijn br = "λ[0]") ;
end);

(* exp8 = scc c0 *)
(fun () -> begin
  let exp = App(scc,c0) in
  print_string "\n8" ;
  assert (betaequal exp (church_of_int 1)) ;
end);

(* exp9 = add c1 c2 *)
(fun () -> begin
  let exp = App(App(add,c1),c2) in
  print_string "\n9" ;
  assert (betaequal exp c3) ;
end);

(* exp10 = mul c2 c2 *)
(fun () -> begin
  let exp = App(App(mul,c2),c2) in
  print_string "\n10" ;
  assert (betaequal exp c4) ;
end);

]);;

(* ----- --------------------- ----- *)
(* ----- Testy ß-równoważności ----- *)
(* ----- --------------------- ----- *)
test ("ß-equality", [

(* 1: (λx. x) y  =?=  (λx. x) z *)
(fun () -> begin
  let exp1 = App(Lam("x",Var "x"),Var "y")
  in let exp2 = App(Lam("x",Var "x"),Var "z")
  in print_string "\n1" ;
  assert (betaequal exp1 exp2 = false)
end);

(* 2: (λx. x y)(λz. z)  =?=  (λa. a y)(λb. b) *)
(fun () -> begin
  let exp1 = App( Lam("x",App(Var "x",Var "y")) , Lam("z",Var "z") )
  in let exp2 = App( Lam("a",App(Var "a",Var "y")) , Lam("b",Var "b") )
  in print_string "\n2" ;
  assert (betaequal exp1 exp2 = true)
end);

(* 3: (λx. y)(λy. a)  =?=  (λz. y)(λx. b) *)
(* przed redukcją fv się różnią, ale po redukcji a i b nie istnieją,
   bo zniknęły w redukcji - tylko 'y' istnieje i jest porównywany *)
(fun () -> begin
  let exp1 = App(Lam("x",Var "y"),Lam("y",Var "a"))
  in let exp2 = App(Lam("z",Var "y"),Lam("x",Var "b"))
  in print_string "\n3" ;
  assert (betaequal exp1 exp2 = true)
end);

(* 4: (λx. x y)(λy. a)  =?=  (λz. z t)(λx. b) *)
(* tutaj znikają y i t, ale za to a i b nie znikają i okazuje się,
   że mimo tego samego indeksu reprezentują różne zmienne w swoich kontekstach *)
(fun () -> begin
  let exp1 = App(Lam("x",App(Var "x", Var "y")), Lam("y",Var "a"))
  in let exp2 = App(Lam("z",App(Var "z", Var "t")), Lam("x",Var "b"))
  in print_string "\n4" ;
  assert (betaequal exp1 exp2 = false)
end);

]);;

(* ----- ------------- ----- *)
(* ----- Testy wyrażeń ----- *)
(* ----- ------------- ----- *)
test ("Expressions", [

(* 1: if T/F (add c2 c1) (scc c0) *)
(fun () -> begin
  let exp pred = App(App(App(iF,pred),App(App(add,c2),c1)),App(scc,c0))
  in print_string "\n1a" ;
  assert (betaequal (exp tru) c3) ;
  print_string "\n1b" ;
  assert (betaequal (exp fls) c1) ;
end);

(* 2: cons(c0,cons(c1,cons(c2,nil)))) *)
(fun () -> begin
  (* lst = [0;1;2] *)
  let lst = App(App(cons,c0),
    App(App(cons,c1),
      App(App(cons,c2), nil)))
  in print_string "\n2a" ;
  (* head (tail lst) *)
  assert (betaequal (App(head,App(tail,lst))) c1) ;
  print_string "\n2b" ;
  (* isnil (tail (tail (tail lst))) *)
  assert (betaequal (App(isnil,App(tail,App(tail,App(tail,lst))))) tru)
end);

]);;

(* ----- ------------- ----- *)
(* ----- Testy parsera ----- *)
(* ----- ------------- ----- *)
test ("Parsing", [

(fun () -> begin
  let str = "λx. x y"
  in let exp = Lam("x",App(Var "x", Var "y"))
  in let parsed = parse str
  in print_string "\n1: "; print_string (string_of_term parsed) ;
  assert (exp = parsed)
end);

(fun () -> begin
  let str = "x y z"
  in let exp = App(App(Var "x", Var "y"),Var "z")
  in let parsed = parse str
  in print_string "\n2: "; print_string (string_of_term parsed) ;
  assert (exp = parsed)
end);

(fun () -> begin
  let str = "x ( y z )"
  in let exp = App(Var "x", App(Var "y", Var "z"))
  in let parsed = parse str
  in print_string "\n3: "; print_string (string_of_term parsed) ;
  assert (exp = parsed)
end);

(fun () -> begin
  let str = "\\k. ( \\x. a b ) ( \\y. b c )"
  in let exp = Lam("k", App( Lam("x",App(Var "a",Var "b")),
                             Lam("y",App(Var "b",Var "c"))))
  in let parsed = parse str
  in print_string "\n4: "; print_string (string_of_term parsed) ;
  assert (exp = parsed)
end);

(fun () -> begin
  let str = "mul 2 5"
  in let parsed = parse str
  in print_string "\n5: "; print_string (string_of_term parsed) ;  
  assert (betaequal parsed (church_of_int 10))
end);

(fun () -> begin
  let parsed1 = parse "sub add 3 2 1"
  in let exp1 =
    App(App(sub,  
      App(App(add,church_of_int 3), church_of_int 2)),
        church_of_int 1)
  in let parsed2 = parse "mul 2 2" 
  in let exp2 =
    App(App(mul,church_of_int 2), church_of_int 2)
  in print_string "\n6: sub add 3 2 1 = mul 2 2";
  assert (exp1 = parsed1) ;
  assert (exp2 = parsed2) ;
  assert (betaequal parsed1 parsed2) ;
end);

(fun () -> begin
  let parsed1 = parse "cons 1 cons 2 cons 3 nil"
  in let exp1 = App(App(cons,church_of_int 1),
                  App(App(cons,church_of_int 2),
                    App(App(cons,church_of_int 3),nil)))
  in print_string "\n6: [1;2;3]";
  assert (exp1 = parsed1) ;
end);

(fun () -> begin
  let parsed = parse "head tail tail ( cons 1 cons 2 cons 3 nil )"
  in print_string "\n7:";
  assert (betaequal parsed (church_of_int 3)) ;
end);

(fun () -> begin
  (* if 2 = 1+1 then (x -> 2*x) else (x -> 7) zaaplikowane do 5 *)
  let parsed = parse "( add ( ( \\x. x ) 2 ) ( if false 5 10 ) )"
  in print_string "\n8:";
  assert (betaequal parsed (church_of_int 12)) ; 
end);

(fun () -> begin
  (* if 2 = 1+1 then (x -> 2*x) else (x -> 7) zaaplikowane do 5 *)
  let parsed = parse "( if eq 2 add 1 1 ( \\x. mul 2 x ) ( \\x. 7 ) ) 5"
  in print_string "\n9:";
  assert (betaequal parsed (church_of_int 10)) ; 
end);

(fun () -> begin
  (*
    [ if [(5 + (if 7=13 then 5 else mul 1 4)) = (λx. mul x x) (1+2) ]
         then ((λx. λy. 17-x-y) (0+1)) else (λx. true) ] 10
     NEXT IS: ( if  ( 5 + 4 = (λx. x^2)3 )  then  ((λx. λy. 17-x-y) 1)  else (λx. true) ) 10
         NEXT IS:  ((λx. λy. 17-x-y) 1) 10 => RESULT IS: 6
  *) 

  let parsed = parse "
    if eq add 5 if eq 7 13 5 mul 1 4 ( ( λx. mul x x ) add 1 2 )
      ( ( λx. λy. sub sub 17 x y ) add 0 1 )
         ( λx. true ) 
    10"

  in print_string "\n10:";
  assert (betaequal_cbv parsed (church_of_int 6)) ;
  
end);

]);;

;;