#load "streams.cmo";;


(* zad2 *)
type 'a stream = Nil | Cons of 'a * 'a stream Lazy.t;;

let rec map f s =
	match s with
	| Nil -> Nil
	| Cons (x, xs) -> Cons (f x, lazy (map f (Lazy.force xs)));;

let rec countdown n =
	Cons(n, lazy (countdown (n-1)));;

let rec cutoff n xs =
	match n, xs with
	| 0, _ -> []
	| _, Nil -> []
	| n, (Cons(x,xs)) ->
		x::(cutoff (n-1) (Lazy.force xs));;

cutoff 5 (map (fun x -> 1. /. (float_of_int x)) (countdown 4))		
		
		
		
		
		
		
		
		
		
		
(* zad4 DONE *)
let rec cyclist lst =
	if (lst = [])
	then raise (Invalid_argument "empty list")
	else
		let rec f first xs =
			match xs with
			| [] -> first
			| x::xs when (first = lazy Streams.Nil) ->
			(* jesli niezainicjowane to: *)
				let rec fst = lazy (Streams.Cons (x, f fst xs))
				in fst
			| x::xs ->
				lazy (Streams.Cons (x, f first xs))
				
		in f (lazy Streams.Nil) lst;;
		
Streams.list_of_stream (Streams.take 10 k);;

(* zad5 DONE *)
type ’a two_way_stream = ’a twcell Lazy.t
and ’a twcell = TWCons of ’a two_way_stream * ’a * ’a two_way_stream;;

let tws_next tws =
	match tws with
	| lazy (TWCons (prev,a,next)) -> next;;

let tws_prev tws =
	match tws with
	| lazy (TWCons (prev,a,next)) -> prev;;

let tws_elem tws =
	match tws with
	| lazy (TWCons (prev,a,next)) -> a;;


