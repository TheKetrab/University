(* Bartlomiej Grochowski *)
(* Lista 3 *)
(* ----- ----- ----- *)

(* zad1 *)
let atoi_tail xs =
	let rec aux acc xs =
		match xs with
		| [] -> acc
		| hd::tl -> 
			let n = (int_of_char hd) - (int_of_char '0') in
			aux (10*acc+n) tl
	in aux 0 xs;;
	
let atoi xs =
	List.fold_left
		(fun acc x -> 10*acc + (int_of_char x - int_of_char '0')) 0 xs;;
		(* funkcja dla akumulatora acc i iksa da nowy akumulator 10*n + 'x'-'0' *)

		
(* zad2 *)
let polynomial_tail xs x =
	let rec aux acc xs =
		match xs with
		| [] -> acc
		| hd::tl -> aux (acc*.x +. hd) tl
	in aux 0. xs;;
	
let polynomial xs x =
	List.fold_left (fun acc hd -> acc*.x +. hd) 0. xs;;
		
polynomial [1.;2.;3.] 3.;;
(* x^2 + 2x + 3 = 18 *)
(* ----- ----- ----- ----- ----- *)


(* zad5 *)
let rec revpoly xs x0 =
	match xs with
	| [] -> 0.
	| hd::tl -> hd +. x0*.(revpoly tl x0);;
	
let revpoly_foldr xs x0 =
	List.fold_right (fun x xs -> x +. x0*.xs) xs 0.;;
	
let revpoly_tail xs x =
	let rec aux acc xs =
		match xs with
		| [] -> acc
		| hd::tl -> aux (acc*.x +. hd) tl
	in aux 0. (List.rev xs);;
	
let revpoly_foldl xs x =
	List.fold_left (fun acc hd -> acc*.x +. hd) 0. (List.rev xs);;


revpoly 		[5.;4.;-3.;0.;1.] 4.;;
revpoly_foldl  	[5.;4.;-3.;0.;1.] 4.;;
revpoly_tail  	[5.;4.;-3.;0.;1.] 4.;;
revpoly_foldr  	[5.;4.;-3.;0.;1.] 4.;;
(* ----- ----- ----- ----- ----- *)


(* zad9 *)
(* --> zrob sublist na ogonie, wynikiem sa te sublisty 
	   oraz te sublisty z glowa na poczatku *)
let rec sublist xs =
	match xs with
	| [] -> [[]]
	| hd::tl -> let sub_tl = sublist tl in
		List.append
			sub_tl
			(List.map (fun xs -> hd::xs) sub_tl);;
			
let rec sublist_foldl xs =
	List.fold_left
		(fun acc hd -> List.append acc (List.map (fun xs -> hd::xs) acc)) [[]] (List.rev xs);;
(* ----- ----- ----- ----- ----- *)




(* ===== ===== ======== ===== ===== *)
(* ===== ===== MACIERZE ===== ===== *)
(* ===== ===== ======== ===== ===== *)

type ’a mtx = ’a list list;;
exception Mtx of string;;
(* raise (Mtx "opis błedu ") *)

type mtx_info = {rows:int; columns:int};;

(* zad10 *)
let mtx_dim m =
	if (m = []) then raise (Mtx "macierz bez kolumn")
	else
		let rws = List.length m in
		let clmns = List.length (List.hd m) in
		
		let rec check_rows xss =
			match xss with
			| [] -> true
			| hd::tl -> (List.length hd = clmns) && (check_rows tl)
		in
		
		if (check_rows m = false)
		then raise (Mtx "wiersze maja rozna dlugosc")
		else {rows = rws; columns = clmns};;
(* ----- ----- ----- ----- ----- *)

		
(* zad11 *)
let rec mtx_row i m =
	if (i=1)
	then (List.hd m)
	else mtx_row (i-1) (List.tl m);;
		
let mtx_column j m =
	List.fold_right (fun col acc -> (List.nth col (j-1))::acc) m [];;
		
let mtx_elem j i m =
	List.nth (List.nth m (i-1)) (j-1);;
(* ----- ----- ----- ----- ----- *)

(* zad12 *)
let transpose m =
	let cols = (List.length (List.hd m)) in
	
	let rec aux i result =
		if (i=cols) then (List.rev result)
		else aux (i+1) ((List.fold_right (fun xs acc -> (List.nth xs i)::acc) m [])::result)
	in aux 0 [];;
	

let m1 = ([[1.;2.;3.];[4.;5.;6.]] : float mtx);;
let m2 = ([[10.;20.;30.];[40.;50.;60.]] : float mtx);;
(* ----- ----- ----- ----- ----- *)
	
(* zad13 *)
let mtx_add (m1 : float mtx) (m2 : float mtx) : float mtx =
	
	(* zakladamy, ze uzytkownik podal dobre macierze,
	   czyli maja tyle samo wierszy i kolumn, stad tylko
	   dwa przypadki w pattern matchingu *)
	
	let rec sum_all_rows xss yss =
		match (xss, yss) with
		| [],[] -> []
		| xs::xss, ys::yss -> (List.map2 (fun x y -> x+.y) xs ys)::(sum_all_rows xss yss)
	in sum_all_rows m1 m2;;
(* ----- ----- ----- ----- ----- *)	
		
(* zad14 *)
let scalar_prod u v =
	List.fold_left2 (fun acc el_u el_v -> (acc +. (el_u*.el_v))) 0. u v;;
	
let polynomial_vec pol x =

	(* bedziemy mnozyc skalarnie: (pol)o(x^n,x^(n-1),...,x,1) *)
	
	let pol_length = List.length pol in
	
	let rec make_x_vec i =
		if (i=1) then [1.]
		else (x**(float_of_int (i-1))) :: make_x_vec (i-1)
	in
	
	scalar_prod pol (make_x_vec pol_length);;

(* ----- ----- ----- ----- ----- *)	

	
	
(* zad15 *)

let rec mtx_apply (m : float mtx) v =
	
	match m with
	| [] -> []
	| x::xs -> (scalar_prod x v) :: (mtx_apply xs v);;
	
mtx_apply [[1.;2.;3.];[0.;0.;1.]] [4.;5.;6.;];;
	
	
let mtx_mul (m1 : float mtx) (m2 : float mtx) =
	
	let trans_m2 = transpose m2 in
	
	let rec result_row r trans_m2 =
		match trans_m2 with
		| [] -> []
		| x::xs -> (scalar_prod x r) :: (result_row r xs)
	in
	
	let rec mul_iter m1 trans_m2 =
		match m1 with
		| [] -> []
		| xs::xss -> (result_row xs trans_m2) :: (mul_iter xss trans_m2)
	in
	
	mul_iter m1 trans_m2;;
	
let m1 = [[1.;2.;3.];[4.;5.;6.];[7.;8.;9.]];;
let m2 = [[8.;6.;5.];[5.;1.;3.];[4.;6.;7.]];;
(* ----- ----- ----- ----- ----- *)
		
(* zad16 *)
let rec det (m : float mtx) =
	
	(* rozwiniecie LaPlacea wg pierwszego wiersza *)
	
	let rec remove_nth xs n =
		match xs with
		| [] -> []
		| x::xs when (n=0) -> xs
		| x::xs -> (x::remove_nth xs (n-1))
	in
	
	let rec mtx_without_col m i =
		match m with
		| [] -> []
		| xs::xss -> (remove_nth xs (i-1))::(mtx_without_col xss i)
	in
	
	let rec loop acc row j =
		match row with
		| [] -> acc
		| x::xs ->
			loop 
				(acc +. 
					(((-1.)**(float_of_int (1+j))) *. x
					*. (det (mtx_without_col (List.tl m) j))))
					(* minory tworzymy ignorujac pierwszy wiersz i usuwajac j-ta kolumne *)
			xs
			(j+1)
	in
	
	match m with
	| [] -> 1.
	| x::xs -> loop 0. x 1;;
(* ----- ----- ----- ----- ----- *)
	
	

let mf1 = ([[1.;2.;3.];[5.;6.;7.];[9.;10.;11.]] : float mtx);;
let mf2 = ([[5.;2.;3.];[5.;6.;7.];[8.;9.;10.]] : float mtx);;
let mf3 = ([[1.;2.];[3.;4.]] : float mtx);;
let mf4 = ([[1.;2.;4.];[3.;4.;5.];[6.;7.;9.]] : float mtx);;	




