(* Bartłomiej Grochowski 300951 *)
(* Parser.ml *)

(*
    Język:

    e ::= x | λx. e | e e |
          n | add e e | mul e e | sub e e | eq e e |
          true | false | if e e e |
          fix e |
          pair e e | fst e | snd e |
          nil | cons e e | head e | tail e | isnil e |
          ( e )

    Przy czym:
      a) aplikacja łączy w lewo: e1 e2 e3 = ((e1 e2) e3)
      b) abstrakcja łączy w prawo: λx. λy. λz. e = λx. (λy. (λz. e))
      c) słowa kluczowe łączą 'najmocniej' szukając swoich argumentów:
           x add 1 mul 2 3 y = x (add 1 (mul 2 3)) y = (x (add 1 (mul 2 3))) y
    
    Możliwe jest również używanie explicite nawiasów, przy czym należy pamiętać,
    żeby były oddzielone spacjami, ponieważ proces tokenizacji to wyłącznie
    oddzielenie znaków przez białe znaki. Przykład:
      a) (λx. x x)(λx. x x)      - ŹLE
      b) ( λx. x x ) ( λx. x x ) - DOBRZE

    UWAGA na zapis abstrakcji:
      a) λx.e   - ŹLE (brak spacji)
      b) λx e   - ŹLE (brak kropki)
      c) λx. e  - DOBRZE
      d) \x. e  - DOBRZE (w pliku)
      e) \\x. e - DOBRZE (w stringu)

*)

open Lambda

type arity = Arity0 | Arity1 | Arity2 | Arity3
exception EndOfExpr

(*
  Słownik słów kluczowych: nazwa * odpowiadająca lambda * arność
  w module 'Lambda' zaimplementowane jest jeszcze kilka funkcji,
  których można używać budując wyrażenia jako drzewa abtrakcyjne,
  ale nie zostały dodane do składni konkretnej języka. Są to:
    iszro, aNd, oR, not, scc, prd, pow
  Np. App(iszro,App(prd,church_of_int 1))
*)
let keywords = [
  ( "add"   , add   , Arity2 ) ;
  ( "mul"   , mul   , Arity2 ) ;
  ( "sub"   , sub   , Arity2 ) ;
  ( "eq"    , eq    , Arity2 ) ;
  ( "true"  , tru   , Arity0 ) ;
  ( "false" , fls   , Arity0 ) ;
  ( "if"    , iF    , Arity3 ) ;
  ( "fix"   , y     , Arity1 ) ;
  ( "pair"  , pair  , Arity2 ) ;
  ( "fst"   , fst   , Arity1 ) ;
  ( "snd"   , snd   , Arity1 ) ;
  ( "nil"   , nil   , Arity0 ) ;
  ( "cons"  , cons  , Arity2 ) ;
  ( "head"  , head  , Arity1 ) ;
  ( "tail"  , tail  , Arity1 ) ;
  ( "isnil" , isnil , Arity1 ) ;
]

let split_by_whitespace (s : string) : string list =
  Str.split (Str.regexp "[ \n\r\x0c\t]+") s

(* alternatywna implementacja - TYLKO JEŚLI str.cma nie zadziała!
let split_by_whitespace (s : string) : string list =
  let len = String.length s in
  let rec iter i word acc =    
    if i >= len
    then
      if word <> ""
        then List.rev (word :: acc)
        else List.rev acc
    else
      let c = String.get s i in
      if (c = ' ' || c = '\n' || c = '\r' || c = '\t')
      then
        if word <> ""
          then iter (i+1) "" (word :: acc)
          else iter (i+1) "" acc
      else
        iter (i+1) (word ^ (String.make 1 c)) acc
  in iter 0 "" []
*)

(*
    Parsowanie

    Używając poniżej zdefiniowanych procedur pomocniczych,
    lista tokenów jest parsowana wykorzystując read_multi_expr
    oraz read_single_expr. Każda z tych procedur zwraca parę
    term * string list, tj. zbudowane drzewo reprezentujące term
    oraz kolejną część listy tokenów do dalszego parsowania.

    Należy zauważyć, że język nie pozwala nam na budowanie
    funkcji f(x) = x+2 w sposób f ::= 'add 2', ponieważ 'add'
    jest słowem kluczowym i język NARZUCA, że MUSI mieć dwa argumenty.

    W celu zbudowaniu takiej funkcji musielibyśmy napisać to 'ręcznie':
      f ::= 'λx. add x 2'

*)

let is_number (s : string) : bool =
  try int_of_string s |> ignore; true
  with e -> false

let is_keyword (s : string) : bool =
  List.exists (fun (kw,kw_term,ar) -> kw = s) keywords

let get_keyword (s : string) =
  List.find (fun (kw,kw_term,ar) -> kw = s) keywords

let is_lambda (s : string) : bool =
  try let c0, c1 = String.get s 0, String.get s 1 in
    c0 = '\\' || (c0 = '\206' && c1 = '\187') (* \ or λ *)
  with e -> false (* index out of bounds *)

(* 'λabc.' -> 'abc' *)
let get_lambda_var (s : string) =
  let len = String.length s in
  match String.get s 0 with
  | '\\' -> String.sub s 1 (len-2)
  | '\206' -> String.sub s 2 (len-3)
  | _ -> assert(false)

(* Procedura czyta kilka wyrażeń i buduje z nich aplikacje.
   Np: e1 e2 e3 ... en -> App( ... , App(App(e1,e2),e3) , ... , en) *)
let rec read_multi_expr (lst : string list) : term * string list =
  let rec aux (lst : string list) (acc : term option) : term * string list =
    match lst, acc with
    | [], None -> raise EndOfExpr
    | [], Some(t) -> t, []
    | ")" :: tl, None -> raise EndOfExpr
    | ")" :: tl, Some(t) -> t, lst
    | hd :: tl, None -> 
        (match read_single_expr lst with
         | e1, [] -> e1, []
         | e1, ")" :: tl -> e1, ")" :: tl
         | e1, lst1 ->
             let (e2, lst2) = read_single_expr lst1 in
               aux lst2 (Some (App(e1,e2))))
    | hd :: tl, Some(t) ->
        let (e3,lst3) = read_single_expr lst in
          aux lst3 (Some (App(t,e3)))
  in aux lst None

(* Procedura ma przeczytać JEDNO wyrażenie. Zgodnie z gramatyką języka
   jest to: liczba, abstrakcja, słowo kluczowe (funkcja) albo wyr. w nawiasie. *)
and read_single_expr (lst : string list) : term * string list =
  match lst with
  (* end *)
  | [] ->
      raise EndOfExpr
  | hd :: tl when hd = ")" ->
      raise EndOfExpr
  (* new multi_expr *)
  | hd :: tl when hd = "(" ->
      let (e,lst) = read_multi_expr tl in
      (match lst with
       | ")" :: tl -> (e,tl)
       | _ -> raise EndOfExpr)
  (* number *)
  | hd :: tl when is_number hd ->
      let n = int_of_string hd in
      (church_of_int n, tl)
  (* lambda *)
  | hd :: tl when is_lambda hd ->
      let var = get_lambda_var hd in
      let (e,tl') = read_multi_expr tl in
        (Lam(var,e),tl')
  (* keyword *)
  | hd :: tl when is_keyword hd ->
      let (kw,kw_term,ar) = get_keyword hd in
      (match ar with
      | Arity0 ->
          (kw_term , tl)
      | Arity1 ->
          let (e1,tl') = read_single_expr tl in
            (App(kw_term,e1) , tl')
      | Arity2 ->
          let (e1, tl') = read_single_expr tl in
          let (e2, tl'') = read_single_expr tl' in
            (App(App(kw_term,e1),e2) , tl'')
      | Arity3 ->
          let (e1, tl') = read_single_expr tl in
          let (e2, tl'') = read_single_expr tl' in
          let (e3, tl''') = read_single_expr tl'' in
            (App(App(App(kw_term,e1),e2),e3) , tl'''))
  (* variable *)
  | hd :: tl ->
      Var(hd), tl

let parse (s : string) : term =
  let lst = split_by_whitespace s in
  match read_multi_expr lst with
  | term, lst -> term

(* term -> string: Ta procedura jest bardzo podobna do string_of_term,
   ale przerabia numerały Churcha na liczby i zachowuje odstępy między tokenami
   (przede wszystkim nawiasami), tak jak przewiduje gramatyka naszego języka. *)
let concrete_of_abstract (t : term) : string =
  let rec aux t =
    match t with
    | Var(str) -> str
    | Lam(str,t') -> (try string_of_int ( int_of_church t )
        with ConversionFailed -> Printf.sprintf "λ%s. ( %s )" str (aux t'))
    | App(t1,t2) ->
        Printf.sprintf "( %s %s )" (aux t1) (aux t2)
  in aux t
