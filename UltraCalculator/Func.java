
import java.io.*;
import java.util.*;

//UWAGA - funkcje musza miec dlugosc >=2, duze litery A,B,C,... oznaczaja argumenty
public class Func implements Serializable {

	public String Name;
	public String Body;
	public int Arity;
	
	public Func(String n, String b, int a) {
		Name = n; Body = b; Arity = a;
	}
	
	
}

