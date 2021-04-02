(* Bartłomiej Grochowski 300951 *)
(* DeBruijn.mli *)

open Lambda

type bruijn =
  | BrVar of int
  | BrLam of bruijn
  | BrApp of bruijn * bruijn

type ctx = string list

exception InsufficientContext
exception NoNormalForm

val bruijn_of_term : term -> bruijn * ctx
val bruijn_of_term_ctx : term -> ctx -> ctx -> bruijn * ctx
val term_of_bruijn : bruijn -> term
val term_of_bruijn_ctx : bruijn -> ctx -> term
val print_bruijn : bruijn -> unit
val string_of_bruijn : bruijn -> string
val reduce_term_to_bruijn : (bruijn -> bruijn) -> term  -> bruijn

val shift : int -> bruijn -> bruijn
val substitution : int -> bruijn -> bruijn -> bruijn
val cbn_natural_eval : bruijn -> bruijn
val cbv_natural_eval : bruijn -> bruijn

val betaequal : term -> term -> bool
val betaequal_cbv : term -> term -> bool
val reduce : term -> term
val reduce_cbv : term -> term
