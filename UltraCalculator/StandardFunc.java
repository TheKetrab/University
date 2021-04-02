
import java.io.*;
import java.util.*;

//UWAGA - funkcje musza miec dlugosc >=2, duze litery A,B,C,... oznaczaja argumenty

public class StandardFunc extends Func implements Serializable {
	
	
	public StandardFunc(String n, String b, int a) {
		super(n,b,a);
	}

	public double gcd(double a, double b) {
		
		double c;
		while (b!=0) {

	        c = a%b;
	        a = b;
	        b = c;
		}

		return a;
	}
	
	public double lcm(double a, double b) {
		return (a*b)/gcd(a,b);
	}
	
	
	public double faculty(double n) {
		
		if(n<2) return 1;
		
		double res=1;
		for(int i=2; i<=n; i++)
			res *= i;
		
		return res;
	}
	
	public double apply_func(String[] args) {
		
		double[] val_args = new double[args.length];
		int n = args.length;
		
		for(int i=0; i<n; i++)
			val_args[i] = Double.parseDouble(args[i]);
		
		System.out.println("APPLY FUNC " + Name + " , arg: ;" + val_args[0] + ";");
		
		switch(Name) {
			
			case "ABS" :
				return Math.abs(val_args[0]);
			case "ACOS" :
				return Math.acos(val_args[0]);
			case "ASIN" :
				return Math.asin(val_args[0]);
			case "ATAN" :
				return Math.atan(val_args[0]);
			case "CBRT" :
				return Math.cbrt(val_args[0]);
			case "CEIL" :
				return Math.ceil(val_args[0]);
			case "COS" :
				return Math.cos(val_args[0]);
			case "EXP" :
				return Math.exp(val_args[0]);
			case "FLOOR" :
				return Math.floor(val_args[0]);
			case "LOG" :
				return Math.log(val_args[0]);
			case "LOG10" :
				return Math.log10(val_args[0]);
			case "MAX" :
				return Math.max(val_args[0],val_args[1]);
			case "MIN" :
				return Math.min(val_args[0],val_args[1]);
			case "POW" :
				return Math.pow(val_args[0],val_args[1]);
			case "ROUND" :
				return Math.round(val_args[0]);
			case "SIGNUM" :
				return Math.signum(val_args[0]);
			case "SIN" :
				return Math.sin(val_args[0]);
			case "SQRT" :
				return Math.sqrt(val_args[0]);
			case "TAN" :
				return Math.tan(val_args[0]);
			case "FACULTY" :
				return faculty(val_args[0]);
			case "LCM" :
				return lcm(val_args[0],val_args[1]);
			case "GCD" :
				return gcd(val_args[0],val_args[1]);
			
			
			default:
				return 0;
				
		}
		
		
	}

}