(* ----- ----- ----- ----- ----- *)
(* ----- --- DEFINICJE --- ----- *)
(* ----- ----- ----- ----- ----- *)
type ’var prop =
	  Var of ’var
	| Not of ’var prop
	| And of ’var prop * ’var prop
	| Or of ’var prop * ’var prop
	| Imp of ’var prop * ’var prop;;

type ’var lit =
	  Pos of ’var
	| Neg of ’var;;
	
type ’var nnf =
	  LitNNF of ’var lit
	| AndNNF of ’var nnf * ’var nnf
	| OrNNF of ’var nnf * ’var nnf;;
	
type ’var clause = ’var lit list;;

type ’var cnf = ’var clause list;;

let var_to_char = function
	| Var(x) -> x;;
(* ----- ----- ----- ----- ----- *)

let first xs = xs;;
let second xs = List.tl xs;;
let third xs = List.tl (List.tl xs);;


(* zad1 DONE *)
let isVar c =
	if (int_of_char c) >= 97 && (int_of_char c) <= 122
	then true
	else false;;

let parse_rpn (s : string) =
	
	let n = String.length s in
	
	let rec parse stack i =
		try
			if (i=n) then (List.hd stack)
			else let c = String.get s i in
				if (isVar c) then parse (Var(c)::stack) (i+1)
				else match c with
				| '~' -> parse (Not (List.hd stack)::(second stack)) (i+1)
				| '*' -> parse (And (List.hd (second stack), List.hd stack)::(third stack)) (i+1)
				| '+' -> parse (Or  (List.hd (second stack), List.hd stack)::(third stack)) (i+1)
				| '>' -> parse (Imp (List.hd (second stack), List.hd stack)::(third stack)) (i+1)
				| _   -> raise (Failure "")
		with Failure(x) -> raise (Failure "Nieprawidlowe wejscie formuly.")
	in
	
	parse [] 0;;


	
	
let unparse_rpn (cp : char prop) =

	let rec parse node =		
		match node with
		| Var (v) -> (String.make 1 v)
		| Not (l) -> String.concat "" [(parse l) ; "~"]
		| And (l,r) -> String.concat "" [(parse l) ; (parse r) ; "*"]
		| Or  (l,r) -> String.concat "" [(parse l) ; (parse r) ; "+"]
		| Imp (l,r) -> String.concat "" [(parse l) ; (parse r) ; ">"]
		| _ -> raise (Failure "Podano zle drzewo wyrazen!")
	in
	
	parse cp;;

(* ----- ----- ----- ----- ----- *)
(*
let parse_prop (s : string) =
	
	let n = String.length s in

	(* TODO: jesli List.hd albo tl zwroci blad tzn ze blad wejsciowego stringa *)
	let rec parse (stack : char list) out i =
		if (i=n) then out (*TODO*)
		else let c = String.get s i in
			if (isVar c) then parse stack (c::out) (i+1)
			else match c with
			| '(' -> parse (c::stack) out (i+1)
			| ')' ->
				(
				match (List.hd stack) with
				| '(' -> parse (List.tl stack) out (i+1)
				| _ -> parse (List.tl stack) ((List.hd stack)::stack) i
				)
			| '~' -> parse (c::stack) out (i+1) (* wloz na stos; zawsze najwyzszy priorytet *)			
			| '*' -> 
				(
				match (List.hd stack) with
				| []  -> parse (c::stack) out (i+1)
				| '(' -> parse (c::stack) out (i+1) (* wrzuc na stos operatorow i parsuj dalej *)
				| '*' -> parse stack (c::out) (i+1) (* zdejmij '*' ze stosu i wrzuc do kolejki wyjsciowej i wloz '*' na stos *)
				| _   -> parse (List.tl stack) ((List.hd stack)::out) i (* zdejmij ze stosu i doloz do kolejki wyjsciowej; i zostaje w tym samym miejscu *)
				)
(*			
			| '+' -> 
				(
				match (List.hd stack) with
				| []  -> parse (c::stack) out (i+1)
				| '(' -> parse (c::stack) out (i+1)
				| '*' -> parse (c::stack) out (i+1)
				| '+' -> parse stack (c::out) (i+1)
				| _   -> parse (List.tl stack) ((List.hd stack)::out) i
				)
			| '>' -> 
				(
				match (List.hd stack) with
				| []  -> parse (c::stack) out (i+1)
				| '(' -> parse (c::stack) out (i+1)
				| '*' -> parse (c::stack) out (i+1)
				| '+' -> parse (c::stack) out (i+1)
				| '>' -> parse (c::stack) out (i+1)
				)
			| _   -> raise (Failure "Napotkano znak niebedacy ani zmienna ani spojnikiem.")
*)			
		in 
		
		parse [] [] 0;;


	
	Póki zostały symbole do przeczytania wykonuj:
Przeczytaj symbol.
---------------------Jeśli symbol jest liczbą dodaj go do kolejki wyjście.
Jeśli symbol jest operatorem, o1, wtedy:
1) dopóki na górze stosu znajduje się operator, o2 taki, że:
o1 jest lewostronnie łączny i jego kolejność wykonywania jest mniejsza lub równa kolejności wyk. o2,
lub
o1 jest prawostronnie łączny i jego kolejność wykonywania jest mniejsza od o2,
zdejmij o2 ze stosu i dołóż go do kolejki wyjściowej i wykonaj jeszcze raz 1)
2) włóż o1 na stos operatorów.
-----------------------------Jeżeli symbol jest lewym nawiasem to włóż go na stos.
Jeżeli symbol jest prawym nawiasem to zdejmuj operatory ze stosu i dokładaj je do kolejki wyjście, dopóki symbol na górze stosu nie jest lewym nawiasem, kiedy dojdziesz do tego miejsca zdejmij lewy nawias ze stosu bez dokładania go do kolejki wyjście. Teraz, jeśli najwyższy element na stosie jest funkcją, także dołóż go do kolejki wyjście. Jeśli stos zostanie opróżniony i nie napotkasz lewego nawiasu, oznacza to, że nawiasy zostały źle umieszczone.
Jeśli nie ma więcej symboli do przeczytania, zdejmuj wszystkie symbole ze stosu (jeśli jakieś są) i dodawaj je do kolejki wyjścia. (Powinny to być wyłącznie operatory, jeśli natrafisz na jakiś nawias oznacza to, że nawiasy zostały źle umieszczone.)

*)

(* zad3 DONE *)
let nnf_of_prop (vp : 'var prop) =
	
	let rec nnf_neg node =
		match node with
		| Var (v) -> LitNNF(Neg (v))
		| Not (l) -> nnf l
		| And (l,r) -> OrNNF (nnf_neg l, nnf_neg r)
		| Or  (l,r) -> AndNNF (nnf_neg l, nnf_neg r)
		| Imp (l, r) -> AndNNF (nnf l, nnf_neg r)
	and
	
	nnf node =		
		match node with
		| Var (v) -> LitNNF(Pos (v))
		| Not (l) -> nnf_neg l
		| And (l,r) -> AndNNF (nnf l, nnf r)
		| Or  (l,r) -> OrNNF  (nnf l, nnf r)
		| Imp (l,r) -> OrNNF (nnf_neg l, nnf r)
		| _ -> raise (Failure "Podano zle drzewo wyrazen!")
	in
	
	nnf vp;;

let exmp1 = Not(And(Or(Var 'p',Var 'q'),Var 'r'));;
let exmp2 = Not(Not(Not(Imp(Var 'p',And(Var 'q',Or(Not(Var 'a'),Var 'b'))))));;

(* wolfram: p && !(p && !q || !(!p => q || r)) *)
let exmp3 = And(Var 'p',Not(Or(And(Var 'p',Not(Var 'q')),Not(Imp(Not(Var 'p'),Or(Var 'q',Var 'r'))))));;


(* zad4 DONE *)
let cnf_of_prop (vp : 'var prop) : 'var cnf =
	
	let nnf = nnf_of_prop vp in
	
	(* to mozna na foldy *)
	(* ----- ----- ----- ----- ----- *)
	let rec join xs qss =
		match qss with
		| [] -> []
		| qs::qss -> (xs@qs)::(join xs qss)
	in
		
	let rec join_clauses pss qss =
		match pss with
		| [] -> []
		| ps::pss -> (join ps qss) @ (join_clauses pss qss)
	in
	(* ----- ----- ----- ----- ----- *)
	
	let rec nnf_to_cnf node =
		match node with
		| LitNNF (Pos l) -> [[Pos (l)]] (* lista z jednego literalu to klauzula *)
		| LitNNF (Neg l) -> [[Neg (l)]]
		| AndNNF (l,r) -> List.append (nnf_to_cnf l) (nnf_to_cnf r) (* laczymy listy klauzul *)
		| OrNNF (l,r) ->
			let pss = nnf_to_cnf l in (* to sa formuly w CNF czyli listy list literalow *)
			let qss = nnf_to_cnf r in (* Q = ((l1,l2,l3),(...),(...)) *)
			(* my chcemy: ( (P1@Q1) (P1@Q2) ... (Pn@Q1) (Pn@Q2) ... ) *)
			(join_clauses pss qss)
	in

	nnf_to_cnf nnf;;
			
(* zad5 DONE *)
let var_of_prop (vp : 'var prop) =
	
	(* spr czy x jest na liscie xs *)
	let rec on_list x xs =
		match xs with
		| [] -> false
		| hd::tl when (hd == x) -> true
		| hd::tl -> on_list x tl
	in
	
	(* scala dwie listy usuwajac powtorzenia *)
	let rec merge xs ys =
		match xs with
		| [] -> ys
		| hd::tl when (on_list hd ys) ->
			merge tl ys
		| hd::tl ->
			merge tl (hd::ys)
	in
	
	let rec find prop =
		match prop with
		| Var(v)   -> [v]
		| Not(l)   -> find l
		| And(l,r) -> merge (find l) (find r)
		| Or (l,r) -> merge (find l) (find r)
		| Imp(l,r) -> merge (find l) (find r)
	in 
	
	find vp;;
	
(* zad6 DONE *)
let rec subst_prop (f : (’v -> ’w prop)) (vp : 'v prop) =

	match vp with
	| Var(v)   -> f v
	| Not(l)   -> Not(subst_prop f l)
	| And(l,r) -> And(subst_prop f l, subst_prop f r)
	| Or (l,r) -> Or(subst_prop f l, subst_prop f r)
	| Imp(l,r) -> Imp(subst_prop f l, subst_prop f r);;	

let phi = (And (Var ’p’, Var ’q’) : char prop);;
subst_prop (fun v -> Var (int_of_char v)) phi;;
subst_prop (function ’p’ -> Or (Var ’q’, Var ’s’) | v -> Var v) phi;;


(* zad7 DONE *)
let rec valuation f (vp : 'v prop) =

	match vp with
	| Var(v)   -> f v
	| Not(l)   -> not (valuation f l)
	| And(l,r) -> (valuation f l) && (valuation f r)
	| Or (l,r) -> (valuation f l) || (valuation f r)
	| Imp(l,r) -> (not (valuation f l)) || (valuation f r);;

valuation (function 'p' -> true | 'q' -> false | v -> true) phi;;
valuation (function 'p' -> true | 'q' -> false | v -> true) exmp3;;

(* zad8 DONE *)
type varval = (char * bool) list;;
exception Unvalued of char;;

let rec getval (varval : varval) c =
	match varval with
	| [] -> raise (Unvalued c)
	| hd::tl when (fst hd == c) -> (snd hd)
	| hd::tl -> (getval tl c);;

getval [(’p’,true) ; (’q’,false) ; (’r’,false)] ’q’;;
getval [(’p’,true) ; (’q’,false) ; (’r’,false)] ’x’;; (* error Unvalued *)

(* zad9 DONE *)

let dig_of_char c =
	(int_of_char c) - (int_of_char '0');;

let rec string_of_bools bools =
	match bools with
	| [] -> ""
	| bl::bls ->
		String.concat ""
			[ (if (bl = true) then "1" else "0") ; string_of_bools bls];;

let bools_of_string s =
	
	let n = String.length s in

	let rec aux i =
		if (i=n) then []
		else (if (String.get s i = '1') then true else false)::(aux (i+1))
	in
	
	aux 0;;

let nextval (vv : varval) : varval option =

	(* DEC TO BIN *)
	let add c s = String.concat "" [c;s] in
	let rec bin_of_dec n bin d = (* n-liczba, b-bin, d-ilosc bitow *)
		if (d = 0) then bin
		else
			if (n mod 2 = 1)
			then bin_of_dec (n/2) (add "1" bin) (d-1)
			else bin_of_dec (n/2) (add "0" bin) (d-1)
	in
	
	(* BIN TO DEC *)
	let rec int_of_bin n bin d k = (* wywolywac z n=0, k=1 *)
		if (d <= 0) then n
		else int_of_bin (n + k*(dig_of_char (String.get bin (d-1)))) bin (d-1) (2*k)
	in
	
	(* LACZENIE W VARVAL *)
	let make_some varlist boollist =
		List.map2 (fun x y -> (x,y)) varlist boollist
	in

	(* FUNKCJA *)
	let try_next lst =
		let n = List.length lst in
		let varlist = List.map (fun (x,y) -> x) lst in
		let boollist = List.map (fun (x,y) -> y) lst in
		let boolean = string_of_bools boollist in
		let dec = int_of_bin 0 boolean n 1 in
		if (dec = int_of_float (2.0**(float_of_int n)) - 1) then None else
			let next_boolean = bin_of_dec ((int_of_bin 0 boolean n 1) + 1) "" n in
			let next_boollist = bools_of_string next_boolean in
			let result = make_some varlist next_boollist in
			Some (result)
	in
	
	try_next vv;;
		
(* dziala, tylko na odwrot niz w przykladzie *)
nextval [(’p’,false); (’q’,true); (’r’,true)];;


(* zad10 DONE *)
let sat_prop prop =

	let vars = var_of_prop prop in
	let first_valuation = List.map (fun x -> (x,false)) vars in
	
	let rec boolean_of_prop valuation prop =
		match prop with
		| Var(v) -> getval valuation v
		| Not(l) -> not (boolean_of_prop valuation l)
		| And(l,r) -> (boolean_of_prop valuation l) && (boolean_of_prop valuation r)
		| Or(l,r) -> (boolean_of_prop valuation l) || (boolean_of_prop valuation r)
		| Imp(l,r) -> (not (boolean_of_prop valuation l)) || (boolean_of_prop valuation r)
	in
	
	
	let rec seek valuation =
		if (boolean_of_prop valuation prop = true) then true
		else
			let next = nextval valuation in
			match next with
			| Some(v) -> seek v
			| None -> false
	in
	
	seek first_valuation;;
	
let exmp4 = Imp(Imp(Var 'p',Var 'q'),Imp(Var 'q',Var 'p'));; (* (p->q) -> (q->p) *)
let exmp5 = And(Imp(Var 'q',And(Var 'p',Var 'r')),Not(Imp(Or(Var 'p', Var 'q'),Or(Var 'p',Var 'r'))));;	(* (q -> (p^r)) ^ ~(pVq -> pVr) *) 
sat_prop exmp5;;
	
(* ===== ===== ===== ===== ===== *)
(* ===== ====  DRZEWA ==== ===== *)
(* ===== ===== ===== ===== ===== *)

type ’a btree = Leaf | Node of ’a btree * ’a * ’a btree;;
type ’a mtree = MTree of ’a * ’a forest
and ’a forest = ’a mtree list;;

(* zad 11 DONE *)
let rec bprefix btree =
	
	(* acc - to, co uzbierales do tej pory *)
	let rec f tree acc =
		match tree with
		| Leaf -> acc
		| Node(l,v,r) -> v::(f l (f r acc))
	in f btree [];;


let rec mprefix mtree =
	
	(* acc - to, co uzbierales do tej pory *)
	let rec f trees acc =
		match trees with
		| [] -> acc
		| MTree(hd,tl)::trees -> hd::(f tl (f trees acc))
	in f [mtree] [];;

	
	
let t1 = Node(Node(Node(Leaf,4,Leaf),2,(Node(Leaf,5,Leaf))),1,Node(Node(Leaf,6,Leaf),3,(Node(Leaf,7,Leaf))));;
(*
 *       1
 *     /  \
 *    2    3
 *   /\    /\
 *  4 5   6 7
 *)


let mt1 = MTree(1,[MTree(2,[MTree(6,[]);MTree(7,[])]) ; MTree(3,[]) ; MTree (4,[MTree(8,[])]) ; MTree(5,[])]);;
(*
 *           1
 *        -------
 *      /  /  \  \
 *    2   3   4   5
 *   /\       |
 *  6 7       8
 *)
