

(* zad 1.1 DONE *)

module type PQUEUE = sig
	type priority
	type 'a t
	exception EmptyPQueue
	val empty : 'a t
	val insert : 'a t -> priority -> 'a -> 'a t
	val remove : 'a t -> priority * 'a * 'a t
end

module PQueue : PQUEUE with type priority = int = struct

	type priority = int
	type 'a t = Empty | PQueue of priority * 'a * 'a t
	exception EmptyPQueue
	
	let empty = Empty
	
	let rec insert pq p c =
		match pq with
		| Empty -> PQueue(p,c,Empty)
		| PQueue(prio,item,tail) ->
			if (prio > p) then PQueue(prio,item,(insert tail p c))
			else PQueue(p,c,pq)
			
	let remove pq =
		match pq with
		| Empty -> raise EmptyPQueue
		| PQueue(prio,item,tail) -> (prio, item, tail)
		
		
end;;


(* zad 1.2 DONE *)

let sort (xs : int list) =

	let rec enqueue (acc : int PQueue.t) (xs : int list) =
		match xs with
		| [] -> acc
		| x::xs -> enqueue (PQueue.insert acc x x) xs
	in
	
	let rec dequeue (acc : int list) (q : int PQueue.t) =
	
		if (q = PQueue.empty) then acc
		else let top = PQueue.remove q in
			match top with
			| (prio,item,tail) ->
				dequeue (item::acc) tail
	in	
		
	dequeue [] (enqueue PQueue.empty xs);;
	

(* zad 1.3 ~DONE *)

module type ORDTYPE =
sig
	type t
	type comparison = LT | EQ | GT
	val compare : t -> t -> comparison
end


module PQueueWithOrd = functor (OrdType : ORDTYPE) -> (struct

	type priority = OrdType.t
	type 'a t = Empty | PQueue of priority * 'a * 'a t
	exception EmptyPQueue
	
	let empty = Empty
	
	let rec insert pq p c =
		match pq with
		| Empty -> PQueue(p,c,Empty)
		| PQueue(prio,item,tail) ->
			if (OrdType.compare prio p = OrdType.GT)
			then PQueue(prio,item,(insert tail p c))
			else PQueue(p,c,pq)
			
	let remove pq =
		match pq with
		| Empty -> raise EmptyPQueue
		| PQueue(prio,item,tail) -> (prio, item, tail)
		
		
end : PQUEUE);;




module OrdString : ORDTYPE with type t = string = struct

	type t = string
	type comparison = LT | EQ | GT
	
	let compare s1 s2 =
		let n1 = String.length s1 in
		let n2 = String.length s2 in
		if (n1 > n2) then GT
		else if (n1 < n2) then LT
		else EQ	

end



module PQueueString = PQueueWithOrd(OrdString);;
	(PQueueString.empty : string PQueueString.t)

let sortString xs =

	let rec enqueue (acc : string PQueueString.t) (xs : string list) =
		match xs with
		| [] -> acc
		| x::xs -> enqueue (PQueueString.insert acc (x : OrdType.t) x) xs
	in
	
	let rec dequeue (acc : string list) (q : string PQueueString.t) =
	
		if (q = PQueueString.empty) then acc
		else let top = PQueueString.remove q in
			match top with
			| (prio,item,tail) ->
				dequeue (item::acc) tail
	in	
		
	dequeue [] (enqueue PQueueString.empty xs);;




(* zad 1.4 TODO	*)
	
	
	
(*	zad 2.1 DONE *)

module type VERTEX = sig
	type t
	type label
	val equal : t -> t -> bool
	val create : label -> t
	val label : t -> label
end

module type EDGE = sig
	type t
	type vertex
	val equal : t -> t -> bool
	val create : V.t -> V.t -> t
	val first : t -> V.t
	val second : t -> V.t
end


(* zad 2.2 DONE *)

module V : VERTEX with type label = string = struct

	type t = V of string
	type label = string
	
	let equal v1 v2 =
		match v1, v2 with
		| V(l1), V(l2) ->
			l1 = l2
		
	let create label =
		V(label)
		
	let label v =
		match v with
		| V(l) -> l
	
end

module E : EDGE with type vertex = V.t = struct

	type t = E of V.t * V.t
	type vertex = V.t
	
	let equal e1 e2 =
		match e1, e2 with
		| E(v1,v2), E(v1',v2') ->
			((v1 = v1') && (v2 = v2'))
		
	let create v1 v2 =
		E(v1,v2)
		
	let first e =
		match e with
		| E(v1,v2) -> v1

	let second e =
		match e with
		| E(v1,v2) -> v2
		
end
	
	

(* zad 2.3 *)

module type GRAPH = sig

	(* typ reprezentacji grafu *)
	type t

	module V : VERTEX
	type vertex = V.t

	module E : EDGE with type vertex = vertex
	type edge = E.t

	
	(* funkcje wyszukiwania *)
	val mem_v : t -> vertex -> bool
	val mem_e : t -> edge -> bool
	val mem_e_v : t -> vertex -> vertex -> bool
	val find_e : t -> vertex -> vertex -> edge
	val succ : t -> vertex -> vertex list 
	val pred : t -> vertex -> vertex list 
	val succ_e : t -> vertex -> edge list
	val pred_e : t -> vertex -> edge list 

	(* funkcje modyfikacji *)
	val empty : t
	val add_e : t -> edge -> t
	val add_v : t -> vertex -> t
	val rem_e : t -> edge -> t
	val rem_v : t -> vertex -> t

	(* iteratory *)
	val fold_v : ( vertex -> 'a -> 'a) -> t -> 'a -> 'a
	val fold_e : ( edge -> 'a -> 'a) -> t -> 'a -> 'a
	
	(* moja *)
	val create : vertex list -> edge list -> t 

end

module Graph : GRAPH with module V = V and module E = E  = struct

	type t = V.t list * E.t list
	module V = V
	type vertex = V.t
	module E = E
	type edge = E.t

	let mem_v (g : t) (v : V.t) =
		let vList, eList = g in
		List.mem v vList
			
	let mem_e (g : t) (e : E.t) =
		let vList, eList = g in
		List.mem e eList
		
	let mem_e_v (g : t) (v1 : V.t) (v2 : V.t) =
		let vList, eList = g in
		List.mem (E.create v1 v2) eList
			
	let find_e (g : t) (v1 : V.t) (v2 : V.t) =
		let vList, eList = g in
		List.find (fun x -> (E.equal x (E.create v1 v2))) eList

	let succ (g : t) (v : V.t) =
		let vList, eList = g in
		let rec aux lst =
			match (lst : E.t list) with
			| [] -> ([] : V.t list)
			| hd::(tl : E.t list) ->
				if (E.first hd = v)
				then (E.second hd)::(aux tl)
				else (aux tl)
		in aux eList


	let pred (g : t) (v : V.t) =
		let vList, eList = g in
		let rec aux lst =
			match (lst : E.t list) with
			| [] -> ([] : V.t list)
			| hd::(tl : E.t list) ->
				if (E.second hd = v)
				then (E.first hd)::(aux tl)
				else (aux tl)
		in aux eList

	let succ_e (g : t) (v : V.t) =
		let vList, eList = g in
		let rec aux lst =
			match (lst : E.t list) with
			| [] -> ([] : E.t list)
			| hd::(tl : E.t list) ->
				if (E.first hd = v)
				then hd::(aux tl)
				else (aux tl)
		in aux eList
		



	let pred_e (g : t) (v : V.t) =
		let vList, eList = g in
		let rec aux lst =
			match (lst : E.t list) with
			| [] -> ([] : E.t list)
			| hd::(tl : E.t list) ->
				if (E.second hd = v)
				then hd::(aux tl)
				else (aux tl)
		in aux eList
		

	let empty = [],[]
	
	let add_e (g : t) (e : E.t) =
		let vList, eList = g in
		(vList,e::eList)
	
	let add_v (g : t) (v : V.t) =
		let vList, eList = g in
		(v::vList,eList)

	let rem_e (g : t) (e : E.t) =
		let vList, eList = g in
		(vList , List.filter (fun x -> x<>e) eList)
		
	let rem_v (g : t) (v : V.t) =
		let vList, eList = g in
		(List.filter (fun x -> x<>v) vList , eList)
		

	let fold_v f (g : t) acc =
		let vList, eList = g in
		List.fold_right f vList acc 
	
	let fold_e f (g : t) acc =
		let vList, eList = g in
		List.fold_right f eList acc 
		
	(* dodatkowa funkcja *)
	let create (vList : V.t list) (eList : E.t list) : t =
		(vList, eList)
	
	
end

(* zad 2.4 - testy DONE *)

(* przykladowy graf
	 B
   / ^
  V  |
A -> C -> E
	 V
	 D
*)

let v1 = V.create "A"
let v2 = V.create "B"
let v3 = V.create "C"
let v4 = V.create "D"
let v5 = V.create "E"

let e1 = E.create v1 v3
let e2 = E.create v3 v2
let e3 = E.create v2 v1
let e4 = E.create v3 v4
let e5 = E.create v3 v5

let g1 = Graph.create [v1;v2;v3;v4;v5] [e1;e2;e3;e4;e5]
Graph.mem_v g1 v1
Graph.mem_v g1 (V.create "xxx")
Graph.mem_e g1 e3

(* nastepniki *)
let getLabelList vList =
	List.map V.label vList

getLabelList (Graph.succ g1 v1)
getLabelList (Graph.succ g1 v3)




