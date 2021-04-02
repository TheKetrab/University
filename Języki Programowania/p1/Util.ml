(* Bartłomiej Grochowski 300951 *)
(* Util.ml *)

(* zwraca index, na której pozycji występuje x *)
let index_of x lst =
  let rec aux lst n =
    match lst with
    | [] -> -1
    | hd :: tl when hd = x -> n
    | hd :: tl -> aux tl (n+1)
  in aux lst 0

(* konkatenacja list, ogonowo, na koniec reverse *)
let append xs ys =
  let rec aux acc lst =
    match lst with
    | [] -> acc
    | hd :: tl -> aux (hd::acc) tl
  in aux ys (List.rev xs)

(* 0-a, 1-b, ... 25-z, 26-a', ... *)
let var_name n =
  let div = n / 26 and rem = n mod 26 in
  let letter = String.make 1 (char_of_int (rem+97)) in
  let prims = String.make div '\'' in
  letter ^ prims

(* List.iter *)
let rec for_each (xs : 'a list) (f : 'a -> unit) : unit =
  match xs with
  | [] -> ()
  | hd::tl -> (begin f hd; for_each tl f; end)
