
	type 'a t = 'a cell Lazy.t
	and 'a cell = Nil | Cons of 'a * 'a t

	let hd l =
		match l with
		| lazy Nil -> failwith "hd from Nil"
		| lazy (Cons (x, _)) -> x
	
	let tl l =
		match l with
		| lazy Nil -> failwith "tl from Nil"
		| lazy (Cons (_, xs)) -> xs
	
	let rec (++) (l1 : 'a t) (l2 : 'a t) : 'a t =
		match l1 with
		| lazy Nil -> l2
		| lazy (Cons(x,xs)) -> lazy (Cons(x, xs ++ l2))

	let init f =
		let rec init_from n =
			lazy (Cons(f n, init_from (n+1)))
		in init_from 0

	let rec map f s =
		match s with
		| lazy Nil -> lazy Nil
		| lazy (Cons(x, xs)) -> lazy (Cons(f x, map f xs))
		
	let rec map2 f s1 s2 =
		match s1,s2 with
		| lazy Nil, lazy Nil -> lazy Nil
		| lazy (Cons(x,xs)), lazy (Cons(y,ys)) ->
			lazy (Cons(f x y, map2 f xs ys))
		| _ -> failwith "map2"
		
	let rec combine s1 s2 =
		match s1,s2 with
		| lazy Nil, lazy Nil -> lazy Nil
		| lazy (Cons(x,xs)), lazy (Cons(y,ys)) ->
			lazy (Cons((x,y),combine xs ys))
		| _ -> failwith "combine"
		
	let rec split s =
		match s with
		| lazy Nil -> (lazy Nil, lazy Nil)
		| lazy (Cons((x,y),str)) ->
			let splitted = split str in
			(lazy (Cons(x, fst splitted)), lazy (Cons(y, snd splitted)))
	
	let rec filter pred s =
		match s with
		| lazy Nil -> lazy Nil
		| lazy (Cons(x,xs)) when (pred x) -> lazy (Cons(x, filter pred xs))
		| lazy (Cons(x,xs)) -> (filter pred xs)
	
	let rec find_opt pred l =
		match l with
		| lazy Nil -> None
		| lazy (Cons(x,xs)) when (pred x) -> Some(x)
		| lazy (Cons(x,xs)) -> find_opt pred xs

	let rec concat ss =
		match ss with
		| lazy Nil -> lazy Nil
		| lazy (Cons(s,ss)) -> s ++ (concat ss)
	
	let rec nth n l =
		match l with
		| lazy Nil -> failwith "nth"
		| lazy (Cons(x,xs)) when (n <= 0) -> x
		| lazy (Cons(x,xs)) -> nth (n-1) xs

	let rec take n s =
		match s with
		| lazy Nil -> lazy Nil
		| lazy (Cons(x,xs)) when (n <= 0) -> lazy Nil
		| lazy (Cons(x,xs)) -> lazy (Cons(x, take (n-1) xs))
		
	let rec drop n s =
		match s with
		| lazy Nil -> lazy Nil
		| lazy (Cons(x,xs)) when (n <= 0) -> lazy (Cons(x,xs))
		| lazy (Cons(x,xs)) -> drop (n-1) xs
		
	let rec list_of_stream s =
		match s with
		| lazy Nil -> []
		| lazy (Cons(x,xs)) -> x::(list_of_stream xs)
	
	let rec stream_of_list l =
		match l with
		| [] -> lazy Nil
		| x::xs -> lazy (Cons(x,stream_of_list xs))

;;
