(* Bartłomiej Grochowski 300951 *)
(* DeBruijn.ml *)

(*
    Obliczenia są prowadzone po strukturze termu przy dwóch kontekstach:
      a) zmienne wolne: fv (free variables)
      b) zmienne związane: bv (bound variables)
    
    Przy czym związanie zmiennej obowiązuje tylko 'w środku' lambd
    dlatego wywołanie rekurencyjne funkcje z dołącza zmienną do bv,
    a po wyjściu z funkcji ta zmienna nie będzie już w bv.
    Z kolei dla zmiennych wolnych musimy przetrzymywać 'słownik',
    więc każde obliczenie zwraca również słownik, ponieważ mógł ulec zmianie.
*)

open Util
open Lambda

type bruijn =
  | BrVar of int
  | BrLam of bruijn
  | BrApp of bruijn * bruijn

type ctx = string list

exception InsufficientContext
exception NoNormalForm

let rec bruijn_of_term_ctx (t : term) (bv : ctx) (fv : ctx) : bruijn * ctx  =
  match t with
  | Lam (x, t') ->
      let (b', fv') = bruijn_of_term_ctx t' (x::bv) fv in
      (BrLam b', fv')
  | App (t1, t2) -> 
      let (b1, fv') = bruijn_of_term_ctx t1 bv fv in
      let (b2, fv'') = bruijn_of_term_ctx t2 bv fv' in
      (BrApp(b1,b2), fv'')
  | Var x ->
      (* when x in bv *)  
      let lookup_bv = index_of x bv in
      if lookup_bv >= 0 then 
      (BrVar(lookup_bv), fv) else
      (* when x in fv *)
      let bv_len = List.length bv in
      let lookup_fv = index_of x fv in
      if lookup_fv >= 0 then
      (BrVar(lookup_fv + bv_len), fv) else
      (* when x neither in bv nor in fv *)
      let fv_len = List.length fv in
      (BrVar(bv_len + fv_len), append fv [x])

let bruijn_of_term t =
  bruijn_of_term_ctx t [] []


(* 
    Funkcja aux bierze term De Bruijna, kontekst zmiennych związanych i liczbę k,
    która reprezentuje najmniejszą zmienną, która nie jest jeszcze związana
    i nie koliduje z żadną z nazw zmiennych wolnych w kontekście fv.
    Jeśli okaże się, że wygenerowana zmienna już znajduje się w kontekście fv,
    to obliczenie jest ponawiane z k zwiększonym o 1.
*)
let term_of_bruijn_ctx (b : bruijn) (fv : ctx) : term =
  let fv_len = List.length fv in
  let rec aux b bv k =
    let v = var_name k in
    if (index_of v fv) <> -1 then
    aux b bv (k+1) else
      match b with
      | BrLam b' ->
          Lam (v, aux b' (v::bv) (k+1))
      | BrApp (b1, b2) ->
          let t1 = aux b1 bv k in
          let t2 = aux b2 bv k in
          App (t1,t2)
      | BrVar i ->
          (* when i in bv *)  
          let bv_len = List.length bv in
          if (i < bv_len) then
          Var(List.nth bv i) else
          (* when i in fv *)
          let i' = i - bv_len in
          if (i' < fv_len) then
          Var(List.nth fv i') else
          (* else unable to construct variable *)
          raise InsufficientContext
  in aux b [] 0

let term_of_bruijn b =
  term_of_bruijn_ctx b []

let rec string_of_bruijn (b : bruijn) : string =
  match b with
  | BrVar i ->
      string_of_int i
  | BrLam b' ->
      Printf.sprintf "λ[%s]" (string_of_bruijn b')
  | BrApp (b1, b2) ->
      Printf.sprintf "(%s %s)" (string_of_bruijn b1) (string_of_bruijn b2)

let rec print_bruijn (b : bruijn) =
  print_string (string_of_bruijn b)

let reduce_term_to_bruijn (eval : bruijn -> bruijn) (t : term) : bruijn =
  let (br, fv) = bruijn_of_term t
  in eval br


(*
    Funkcja shift inkrementuje indeksy zmiennych, ale tylko tych wolnych. 
    Które indeksy reprezentują zmienne wolne? Te, które mają większy bądź równy numer
    ilości lambd, w których są zagnieżdżone. W aux za 'głębokość w lambdach'
    odpowiada argument c (ang. cutoff) . Parametr d, mówi o ile chcemy przesunąć.
*)
let shift (d : int) (b : bruijn) : bruijn =
  let rec aux c t =
    match t with
    | BrVar(i) ->
        if i >= c then BrVar(i+d) else BrVar(i)
    | BrLam(b') ->
        BrLam(aux (c+1) b')
    | BrApp(b1,b2) ->
        BrApp(aux c b1, aux c b2)
  in aux 0 b

(* [j -> b']b  albo  [b'/j]b *)
let substitution (j : int) (b' : bruijn) (b : bruijn) : bruijn =
  let rec aux c b =
    match b with
    | BrVar(i) ->
        if i = j+c then shift c b'
        else BrVar(i)
    | BrLam(b1) ->
        BrLam(aux (c+1) b1)
    | BrApp(b1,b2) ->
        BrApp(aux c b1, aux c b2)
  in aux 0 b

let subst_top s t =
  shift (-1) (substitution 0 (shift 1 s) t)

(*
    Semantyka naturalna w porządku normalnym (Call By Name)

    Chcemy obliczać wyrażenia (tj. aplikować je do siebie), dlatego:
      a) jeśli mamy zmienną, to nie da się tego uprościć
      b) jesli mamy lambdę, to być może w ciele da się coś uprościć
      c) jeśli mamy aplikację, to korzystamy z reguł:
      
                                e1 ⇓ λx. e     [x → e2]e ⇓ v
          (I)  v ⇓ v ,    (II)  ----------------------------
                                        e1 e2 ⇓ v

         przy czym jeśli e1 nie jest lambdą, to będziemy redukować e2.

    Nie każdy term ma postać normalną (np. (λx. x x)(λx. x x)), dlatego
    będziemy kontrolować, czy się nie zapętlamy - będziemy ciągnąć ze sobą
    listę 'history', tj. listę termów, na których się już wywołaliśmy.
    Jeśli 'index_of b history' jest różny od -1, to ten term już jest
    w historii, czyli się zapętliliśmy -> term nie ma postaci normalnej.

    --- --- --- --- ---
    Ponieważ strategia CBN duplikuje obliczenia, być może chcemy szybciej
    obliczać wartość termów - dlatego dostępna jest również metoda obliczania
    Call By Value: cbv_natural_eval - UWAGA: obliczenia mogą się zapętlać!
    (np. (λx. y)Ω → (λx. y)Ω → ...) (Ω = (λx. x x)(λx. x x))

*)

let cbn_natural_eval (b : bruijn) : bruijn =
  let rec aux b history =
    if index_of b history <> -1
      then raise NoNormalForm else
    match b with
    | BrVar( i ) -> BrVar( i )
    | BrLam( b' ) -> BrLam( aux b' (b :: history) )
    | BrApp( b1 , b2 ) -> 
        let b1' = aux b1 (b :: history) in 
        (match b1' with
        | BrLam( b1'' ) -> 
            aux ( subst_top b2 b1'' ) (b :: history)
        | _ -> 
            BrApp( b1' , aux b2 (b :: history) ))
  in aux b []

(* tutaj ręcznie ograniczamy głębokość rekursji przez 1000 *)
let cbv_natural_eval (b : bruijn) : bruijn =
  let rec aux b x =
    if x > 1000 then raise NoNormalForm else
    match b with
    | BrVar( i ) -> BrVar( i )
    | BrLam( b' ) -> BrLam( aux b' (x+1) )
    | BrApp( b1 , b2 ) ->
        let b1', v2 = aux b1 (x+1), aux b2 (x+1) in
        (match b1' with
         | BrLam( b1'' ) ->
             aux (subst_top v2 b1'') (x+1)
         | _ ->
             BrApp( b1' , v2 ))
  in aux b 0

(* 
    Sprawdzanie równoważności termów.

    Procedura 'betaequal' sprawdza równoważność dwóch termów.
    Najpierw sprowadza oba termy do nameless form - indeksy De Bruijna.
    Jeśli w tych termach są jakieś zmienne wolne, to zapamiętujemy to.

    Termy w postaci De Bruijna redukujemy (domyślnie strategią Call By Name,
    ale ponieważ takie obliczenia długo trwają, istnieje możliwość redukcji
    używając Call By Value: betaequal_cbv t1 t2

    Później musimy porównać termy w postaci De Bruijna.
    Jeśli mają taką samą strukturę i były zamknięte to wejściowe termy były równoważne.
    Jeśli mają taką samą strukturę, ale były otwarte, to musimy sprawdzić,
    czy indeksy De Bruijna odpowiają tym samym zmiennym w kontekstach.

    Np. (λx. x) y oraz (λx. x) z redukują się odpowiednio do y i z.
    Ich reprezentacje w postaci De Bruijna to (λ[0] 0) → 0,
    ale reprezentują RÓŻNE termy (w swoich kontekstach).
    Tym porównaniem zajmuje się procedura nf_open_eq (normal form open term equal),
    która porównuje dwa termy w postaci normalnej i sprawdza, czy zmienne
    wolne z termów są sobie równe (nawet jeśli mają różne indeksy De Bruijna).

*)

(* ta procedura działa dobrze tylko na termach w postaci normalnej! *)
(* val nf_open_eq : bruijn * string list -> bruijn * string list -> bool *)
let nf_open_eq (b1, fv1) (b2, fv2) : bool =
  let rec aux b1 b2 d =
    match b1, b2 with
    | BrVar(i), BrVar(j) ->
        (if (i < d && j < d) then i = j
          else try List.nth fv1 (i - d) = List.nth fv2 (j - d)
            with e -> false)
    | BrLam(b1'), BrLam(b2') ->
        aux b1' b2' (d+1)
    | BrApp(b11,b12), BrApp(b21,b22) ->
        aux b11 b21 d && aux b12 b22 d
    | _ ->
        false
  in aux b1 b2 0


(* BETAEQUAL T1 T2 *)
let betaequal_strategy (eval : bruijn -> bruijn) (t1 : term) (t2 : term) : bool =
  let (b1,fv1) = bruijn_of_term t1 in
  let (b2,fv2) = bruijn_of_term t2 in
  let b1' = eval b1 in
  let b2' = eval b2 in
  nf_open_eq (b1', fv1) (b2', fv2)

let betaequal (t1 : term) (t2 : term) : bool =
  betaequal_strategy cbn_natural_eval t1 t2

let betaequal_cbv (t1 : term) (t2 : term) : bool =
  betaequal_strategy cbv_natural_eval t1 t2

(* REDUCE T *)
let reduce_strategy (eval : bruijn -> bruijn) (t : term) : term =
  let (b,fv) = bruijn_of_term t in 
    let b' = eval b in
      term_of_bruijn_ctx b' fv

let reduce (t : term) : term =
  reduce_strategy cbn_natural_eval t

let reduce_cbv (t : term) : term =
  reduce_strategy cbv_natural_eval t
  

