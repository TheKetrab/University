
import java.io.*;
import java.util.*;


public class Operator implements Serializable {

	public String Symbol; // znak
	public Boolean Left_Associative; // lewostronnie laczne?
	public int Priority;
	public int Arity; // arnosc
	
	public Operator(String s, Boolean l, int p, int a) {
		Symbol = s; Left_Associative = l; Priority = p; Arity = a;
	}
	
	public static double apply_op(String op, String arg1, String arg2) {
		
		double v1 = Double.parseDouble(arg1);
		double v2 = Double.parseDouble(arg2);
		
		switch(op) {
			
			case "+" :
				return v1 + v2;
			case "-" :
				return v1 - v2;
			case "*" :
				return v1 * v2;
			case "/" :
				return v1 / v2;
			case "^" :
				return Math.pow(v1,v2);
			case "%" :
				return v1 % v2;
			default :
				return 0;
		}
	}
		
}

