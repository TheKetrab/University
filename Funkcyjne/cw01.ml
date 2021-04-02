(* ----- Zad 1 ----- *)
(*

Jaki jest typ wyr: fun x -> x
	- : 'a -> 'a

Napisz wyr, którego wart. jest Id, ale, które ma typ int->int
	# let int_id = if false then (fun x->5) else (fun x->x);;
	musi być taki sam typ zwracany w obu gałęziach ifa,
	ta pierwsza gałąź wymusza, żeby to była funkcja x->int
	a druga musi być int->int, a jest też identycznością.

Napisz wyr z typem: ('a -> 'b) -> ('c -> 'a) -> 'c -> 'b
                    |  f(x)  |    |  g(x)  |   | f(g(x)) |
	# let compose f g = fun x -> f (g x)

Napisz wyr z typem ('a -> 'b)
	
	# let rec f n = f n
	;; zapetla sie, temu 'b??
	albo fun x->hd[]

Wyr z typem ('a -> 'b)
let f = fun x y -> x;;
*)

(* ----- Zad 2 ----- *)

(* rekursja ogonowa: -1 dla duzych liczb 
   czyli taka ze jest akumulator *)
(* wywolywac z acc = 0 *)
(* dla duzych n wynik to -1 *)

let f_tail n =
	let rec f_tail_help n acc = 
		if n = 0
		then acc 
		else f_tail_help (n-1) (2*acc +1)
	in f_tail_help n 0 ;;


(* zwykla rekursja: stackoverflow dla duzych liczb *)
(* dla duzych n zawiesza sie *)
let rec f n = if (n = 0) then 0 else 2 * (f (n - 1)) + 1;;



(* ----- Zad 3 ----- *)

let (<.>) f g x  = f(g(x));;
lub: (<.>) f g = fun x -> f (g(x))

let f1 = fun x -> 2 * x;;
let f2 = fun x -> x + 1;;

(* przyklad: *)
# (f1 <.> f2) 2;;
- : int = 6
# (f2 <.> f1) 2;;
- : int = 5

let rec (<^>) f n =
  if n = 0
    then
      fun x -> x
    else
      f <.> (f <^> (n - 1));;
	  
(* przyklad: *)
(f1 <^> 2) 2;;
--> (2*(2*x)) [2] = 8

(* mnozenie NIE DZIALA *)
let rec (<*>) a b = ( ((fun x -> b + x)) <^> a ) 1;;
(* poteegowanie  *)
let (<**>) n m = fun x -> n*x)



(* ----- Zad 4 ----- *)
let rec ms1 = function x -> if (x=0) then 0 else ((ms1 (x-1)) + 1);;
let rec ms2 = function x -> if (x=0) then 0 else ((ms2 (x-1)) - 2);;
let hd s = s 0;;
let tl s = fun n -> s (n + 1);;
let add c s = function n -> (s n) + c;;
let map proc s = function n -> proc (s n);;
let map2 proc s1 s2 = function n -> proc (s1 n) (s2 n);;
let replace n a s = function x -> if (x mod n = 0) then a else s x;;
let take n s = function x -> s (n*x);;
(* SCAN TODO *)
let rec tabulate ?(b=0) e s = if (b=e) then [] else (s b)::(tabulate ~b:(b+1) e s);;
 


(* przyklady strumieni: *)
(* strumien liczb naturalnych *)
let rec ms1 = function x -> if (x=0) then 0 else ((ms1 (x-1)) + 1);;
tabulate 10 ms1;;
- : int list = [0; 1; 2; 3; 4; 5; 6; 7; 8; 9]

(* strumien liczb calkowitych postaci 2k *)
let rec ms2 = function x -> if (x=0) then 0 else ((ms2 (x-1)) - 2);;
tabulate 10 ms2;;
- : int list = [0; -2; -4; -6; -8; -10; -12; -14; -16; -18]


(* hd *)
let hd s = s 0;;

(* tl - zwraca strumien, tylko glowa jest nastepne *)
let tl s = fun n -> s (n + 1);;

(* add strumien, const*, daje strumien zwiekszony o c *)
let add c s = function n -> (s n) + c;;
tabulate 10 (add 5 ms1);;
- : int list = [5; 6; 7; 8; 9; 10; 11; 12; 13; 14]

(* map, mapujemy na argument strumienia *)
let map proc s = function n -> proc (s n);;
tabulate 10 (map (fun x -> 2*x) ms1);;
- : int list = [0; 2; 4; 6; 8; 10; 12; 14; 16; 18]

(* map2  - zwraca strumien funkcji mapujacej z dwoch strumieni*)
(* w moim przykladzie procedura tworzy strumien sumy poszczegolnych wyrazow dwoch strumieni *)
let map2 proc s1 s2 = function n -> proc (s1 n) (s2 n);;
tabulate 10 (map2 (fun x y -> x+y) ms1 ms2);;
- : int list = [0; -1; -2; -3; -4; -5; -6; -7; -8; -9]

(* replace *)
let replace n a s = function x -> if (x mod n = 0) then a else s x;;
tabulate 10 (replace 5 100 ms1);;
- : int list = [100; 1; 2; 3; 4; 100; 6; 7; 8; 9]


(* take - strumien co n-tego argumentu bazowego strumienia *)
let take n s = function x -> s (n*x);;
tabulate 10 (take 10 ms1);;
- : int list = [0; 10; 20; 30; 40; 50; 60; 70; 80; 90]

(* scan *)
(* ??? *)
let scan f a s i =
	let rec aux n acc =
		if n = i
		then acc
		else aux (i+1) (f acc (s i))
	in aux i a;;


(* tabulate *)
let rec tabulate ?(b=0) e s = if (b=e) then [] else (s b)::(tabulate ~b:(b+1) e s);;

let tabulate s ?(b=0) e =
	let rec aux i acc =
		if i<b
		then acc
		else aux (i-1) ((s i)::acc)
	in aux e [];;

	
(* ----- Zad 5 ----- *)
(* 
ile jest funkcji a->a->a??
a) f x y -> x
b) f x y -> y
*)
type Bool = B of a->a->a
let true = B(fun x y -> x)
let false = B(dun x y -> y)


let cor (B f) (B g) = B (fun x y -> f true (g true false)



(* ----- Zad 6 ----- *)
let zero = fun f x -> x;;
let succ cx = fun f x -> cx f (f x);;
let add cx cy = fun f x -> cx (cy f x);;
let mul cx cy = fun f x -> cy (cx f) x;;
let isZero cx = cx (fun x -> false) true;;
let int_of_cnum cx = cx ((+)1);;
let rec cnum_of_int n =
	if n=0
	then zero
	else succ (cnum_of_int(n-1));;


