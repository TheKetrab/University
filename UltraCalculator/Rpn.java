
import java.io.*;
import java.util.*;

public abstract class Rpn {

	public enum CharType {
		SPACE,
		OPEN_BRACKET,
		CLOSE_BRACKET,
		COMMA,
		DOT,
		DIGIT,
		VARIABLE,
		OPERATOR,
		FUNC,
		UNKNOW
	}
	
	public static CharType type(char c) {
		
		if(c==' ')
			return CharType.SPACE;
		
		if(c==',')
			return CharType.COMMA;
		
		if(c=='.')
			return CharType.DOT;

		if(c=='(')
			return CharType.OPEN_BRACKET;
		
		if(c==')')
			return CharType.CLOSE_BRACKET;
		
		if (c>=48 && c<=57)
			return CharType.DIGIT;
		
		if (c>=65 && c<=90)
			return CharType.FUNC;
			
		if (c>=97 && c<=122)
			return CharType.VARIABLE;
		
		if (c=='+' || c=='-' || c=='*' || c=='/'
		 || c=='%' || c=='^')
			return CharType.OPERATOR;
		
		return CharType.UNKNOW;
		
	}
	
	
// ***** ***** ***** ***** ***** ***** ***** *****
// 				--- PARSE ---
// ***** ***** ***** ***** ***** ***** ***** *****

	// zamienia np 12a na 12 * a
	public static String parse(String input, Memory memory, String[] fun_params) {
		
		Boolean FirstCharTaken = false;
		
		System.out.println("BEDZIEMY PARSOWAC ----- ----- ---- >> ;" + input + ";");
		
		
		String result = "";
		char prev_char = '\0';
		for (int i=0; i<input.length(); i++) {
			char got_char = input.charAt(i);
		
			switch(type(got_char)) {
				
				case DIGIT :
					
					FirstCharTaken = true;
					if(type(prev_char) == CharType.DIGIT
					||(type(prev_char) == CharType.DOT))
						result += got_char;
					else
						result += (" " + got_char);
					break;
					
				case DOT :
					
					FirstCharTaken = true;
					if(type(prev_char) == CharType.DIGIT)
						result += got_char;
					else
						result += (""); // ignoruj kropke
					break;

				case VARIABLE :
					
					FirstCharTaken = true;
					if((type(prev_char) == CharType.DIGIT)
					 ||(type(prev_char) == CharType.VARIABLE))
						result += (" * " + got_char);
					else
						result += (" " + got_char);
					break;

				case OPERATOR :
					
					System.out.println("POBRANO ZNAK: " + Character.toString(got_char));
					if(Character.toString(got_char).equals("-") && FirstCharTaken==false ) {
						result += (" 0 -");
						FirstCharTaken = true;
					}
					else {
						FirstCharTaken = true;
						result += (" " + got_char);
					}
					
					break;

				case FUNC :
				
					// TO JEST FUNKCJA
					
					FirstCharTaken = true;
					
					if(i+1<input.length() && type(input.charAt(i+1)) == CharType.FUNC) {

						result += (" " + "(");
						
						// wyciagnij nazwe funkcji
						i++; // nastepna litera
						String name = Character.toString(got_char);
						for( ; (type(input.charAt(i)) != CharType.OPEN_BRACKET) ; i++ ) {
							if(type(input.charAt(i)) != CharType.SPACE) // w nazwie funkcje moga byc rozne znaki, ale nie moga byc spacje!
								name += input.charAt(i);
						}
						
						i++; // przeskoczenie nawiasu
						Func f = memory.find_func(name);
						if(f==null) f = memory.find_standardfunc(name);
						String[] args = new String[f.Arity];
						
						// wszystkie argumenty to puste Stringi
						for(int j=0; j<f.Arity; j++)
							args[j]="";

						
						// zainicjuj argumenty
						int fun_inside_counter=0; // sprawdza wewnetrzne funkcje
						for(int j=0; true ; i++)
							if(type(input.charAt(i)) == CharType.OPEN_BRACKET) {
								fun_inside_counter++; args[j] += input.charAt(i); }
							else if(type(input.charAt(i)) == CharType.CLOSE_BRACKET && fun_inside_counter!=0) { fun_inside_counter--; args[j] += input.charAt(i); }
							else if(type(input.charAt(i)) == CharType.CLOSE_BRACKET && fun_inside_counter==0) { break; }
							else if(type(input.charAt(i)) == CharType.COMMA && fun_inside_counter==0) j++; // jesli przecinek to zwieksz iterator argumentow
							else args[j] += input.charAt(i); // wpp. powieksz argument
							
						
						/*System.out.println("ZAINICJOWANE ARGUMENTY:");
						int k=0;
						while(k<f.Arity) {
							System.out.println(args[k]);
							k++;
						}*/

						// parsuj argumenty
						for(int j=0; j<f.Arity; j++) {
							System.out.println("bedziemy parsowac: ;" + args[j] + ";");
							args[j] = parse(args[j],memory,fun_params);
						}
						
						/*System.out.println("SPARSOWANE ARGUMENTY:");
						k=0;
						while(k<f.Arity) {
							System.out.println(args[k]);
							k++;
						}*/


						// wstaw cialo
						
						if(f instanceof StandardFunc) {
							
							result += (" " + f.Name);
							System.out.println(f.Arity);
							String[] empty = new String[0];
							for(int x=0; x<f.Arity; x++) {
								result += (" (" + args[x] + " )");
							
							}
						}

						else {
						
							String b = parse(f.Body,memory,args);
							result += b;
							
						}
							result += (" " + ")");
						
						got_char=')';
						break;

					}
					
					// TO JEST ARGUMENTEM
					else {
						
							//if(type(b.charAt(j)) == CharType.FUNC)
								result += (" ( " + fun_params[(input.charAt(i) - 'A')] + " )");
							//else
							//	result += (" " + b.charAt(j));
						
						
						break;
						
					}



					case OPEN_BRACKET :
					
						FirstCharTaken = true;
						
						if((type(prev_char) == CharType.DIGIT)
						||(type(prev_char) == CharType.VARIABLE)
						||(type(prev_char) == CharType.CLOSE_BRACKET))
							result += (" * " + got_char);
						else if(type(prev_char)	== CharType.FUNC)
							result += got_char;
						else if((type(prev_char) == CharType.OPERATOR)
							  ||(type(prev_char) == CharType.OPEN_BRACKET))
							result += (" " + got_char);
						
						
						else
							result += (" " + got_char); //czyli np byly dwie spacje?
						break;
					
					case CLOSE_BRACKET :
					
						FirstCharTaken = true;
						
						result += (" " + got_char);
						break;
				
				
				default :
					
					// ignoruj znak
					break;
				
				
			}
			
			prev_char = got_char;
		}
			
			
			
		System.out.println("PARSE ZWRACA: ;" + result + ";");
		
		return result;
	}
	
	
	private static Queue<String> parsed_to_queue(String p) {
		
		System.out.println("PARSED TO QUEUE: ;" + p + ";");
		Queue<String> q = new LinkedList<String>();
		int n = p.length();
		
		for(int i=0; i<n; i++) {
			String arg = "";
			for( ; i<n; i++) {
				if(p.charAt(i)==' ') break;
				arg += p.charAt(i);
				System.out.println("arg: ;" + arg + ";");
			}
			
			if(arg!="") // jesli nie jest pusty
				q.offer(arg);
		}
		return q;
	}
	
	private static String queue_to_string(Queue<String> q) {
		
		String r = "";
		while(q.peek() != null)
			r += (q.poll() + " ");
		
		return r;
	}
	
	private static String insert_var_meaning(String input,Memory mem) {
		
		// bierze sparsowane wyrazenie
		String r = "";
		int n=input.length();
		for(int i=0; i<n; i++) {
			if(type(input.charAt(i)) == CharType.VARIABLE) {
				String var_string = "" + input.charAt(i);
				Var v = mem.find_var(var_string);
				String[] empty = new String[0];
				r += " ( ";
				r += parse(v.Meaning,mem,empty);
				r+= " ) ";
			}
			else {
				
					r += input.charAt(i);

			}
		}
			
		
		return r;
	}

	public static String normal_to_rpn(String input,Memory mem) {

		String[] empty = new String[0];
		Queue<String> q = parsed_to_queue(insert_var_meaning(input,mem));
		Stack<String> s_op = new Stack<String>(); // stos operatorów
		// w przypadku natrafienia na funkcje: ( FUN ( ARG1 ) ... ( ARGN ) )
		// zamien na: | onp(ARG1) | ... | onp(ARGN) | FUN
		// i wtedy osobno policz wartosc dla kazdego, zapamietaj je, i naloz na to funkcje
		
		String result = "";
		
		while(q.peek() != null) {
			String arg = q.poll();
			System.out.println("TEST --> arg: ;" + arg + ";");
			result += " ";
			switch(type(arg.charAt(0))) {
				
				case OPEN_BRACKET :
					s_op.push(arg);
					
					break;
				case CLOSE_BRACKET :
					System.out.println("WRITE STACK:");
					s_op.write();
					System.out.println("res now:" + result);
					System.out.println("--------------------");
					// Zdejmujemy dopoki nie bedzie otwierajacego
					while(true) {
						String got_from_stack = s_op.pop();
						if(got_from_stack.equals("("))
							break;
						else
							result += (" " + got_from_stack);
					}
				
					break;
				case DIGIT :
					result += arg;
					
					break;
				case OPERATOR :

					Operator op_actual = mem.find_op(arg); //o1
					while(true) {
						
						System.out.println("TU JESTEM 1");
						if(s_op.empty() == true) {
							s_op.push(op_actual.Symbol);
							break;
							
						}
						if(s_op.top().equals("(")) {
							s_op.push(op_actual.Symbol);
							break;
							
						}
						
						Operator op_on_top = mem.find_op(s_op.top()); //o2
						if (((op_actual.Left_Associative == true)
						    && (op_actual.Priority <= op_on_top.Priority))
						 || ((op_actual.Left_Associative == false)
						   &&(op_actual.Priority < op_on_top.Priority))) {
							   
							   result += (" " + s_op.pop());
							   
						   }
						else {  
							s_op.push(op_actual.Symbol);
							break;
						
						}
						System.out.println("TU JESTEM 1");
					}
						

					break;
				case FUNC :
				
					StandardFunc sf = mem.find_standardfunc(arg);
					//musimy pobrac jej argumenty, przerobic na onp i dokleic do wyjscia
					//result+="|";
					int bracket_counter=0;
					int arg_done=0;
					String fun_arg = "";
					while(arg_done<sf.Arity) { // dopoki nie zainicjowalismy wszystkich argumentow
						
						System.out.println("Siema, wywolalem sie z funkcja , arnosc , arg_done: ;" + sf.Name + "; " + sf.Arity + " " + arg_done);
						
						String got = q.poll();
						System.out.println("POBIERAM ZNAK: pobrano: ;" + got + ";");
						if(got.equals("(")) { bracket_counter++; fun_arg += (" " + got); }
						else if(got.equals(")")) { bracket_counter--; fun_arg += (" " + got); }
						else fun_arg += (" " + got);
						
						// jesli po przejsciu tego counter sie zgadza, to argument gotowy
						if(bracket_counter == 0) {
							System.out.println("LOG --------> fun_arg: " + fun_arg);
							result += normal_to_rpn(fun_arg,mem);
							//result += " |"; // <-- oddzielacz
							fun_arg = "";
							arg_done++;
							System.out.println("LOG --------> arg_done: " + arg_done);
						}
					}
					
					result += (" " + sf.Name);
					
					break;
				default:
				
					break;
			}
			
		}
			
		//zdejmujemy operatory do konca stosu
		while(s_op.empty() == false) {
			
			result += (" " + s_op.pop());
			
		}
		

		return result;
	}
	
	public static double rpn_to_result(String input, Memory mem) {

		Queue<String> q = parsed_to_queue(input); // rpn tez jest w wersji sparsowanej
		Stack<String> stack = new Stack<String>();
		
		while(q.peek() != null) {// dopoki cos jest do wziecia
		
			stack.write();
			String taken = q.poll();
			if(type(taken.charAt(0)) == CharType.DIGIT) {
				stack.push(taken);
			}

			else if(type(taken.charAt(0)) == CharType.OPERATOR) {
				String arg1 = stack.pop();
				String arg2 = stack.pop();
				System.out.println("Raport: taken, arg2, arg1 ;" + taken + "; ;" + arg2 + "; ;" + arg1 + ";");
				double value_op = Operator.apply_op(taken,arg2,arg1); // wazne zeby taka kolejnosc
				stack.push(Double.toString(value_op));
			}
			
			else if(type(taken.charAt(0)) == CharType.FUNC) {
				StandardFunc sf = mem.find_standardfunc(taken);
				int n = sf.Arity;
				String[] args = new String[n];
				
				for(int i=n-1; i>=0; i--) {
					args[i] = stack.pop(); // <-- inicjujemy argumenty od tylu!
					System.out.println("Zainicjowano argument ;" + args[i] + ";");
				}
				
				double value_standardfunc = sf.apply_func(args);
				stack.push(Double.toString(value_standardfunc));
				
				
			}
		
		}
		
		stack.write();
		double result = Double.parseDouble(stack.pop());
		System.out.println("Wynik calego algorytmu: ;" + result + ";");
		
		return result;
		
		
	}
	
	
	public static double expr_to_result(String input, Memory mem) {
		String[] empty = new String[0];
		String parsed = parse(input,mem,empty);
		String rpned = normal_to_rpn(parsed,mem);
		double done = rpn_to_result(rpned,mem);
		
		return done;
	}

}
