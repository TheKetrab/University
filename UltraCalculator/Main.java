
import java.io.*;
import java.util.*;


public class Main {

    public static void main(String[] args) {
        
		Global g = new Global();
		
		String[] empty = new String[0];
		
		//System.out.print(Rpn.parse("3+TRIPPLE(5,SQUARE(4),ADD(5,55))",g.mem,empty));
		//System.out.print(Rpn.parse("3+SUM_OF_SQUARES(ADD(4,5),SIN(5+MIN(2,193*1)))",g.mem,empty));
		
		//System.out.print(Rpn.parse("MIN(3+11*2,6*SIN(44))*13",g.mem,empty));
		//System.out.print(Rpn.parse("MIN(3+11*2,INC(44))",g.mem,empty));
		//System.out.print(Rpn.parse("SIN(44)",g.mem,empty));
		//System.out.print(Rpn.normal_to_rpn("( 2 + 3 ) * 44 + 11",g.mem));
		
		//System.out.print(Rpn.normal_to_rpn("3+SUM_OF_SQUARES(ADD(4,5),SIN(5+MIN(2,193*1)))",g.mem));
		//System.out.print(Rpn.rpn_to_result(Rpn.normal_to_rpn(Rpn.parse("3+SUM_OF_SQUARES(ADD(4,5),SIN(5+MIN(2,193*1)))",g.mem,empty),g.mem),g.mem)); <-- nie dziala
		// nie dziala System.out.print(Rpn.rpn_to_result("3     4  5  +   4  5  +  *      5     2   193  1  * MIN   + SIN      5     2   193  1  * MIN   + SIN   *  + +",g.mem));
		System.out.print(Rpn.expr_to_result("MIN(3+11*2,6*SIN(44))*13",g.mem));
		
		
		//System.out.print(Rpn.normal_to_rpn(Rpn.parse("SIN(a+66)",g.mem,empty),g.mem));
		//System.out.print(Rpn.parse("3+SUM_OF_SQUARES(ADD(4,5),SIN(5+MIN(2,193*1)))",g.mem,empty));
		
		
	
/*	
1+3*(4+2*3) ONP: 13423*+*+
3*(9-2*4+1)-2 ONP: 3924*-1+*2-
6+3*2*(7-4)  ONP: 632*74-*+
(2+6)*(9-8+4)-3 ONP: 26+98-4+*3-
((2+5)-(3-1))*3+4 ONP: 25+31--3*4+
	*/
	
	
	}

}
