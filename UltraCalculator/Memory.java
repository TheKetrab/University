
import java.io.*;
import java.util.*;

public class Memory implements Serializable {

	private List<Operator> ops;
	public List<Func> funcs;
	private List<StandardFunc> standardfuncs;
	public List<Var> vars;

	public Memory() {
		
		ops = new ArrayList<Operator>();
		funcs = new ArrayList<Func>();
		standardfuncs = new ArrayList<StandardFunc>();
		vars = new ArrayList<Var>();
		
		// inicjacja zmiennych
		vars.add(new Var("a","(2+2)"));
		vars.add(new Var("b","4*7"));
		vars.add(new Var("c","7^11"));
		vars.add(new Var("d","a+5"));

		// inicjacja operatorow
		ops.add(new Operator("+",true,1,2));
		ops.add(new Operator("-",true,1,2));
		ops.add(new Operator("*",true,2,2));
		ops.add(new Operator("/",true,2,2));
		ops.add(new Operator("^",true,3,2));
		ops.add(new Operator("%",true,3,2));
		
		// inicjacja standardowych funkcji
		standardfuncs.add(new StandardFunc("ABS","",1));
		standardfuncs.add(new StandardFunc("ASIN","",1));
		standardfuncs.add(new StandardFunc("ATAN","",1));
		standardfuncs.add(new StandardFunc("CBRT","",1));
		standardfuncs.add(new StandardFunc("CEIL","",1));
		standardfuncs.add(new StandardFunc("COS","",1));
		standardfuncs.add(new StandardFunc("EXP","",1));
		standardfuncs.add(new StandardFunc("FLOOR","",1));
		standardfuncs.add(new StandardFunc("LOG","",1));
		standardfuncs.add(new StandardFunc("LOG10","",1));
		standardfuncs.add(new StandardFunc("MAX","",2));
		standardfuncs.add(new StandardFunc("MIN","",2));
		standardfuncs.add(new StandardFunc("POW","",2));
		standardfuncs.add(new StandardFunc("ROUND","",1));
		standardfuncs.add(new StandardFunc("SIGNUM","",1));
		standardfuncs.add(new StandardFunc("SIN","",1));
		standardfuncs.add(new StandardFunc("SQRT","",1));
		standardfuncs.add(new StandardFunc("TAN","",1));
		standardfuncs.add(new StandardFunc("FACULTY","",1));	// silnia
		standardfuncs.add(new StandardFunc("LCM","",2)); 		// NWW
		standardfuncs.add(new StandardFunc("GCD","",2)); 		// NWD
		
		// inicjacja wlasnych funkcji		
		funcs.add(new Func("INC","A+1",1));
		funcs.add(new Func("SQUARE","A*A",1));
		funcs.add(new Func("IDENTITY","A",1));
		funcs.add(new Func("AVG","(A+B)/2",2));

	}

	public void write_ops() {
		
		int siz = ops.size();
		for(int i=0; i<siz; i++) {
			Operator temp = ops.get(i);
			System.out.print(temp.Symbol + " " + temp.Left_Associative + " ");
			System.out.print(temp.Priority + " " + temp.Arity + "\n");
		}
		
	}

	public Operator find_op(String s) {
		
		System.out.println("Szukam operatora: ;" + s + ";");
		int siz = ops.size();
		for(int i=0; i<siz; i++) {
			Operator temp = ops.get(i);
			System.out.println("act op: ;" + temp.Symbol + "; " + (temp.Symbol.equals(s)));
		
			if(temp.Symbol.equals(s)) return temp;		
		}
		
		System.out.println("NIE ZNALAZLEM");
		return ops.get(0); // rzucic wyjatek albo zwrocic nulla?
	}
	

	public Func find_func(String s) {
		
		System.out.println("SZUKAM FUNKCJI: ;" + s + ";");
		int siz = funcs.size();
		for(int i=0; i<siz; i++) {
			Func temp = funcs.get(i);
			if(temp.Name.equals(s)) return temp;		
		}
		
		System.out.println("NIE ZNALAZLEM");
		return null; // rzucic wyjatek albo zwrocic nulla?
	}

	public StandardFunc find_standardfunc(String s) {
		
		System.out.println("SZUKAM FUNKCJI: ;" + s + ";");
		int siz = standardfuncs.size();
		for(int i=0; i<siz; i++) {
			StandardFunc temp = standardfuncs.get(i);
			if(temp.Name.equals(s)) return temp;		
		}
		
		System.out.println("NIE ZNALAZLEM");
		return standardfuncs.get(0); // rzucic wyjatek albo zwrocic nulla?
	}

	public Var find_var(String s) {
		
		System.out.println("SZUKAM ZMIENNEJ: ;" + s + ";");
		int siz = vars.size();
		for(int i=0; i<siz; i++) {
			Var temp = vars.get(i);
			if(temp.Letter.equals(s)) return temp;		
		}
		
		System.out.println("NIE ZNALAZLEM");
		return vars.get(0); // rzucic wyjatek albo zwrocic nulla?
	}

	public void add_var(Var v) {
		vars.add(v);
	}
	
	public void add_func(Func f) {
		funcs.add(f);
	}
	
	public void remove_var(String s) {
		
		int position = 0;
		Var temp;
		for(int i=0; i<vars.size(); i++) {
			temp = vars.get(i);
			if(temp.Letter.equals(s)) break;
			position++;
		}
		
		vars.remove(position);
	}
	
	public void remove_func(String s) {
		
		int position = 0;
		Func temp;
		for(int i=0; i<funcs.size(); i++) {
			temp = funcs.get(i);
			if(temp.Name.equals(s)) break;
			position++;
		}
		
		funcs.remove(position);
	}
	
	
	
	
	
}
