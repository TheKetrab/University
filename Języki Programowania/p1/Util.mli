(* Bartłomiej Grochowski 300951 *)
(* Util.mli *)

val index_of : 'a -> 'a list -> int
val append : 'a list -> 'a list -> 'a list
val var_name : int -> string
val for_each : 'a list -> ('a -> unit) -> unit
