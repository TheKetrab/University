(* Bartłomiej Grochowski 300951 *)
(* Lambda.ml *)

type term = 
  | Var of string
  | Lam of string * term
  | App of term * term

exception ConversionFailed

val string_of_term : term -> string
val print_term : term -> unit
val int_of_church : term -> int
val church_of_int : int -> term

val c0 : term
val c1 : term
val c2 : term
val c3 : term
val c4 : term

(* Logic *)
val tru : term
val fls : term
val iF : term
val iszro : term
val aNd : term
val oR : term
val not : term

(* Pair *)
val pair : term
val fst : term
val snd : term

(* Arith *)
val scc : term
val prd : term
val add : term
val sub : term
val mul : term
val pow : term
val eq : term

(* List *)
val nil : term
val cons : term
val head : term
val tail : term
val isnil : term

(* Recursion *)
val y : term
val y' : term
val fix : term