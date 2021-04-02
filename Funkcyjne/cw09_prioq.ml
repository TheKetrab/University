
module type ORDERED = sig
	type t
	type comparison = LT | EQ | GT
	val compare : t -> t -> comparison
end

module type PQUEUE = sig
	type priority
	type 'a t
	exception EmptyPQueue
	val empty : 'a t
	val insert : 'a t -> priority -> 'a -> 'a t
	val remove : 'a t -> priority * 'a * 'a t
end

(* cos brakuje... *)
module PQList = functor (OrdType : ORDERED) -> (struct

	type priority = int
	type 'a t = OrdType.t list
	exception EmptyPQueue
	
	let empty = []
	
	let rec insert pq p c =
		match pq with
		| [] -> [(p,c)]
		| (prio,item)::tail ->
			if (OrdType.compare prio p = OrdType.GT)
			then (prio,item)::(insert tail p c)
			else (p,c)::pq
			
	let remove pq =
		match pq with
		| [] -> raise EmptyPQueue
		| (prio,item)::tail -> (prio, item, tail)
		
		
end : PQUEUE);;
