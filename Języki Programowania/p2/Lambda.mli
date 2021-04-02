(* Bartłomiej Grochowski 300951 *)
(* Lambda.mli *)

type ty =
  | Nat
  | Bool
  | Unit
  | Arr of ty * ty

type term = 
  | True
  | False
  | Num of int
  | Add of term * term
  | Sub of term * term
  | Mul of term * term
  | Div of term * term
  | UnitVal
  | Equal of term * term
  | Greater of term * term
  | IsZero of term
  | If of term * term * term
  | Var of int
  | Lam of ty * term
  | App of term * term
  | Fix of term
  | Exception of string * ty * term
  | Throw of string * term * ty
  | TryCatch of term * ((string * term) list)

exception TypeError of string
exception EvalException
exception UnhandledException of string * term

type gamma = ty list
type delta = (string * ty) list
type context = gamma * delta

type estate =
  | Val of term
  | Err of string * term

val newctx : gamma * delta

val err_of_estate : estate -> string * term
val val_of_estate : estate -> term
val iserr : estate -> bool

val add_binding : context -> ty -> context
val get_type : context -> int -> ty
val add_exn : context -> string -> ty -> context
val exception_in_ctx : context -> string -> ty -> bool

val string_of_type : ty -> string
val string_of_term : term -> string

val typeof : context -> term -> ty
val eval : term -> term * ty
val run : term -> unit
