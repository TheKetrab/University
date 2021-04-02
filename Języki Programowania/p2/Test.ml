(* Bartłomiej Grochowski 300951 *)
(* Test.ml *)

(* 
  Skrypt odpala wszystkie testy.
  Jeśli któryś się nie powiedzie, to rzucony jest OCamlowy wyjątek Assertion Failed.
  Można wyłączyć printowanie logu wszystkich testów: ptd = false
*)

#load "Lambda.cmo" ;;
open Lambda ;;

(* ------------------------------------ *)
let ptd = true ;; (* print test details *)
(* ------------------------------------ *)



let log name t = begin
  print_string ("\n\nTest '" ^ name ^ "' failed. Details:");
  run t
end ;;

(* ----- ----- test ----- ----- *)
let test
  (name : string)
  (t : term)                    (* term do testowania *)
    (te : bool)                 (* oczekiwany typeerror *)
    (res : (term * ty) option)  (* oczekiwany wynik funkcji eval *)
    (unex : bool)               (* oczekiwany niezłapany wyjątek *)
    
 : bool = 

  (* wypisz szczegóły testu *)
  if ptd then ( print_string ("\n" ^ name ^ ":"); run t; );

  match (te,res,unex) with
  
  | (true, None, false) -> (
      try
        (* jesli typ się obliczył, to nie ma typeerrora *)
        let _ = typeof newctx t in
          (log name t; false)
      with TypeError(_) -> true
  )
  | (false, Some(v,tyT), false) -> (
      try
        let (v1,tyT1) = eval t in
        (* wartość i typ się zgadzają? *)
        if (v = v1 && tyT = tyT1) then true
        else (log name t; false)
      with _ -> (log name t; false)
  )
  | (false, None, true) -> (
      try
        let (_,_) = eval t in
          log name t; false
      (* eval powinno wyrzucić unhandled exception *)
      with UnhandledException(_,_) -> true
       | _ -> log name t; false;
  )
  | (_,_,_) ->
      (run t; false;)
;;

(* ------------------------- *)
(* Ewaluacja zwykłych termów *)
(* ------------------------- *)
let normal_tests = [

(fun () ->
  (* [if true then 2+10 else 6] *)
  let t = If(True,Add(Num(2),Num(10)),Num(6))
  in test "normal_test1" t false (Some(Num(12),Nat)) false
);

(fun () ->
  (* [if true then 2+true else 6] *)
  let t = If(True,Add(Num(2),True),Num(6))
  in test "normal_test2" t true None false
);

(fun () ->
  (* [if iszero(7) then (2+1) else (1+8) ] * [if false then false else (3*3)] *)
  let t = Mul(
    If(IsZero(Num(7)), Add(Num(2),Num(1)), Add(Num(1),Num(8))),
    If(False,False,Mul(Num(3),Num(3))))
  in test "normal_test3" t true None false
);

(fun () ->
  (* (λ:Nat[x0+5] 7) - [if false then 5 else (λ:Bool[if x0 then 5 else 6] true)] *)
  let t = Sub(
    App(Lam(Nat,Add(Var(0),Num(5))),Num(7)),
    If(False,Num(5),App(
      Lam(Bool,If(Var(0),Num(5),Num(6))), True)))
  in test "normal_test4" t false (Some(Num(7),Nat)) false
);

(fun () ->
  (* 2+(λ:Bool[if x0 then 3 else 4]) *)
  let t = Add(Num(2), Lam(Bool,If(Var(0),Num(3),Num(4))))
  in test "normal_test5" t true None false
);

(fun () ->
  (* 2+((λ:Bool[if x0 then 3 else 4]) true) *)
  let t = Add(Num(2), App(Lam(Bool,If(Var(0),Num(3),Num(4))),True))
  in test "normal_test6" t false (Some(Num(5),Nat)) false
);

(fun () ->
  (* 2+((λ:Bool[if x0 then true else 4]) false)
    - wyrażenie poprawnie by się obliczyło, ale typechecker nie przepuszcza (typowanie statyczne) *)
  let t = Add(Num(2), App(Lam(Bool,If(Var(0),True,Num(4))),False))
  in test "normal_test7" t true None false
);

(fun () ->
  (* aplikacja do funkcji f 2 3, funkcji odejmującej *)
  (* (λ:(Nat→Nat→Nat)[x0 2 3]  λ:Nat[λ:Nat[x1-x0]])  *)
  let t = App(
    Lam(Arr(Nat,Arr(Nat,Nat)), App(App(Var(0),Num(2)),Num(3))),
    Lam(Nat,Lam(Nat,Sub(Var(1),Var(0)))))
  in test "normal_test8" t false (Some(Num(-1),Nat)) false;
)
] ;;

(* ----------------- *)
(* Termy z wyjątkami *)
(* ----------------- *)

let div = Lam(Nat,Lam(Nat,
  If(Equal(Var(0),Num(0)),
    Throw("arithexc",UnitVal,Nat),
    Div(Var(1),Var(0))))) ;;

let exception_tests = [

  (* testy typu: try (div 2 0) catch [...] *)

(fun () ->
  let t = Exception("arithexc",Unit,
    TryCatch(App(App(div,Num(2)),Num(0)),
      [("arithexc",Num(42))]))
  in test "exc_test1" t false (Some(Num(42),Nat)) false
);

  (* typy obsługi ti w catch mają zły typ *)

(fun () ->
  let t = Exception("arithexc",Unit,
    TryCatch(App(App(div,Num(2)),Num(0)),
      [("arithexc",True)]))
  in test "exc_test2" t true None false
);

(fun () ->
  let t = 
    Exception("arithexc",Unit,
    Exception("arithexc2",Unit,
    Exception("arithexc3",Unit,
    TryCatch(App(App(div,Num(2)),Num(0)),
      [("arithexc",Num(17));
      ("arithexc2",True);
      ("arithexc3",Num(42))
      ]))))
  in test "exc_test3" t true None false
);

  (* jednego z wyjątków nie ma z kontekście (nieznany wyjątek arithexc2) *)

(fun () ->
  let t =
    Exception("arithexc",Unit,
    Exception("arithexc1",Unit,
    TryCatch(App(App(div,Num(2)),Num(0)),
      [("arithexc",Num(17));
      ("arithexc1",Num(1));
      ("arithexc2",Num(42))
      ])))
  in test "exc_test4" t true None false
);

  (* różne typy obsługi, więc t może zwracać różne typy *)

(fun () ->
  let t =
    Exception("arithexc",Unit,
    Exception("arithexc1",Nat,
    TryCatch(App(App(div,Num(2)),Num(0)),
      [("arithexc",UnitVal);
      ("arithexc1",Num(1));
      ])))
  in test "exc_test5" t true None false
);

  (* wyrzucony arithexc zostaje złapany przez handler (unit -> num(10)) *)

(fun () ->
  let t =
    Exception("arithexc",Unit,
    Exception("arithexc1",Nat,
    TryCatch(App(App(div,Num(2)),Num(0)),
      [("arithexc",Lam(Unit,Num(10)));
      ("arithexc1",Num(20));
      ])))
  in test "exc_test6" t false (Some(Num(10),Nat)) false
);


  (* nazwa wyjątku w handlerze się zgadza, ale typ argumentu skojarzonego z wyjątkiem się nie zgadza
  (rzucony został dla Unit, a handler jest dla arithexc Nat - wyjątek będzie niezłapany *)

(fun () ->
  let t =
    Exception("arithexc",Unit,
    Exception("arithexc",Nat,
    Exception("arithexc1",Nat,
    TryCatch(App(App(div,Num(2)),Num(0)),
      [("arithexc",Lam(Nat,Num(10)));
      ("arithexc1",Num(20));
      ]))))
  in test "exc_test7" t false None true
);

  (* łapany jest pierwszy pasujący wyjątek  *)
(fun () ->
  let t =
    Exception("arithexc",Unit,
    Exception("arithexc1",Nat,
    TryCatch(App(App(div,Num(2)),Num(0)),
      [("arithexc1",Num(1));
      ("arithexc",Lam(Unit,Num(10)));
      ("arithexc",Num(20)); (* tutaj też by się złapało, ale złapie się w tym poprzednim catchu *)
      ])))
  in test "exc_test8" t false (Some(Num(10),Nat)) false
);

  (* zagnieżdżone wyjątki: *)

(fun () ->
  let t = (* złapanie wyjątku w środku *)
    Exception("insideexc",Bool,
    Exception("outsideexc",Bool,
    TryCatch(
      TryCatch(
        If(True,Throw("insideexc",True,Nat),Num(5)),
        [("insideexc",Num(10))]),
      ["insideexc",Num(20);
       "outsideexc",Num(30)])))
  in test "exc_test9" t false (Some(Num(10),Nat)) false
);

(fun () ->
  let t = (* niezłapanie w środku -> przepuszczenie -> złapanie na zewnątrz *)
    Exception("insideexc",Bool,
    Exception("outsideexc",Bool,
    TryCatch(
      TryCatch(
        If(True,Throw("outsideexc",True,Nat),Num(5)),
        [("insideexc",Num(10))]),
      ["insideexc",Num(20);
       "outsideexc",Num(30)])))
  in test "exc_test10" t false (Some(Num(30),Nat)) false
);

(fun () ->
  let t = (* złapanie wyjątku i rzucenie w środku nowego *)
    Exception("Aexc",Unit,
    Exception("Bexc",Unit,
    Exception("Cexc",Unit,
    TryCatch(
      TryCatch(
        If(True,Throw("Aexc",UnitVal,Nat),Num(5)),
        [("Aexc",Throw("Bexc",UnitVal,Nat))]),
      [("Bexc",Num(20));
       ("Cexc",Num(30))]))))
  in test "exc_test11" t false (Some(Num(20),Nat)) false
);

] ;;


(* ------------------- *)
(* Fix i fix + wyjątki *)
(* ------------------- *)

let g = Lam(Arr(Nat,Nat), Lam(Nat,
  If(
    Equal(Var(0),Num(0)),
    Num(1),
    Mul(Var(0),App(Var(1),Sub(Var(0),Num(1))))
  )
));;

let iseven =
  Lam(Arr(Nat,Bool), Lam(Nat,
  If(Greater(Var(0),Num(-1)), (* x >= 0*)
    If(
      Equal(Var(0),Num(0)),
      True,
      App(Var(1),Sub(Var(0),Num(2))) (* iseven(x-2) *)
    ),
    (* probably not, bo dla ujemnych parzystych też wyrzuci
       będziemy mogli przechwycić argument, i sprawdzić, czy zostało
       rzucone -1 - jesli tak, to na pewno nieparzyste, a jeśli nie,
       to rzucamy dalej probably_not *)
    Throw("probably_not",Var(0),Bool)
  )));;

let fib = 
  Lam(Arr(Nat,Nat), Lam(Nat,
    If(Greater(Num(0),Var(0)),
      Throw("less_than_zero_exc",Var(0),Nat),
      If(Equal(Var(0),Num(0)),Num(0),
        If(Equal(Var(0),Num(1)),Num(1),
          Add(
            App(Var(1), Sub(Var(0),Num(1))), (* f(n-1) *)
            App(Var(1), Sub(Var(0),Num(2)))  (* f(n-2) *)
          )
  )))));;

let fix_tests = [

(fun () ->
  let t = (* silnia - uwaga: dla liczb ujemnych się zapętla *)
    (App(Fix(g),Num(6)))
  in test "fix_test1" t false (Some(Num(720),Nat)) false
);

(fun () ->
  let t = 
    Exception("probably_not",Nat,
    (App(Fix(iseven),Num(4))))
  in test "fix_test2" t false (Some(True,Bool)) false
);

(fun () ->
  let t = (* łapiemy wyjątek, argumentem jest -1, czyli liczba nie była parzysta *)
    Exception("probably_not",Nat,
    TryCatch(
      (App(Fix(iseven),Num(17))),
      ["probably_not",Lam(Nat, 
          If(Equal(Var(0),Num(-1)),
            False,
            Throw("probably_not",Var(0),Bool)))
      ]
    ))
  in test "fix_test3" t false (Some(False,Bool)) false
);

(fun () ->
  let t = (* łapiemy wyjątek, argumentem jest -10, czyli dalej nie wiemy -> rzucamy znowu wyjątek *)
    Exception("probably_not",Nat,
    TryCatch(
      (App(Fix(iseven),Num(-10))),
      ["probably_not",Lam(Nat, 
          If(Equal(Var(0),Num(-1)),
            False,
            Throw("probably_not",Var(0),Bool)))
      ]
    ))
  in test "fix_test4" t false None true
);

(fun () ->
  let t =
    Exception("less_than_zero_exc",Nat,
    (App(Fix(fib),Num(10))))
  in test "fix_test5" t false (Some(Num(55),Nat)) false
);

(fun () ->
  let t =
    Exception("less_than_zero_exc",Nat,
    (App(Fix(fib),Num(-50))))
  in test "fix_test6" t false None true
);

] ;;


(* ------------------- *)
(* Uruchomienie testów *)
(* ------------------- *)

List.iter (fun x -> assert (x() = true)) normal_tests ;;
List.iter (fun x -> assert (x() = true)) exception_tests ;;
List.iter (fun x -> assert (x() = true)) fix_tests ;;

