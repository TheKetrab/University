(* Bartłomiej Grochowski 300951 *)
(* Main.ml *)

open DeBruijn
open Parser

exception BadInput of string

let read_file f =
  let ic = open_in f in
  let rec build_list l =
    match input_line ic with
    | line -> build_list (line :: l)
    | exception End_of_file -> close_in ic; List.rev l
  in build_list []

let main =

  try

    (* SCAN INPUT *)
    let argc = Array.length Sys.argv in
    if (argc < 3) then raise (BadInput "no args enought") else
    let input_lst = read_file Sys.argv.(1) in
    let oc = open_out Sys.argv.(2) in
  
    let mode = 
      if (argc < 4) then "normalize" else
        (match Sys.argv.(3) with
         | "n"         -> "normalize"
         | "normalize" -> "normalize"
         | "c"         -> "compare"
         | "compare"   -> "compare"
         | _ -> raise (BadInput "wrong 2nd argument!"))
    in

    if (mode = "normalize" && (List.length input_lst <> 1))
      then raise (BadInput "while normalize-mode input file must consist of 1 line") else
    
    if (mode = "compare" && (List.length input_lst <> 2))
      then raise (BadInput "while compare-mode input file must consist of 2 lines") else

    let eval =
      if (argc < 5) then "cbn" else
        (match Sys.argv.(4) with
         | "cbn" -> "cbn"
         | "cbv" -> "cbv"
         | _ -> raise (BadInput "wrong 3rd argument!"))
    in

    (* DO WORK *)
    let result =
      match mode, eval with
      | "normalize", "cbn" ->
          let t = parse (List.nth input_lst 0) in
          let t' = reduce_cbv t in
          concrete_of_abstract t'
      | "normalize", "cbv" ->
          let t = parse (List.nth input_lst 0) in
          let t' = reduce_cbv t in
          concrete_of_abstract t'
      | "compare", "cbn" ->
          let t1 = parse (List.nth input_lst 0) in
          let t2 = parse (List.nth input_lst 1) in
          if (betaequal t1 t2) then "true" else "false"
      | "compare", "cbv" ->
          let t1 = parse (List.nth input_lst 0) in
          let t2 = parse (List.nth input_lst 1) in
          if (betaequal_cbv t1 t2) then "true" else "false"
      | _ ->
          raise (BadInput "wrong input")
    in

    (* WRITE RESULT *)
    Printf.fprintf oc "%s\n" result;
    close_out oc;

  with e -> print_string (Printf.sprintf "\n%s\n%s\n" (Printexc.to_string e)
      "Run program with: ./prog IN_FILE OUT_FILE [n|normalize=default|c|compare] [cbn=default|cbv]")

