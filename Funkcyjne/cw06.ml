#load "streams.cmo";;

type board_size = int * int;;
type position = int * int;;


(* cw 1 *)
(* knight_list : board_size -> position -> position list Streams.t *)
let rec knight_list size pos =




Jeśli w trakcie obliczeń droga skoczka jest opisana listą [(ik, jk), . . . ,(i0, j0)],
to możemy ją (zarówno drogę, jak i listę) przedłużyć o jeden krok w następujący sposób: 

wyznaczamy listę od 2 do 8 pól (mniej niż 8, jeśli skoczek znajduje się przy brzegu szachownicy), na które skoczek może przejść w jednym kroku.
Odrzucamy te z pól, które skoczek już odwiedził (tzn. które znajdują się już w budowanej liście).
Dla każdego z pozostałych pól przedłużamy ścieżkę o to pole (dodajemy je do głowy listy)
i rekurencyjnie uruchamiamy algorytm na przedłużonej liście.
Rekursja kończy się gdy wykonamy m · n − 1 kroków


(* zakladamy, ze indeksujemy od 1 do 8 wlacznie *)
let rec bPossiblePosition size pos =
	match pos, size with
	| (x,y) , (m,n) ->
		if (x<1 || y<1 || x>m || y>n)
		then false
		else true;;

let rec bFreePosition pos occupiedList =
	match occupiedList with
	| [] -> true
	| x::xs when (x = pos) -> false
	| x::xs -> bFreePosition pos xs;;

let getPossiblePositionsList size pos =
	
	let m = fst size in
	let n = snd size in
	let x = fst pos in
	let y = snd pos in
	
	let rec aux acc i =
		match i with
		| 0 when (bPossiblePosition size (x+1,y+2)) ->
			aux ((x+1,y+2)::acc) (i+1)
		| 1 when (bPossiblePosition size (x+2,y+1)) ->
			aux ((x+2,y+1)::acc) (i+1)
		| 2 when (bPossiblePosition size (x+2,y-1)) ->
			aux ((x+2,y-1)::acc) (i+1)
		| 3 when (bPossiblePosition size (x+1,y-2)) ->
			aux ((x+1,y-2)::acc) (i+1)
		| 4 when (bPossiblePosition size (x-1,y-2)) ->
			aux ((x-1,y-2)::acc) (i+1)
		| 5 when (bPossiblePosition size (x-2,y-1)) ->
			aux ((x-2,y-1)::acc) (i+1)
		| 6 when (bPossiblePosition size (x-2,y+1)) ->
			aux ((x-2,y+1)::acc) (i+1)
		| 7 when (bPossiblePosition size (x-1,y+2)) ->
			aux ((x-1,y+2)::acc) (i+1)
		| 8 -> acc
		| _ -> aux acc (i+1)
	in
	
	aux [] 0;;
	
(* 
getPossiblePositionsList (8,8) (4,5);;
*)

let rec modifyPostionsListToFree lst occupied =
	match lst with
	| [] -> []
	| x::xs when (bFreePosition x occupied) ->
		x::(modifyPostionsListToFree xs occupied)
	| x::xs ->
		modifyPostionsListToFree xs occupied;;

		
let rec findWayFrom size pos waySoFar step =

	match size, pos with
	| (m,n),(x,y) ->
		
		if (step = m*n - 1) then Some(waySoFar) else
		
		let newOccupation = pos::waySoFar in
		let newWays = (modifyPostionsListToFree (getPossiblePositionsList size pos)) in
		
		let aux 
		
		
			



(* ------------------------------------------------ *)


let solutions n =
 
  let show board =
    let pr v =
      for i = 1 to n do
        print_string (if i=v then " q" else " _");
      done;
      print_newline() in
    List.iter pr board;
    print_newline() in
 
  let rec safe i j k = function
    | [] -> true
    | h::t -> h<>i && h<>j && h<>k && safe i (j+1) (k-1) t in
 
  let rec loop col p =
    for i = 1 to n
    do
      if safe i (i+1) (i-1) p then
        let p' = i::p in
        if col = n then show p'
        else loop (col+1) p'
    done in
 
  loop 1 [] in
 
let n =
  if Array.length Sys.argv > 1
  then int_of_string Sys.argv.(1)
  else 8 in
 
solutions n




(* ------------------------------------------------ *)

let 

let rec free x y lst =

	match lst with
	| [] -> true
	| hd::tl -> 




