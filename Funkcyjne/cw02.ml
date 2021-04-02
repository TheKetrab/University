
let rand n =
	let rec aux i acc =
		if (i>=n) then acc
		else (aux (i+1) (((Random.int 1000)-500)::acc))
	in aux 0 [];;

(* ===== ===== =====*)
(* zad1 DONE *)


(* wersja nieogonowa *)
let ins_everywhere v xs =

	let rec ins_at_pos v n i xs =
		match xs with
		| [] -> [v]
		| hd::tl ->
			if (n = i) then v::xs
			else  hd::(ins_at_pos v n (i+1) tl)
	in

	let len = List.length xs in

	let rec loop i =
		if (i > len) then []
		else (ins_at_pos v i 0 xs)::(loop (i+1))
	in
	
	loop 0;;

(* wersja ogonowa *)
let ins_everywhere_tail v xs =

	let ins_at_pos v n xs =
		let rec aux i xs acc =		(* Jak to dziala? 				*)
			match xs with			(* (01234) 						*)
			| [] ->					(* (1234)(0) // (xs)(acc) 		*)
				List.append (List.rev acc) [v]	(* ---------------- *)			
			| hd::tl ->				(* (234)(10) -> (34)(210) 		*)
				if (i = n)			(* wynik: append (rev acc) xs	*)
				then (List.append (List.rev acc) (v::xs))
				else (aux (i+1) tl (hd::acc))
		in aux 0 xs []
	in

	let rec loop i acc =
		if (i < 0)
		then acc
		else (loop (i-1) ((ins_at_pos v i xs)::acc))
	in loop (List.length xs) [];;
	
ins_everywhere 0 [1;2;3];;
ins_everywhere_tail 0 [1;2;3];;

(* ===== ===== =====*)
(* zad2 DONE *)

let iperm xs =

	let rec multi_ins v xss acc =
		match xss with
		| [] -> acc
		| xs::tl -> multi_ins v tl (List.append (ins_everywhere v xs) acc)
	in

(*
	let rec multi_ins v xss =
		match xss with
		| [] -> []
		| xs::tl -> 
			(ins_everywhere v xs)::multi_ins v tl
	in
*)
	
	let rec aux xs acc =
		match xs with
		| [] -> acc
		| hd::tl ->
			(aux tl (multi_ins hd acc []))
	in aux xs [[]];;	

(* ===== ===== =====*)
(* zad3 DONE *)
let isort xs =

	let rec ins_ord v xs =
		match xs with
		| [] -> [v]
		| hd::tl when (v <= hd) -> (v::xs)
		| hd::tl -> hd::(ins_ord v tl)
	in
	
	let rec loop xs acc =
		match xs with
		| [] -> acc
		| hd::tl -> loop tl (ins_ord hd acc)
	in
	
	loop xs [];;

	
(* ===== ===== =====*)
(* zad4 DONE *)
let isort_tail xs =

	let rec ins_ord_tail v xs acc =
		match xs with
		| [] ->
			(List.append (List.rev acc) [v])
		| hd::tl when (v <= hd) ->
			(List.append (List.rev acc) (v::xs))
		| hd::tl ->
			(ins_ord_tail v tl (hd::acc)) 
	in

	let rec sort xs acc =
		match xs with
		| [] -> acc
		| hd::tl -> sort tl (ins_ord_tail hd acc [])
	in
	
	sort xs [];;


(* ===== ===== =====*)
(* zad 5 DONE *)
let sel_anything xs =

	let rec take_nth_elem n i xs acc =
		if (n = i)
		then (List.hd xs, (List.append (List.rev acc) (List.tl xs)))
		else take_nth_elem n (i+1) (List.tl xs) ((List.hd xs)::acc)
	in
	
	let rec all i xs acc =
		if (i < 0)
		then acc
		else all (i-1) xs ((take_nth_elem i 0 xs [])::acc)
	in
	
	all ((List.length xs)-1) xs [];;
		
		

(* ===== ===== =====*)
(* zad 6 DONE *)
let rec sperm xs =

	(* dolacza v na poczatek kazdego elementu listy *)
	(* dziala ogonowo, wynik jest lista odwrocona *)
	let rec join v xss acc =
		match xss with
		| [] -> acc
		| hd::tl ->
			join v tl ((v::hd)::acc)
	in
	
	(* xs - lista par, ktora przetwarzamy *)
	let rec loop xs acc =
		match xs with
		| [] -> acc
		| hd::tl ->
			let permuted = sperm (snd hd) in
			let joined = join (fst hd) permuted [] in
			
			loop tl (List.append joined acc)
			(* pytanie: jak uniknac appenda? *)
	in

	match xs with
	| [] -> [[]]
	| _ -> loop (sel_anything xs) [];;

	
(* ===== ===== =====*)
(* zad7 DONE *)

(* zwraca pare (min * lista_bez_minimum) *)
(* dziala ogonowo *)
let rec take_min_elem xs min acc =
	match xs with
	| [] -> (min, acc)
	| hd::tl when (hd < min) ->
		(take_min_elem tl hd (min::acc))
	| hd::tl ->
		(take_min_elem tl min (hd::acc));;

(* dziala nieogonowo *)
let rec ssort lst =
	match lst with
	| [] -> []
	| hd::tl -> 
		let result = (take_min_elem tl hd []) in
		(fst result)::(ssort (snd result));;

(* ===== ===== =====*)
(* zad8 dziala, ale nie wytlumacze tych glebokosci itp *)
let ssort_tail lst =
	
	let rec loop xs acc =
		match xs with
		| [] -> (List.rev acc)
		| hd::tl -> 
			let result = (take_min_elem tl hd []) in
			loop (snd result) ((fst result)::acc)
	in
	
	loop lst [];;
	
let rec gen n =
	if (n<0) then []
	else 10::(gen (n-1))
;;	

(* ===== ===== =====*)
(* zad 9 DONE (todo: sprawdzic szybkosc dla sperm, iperm) *)
let monotone xs =
	
	let rec aux xs prev =
		match xs with
		| [] -> true
		| hd::tl when (prev <= hd) -> (aux tl hd)
		| _ -> false
	in
	
	if (xs = []) then true
	else aux (List.tl xs) (List.hd xs);;

	
let perm_sort p xs =

	let all_permutations = (p xs) in
	
	let rec loop xss =
		if (monotone (List.hd xss)) then (List.hd xss)
		else loop (List.tl xss)
	in 
	
	loop all_permutations;;

(* perm_sort sperm (rand 9);; *) (* 3s *)
(* perm_sort iperm (rand 9);; *)(* 4s *)
	
(* ===== ===== =====*)
(* zad 10 mialo byc bez length, nie wiem jak *)
	
let split xs =
	let rec splitter i n xs acc =
		if (i = n)
		then (List.rev acc,xs)
		else splitter (i+1) n (List.tl xs) ((List.hd xs)::acc)
	in splitter 0 ((List.length xs)/2) xs [];;
	(* funkcja dziala poprawnie *)


(* dwa wskazniki, jeden przesuwamy o 1,
   drugi przesuwamy o 2. jak drugi dojdzie
   do konca listy, to ten drugi jest w polowie *)
let split xs =
	let rec aux acc left right =
		match left, right with
		| l::left, _::_::right -> aux (l::acc) left right
		| left, [_] -> (List.rev acc, left)
	in aux [] xs xs;;
		
(* ===== ===== =====*)
(* zad 11 DONE *)
let rec (<+>) xs ys =
	match xs, ys with
	| [], [] -> []
	| hx::tx, [] -> xs
	| [], hy::ty -> ys
	| hx::tx, hy::ty when (hx<hy) ->
		hx::(tx<+>ys)
	| hx::tx, hy::ty ->
		hy::(xs<+>ty);;

let rec msort xs =
	match xs with
	| [] -> []
	| hd::[] -> [hd]
	| hd::tl ->
		let splitted = split xs in
		(msort (fst splitted))<+>(msort (snd splitted));;
	
	
(* 12 *)
(*
let rev_merge comp =
	let aux acc xs ys = match with
		| [],[] -> acc
		| xs,[] -> (List.rev_append xs acc)
		| [],ys -> (List.rev_append ys acc)
		| x::xs as xs', y::ys as ys' ->
			if comp x y
			then aux (x::acc) xs ys'
			else aux (y::acc) xs' ys
	in aux [] xs ys
			;;
*)