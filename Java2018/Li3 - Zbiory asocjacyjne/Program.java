import struktury.*;

import java.io.*;

/** Klasa program testuje działanie klas.
@author Bartłomiej Grochowski
@version 1.0
*/
public class Program {
	
	
	/** test zbioru na tablicy */
	public static void main(String args[]) {
		
		try {
			
			
			ZbiorNaTablicy znt = new ZbiorNaTablicy(2);
			
			Para p1 = new Para("Key1",1);
			System.out.println("p1 : " + p1);
			
			Para p2 = new Para("Key2",2);
			System.out.println("p1.equals(p2) : " + p1.equals(p2));
			System.out.println("p1.equals(p1) : " + p1.equals(p1));
			
			System.out.println("ZNT.ile() : " + znt.ile());
			znt.wstaw(p1);
			System.out.println("ZNT.ile() : " + znt.ile());
			znt.wstaw(p1);
			System.out.println("ZNT.ile() : " + znt.ile());
			
			Para p3 = new Para("Key3",3);
			// znt.wstaw(p3); //wstawienie pary do pelnego zbioru = wyjatek
			
			
			/** test zbioru na tablicy dynamicznej */
			ZbiorNaTablicyDynamicznej zntd = new ZbiorNaTablicyDynamicznej(2);
			System.out.println("zntd.get_rozmiar() : " + zntd.get_rozmiar());
			zntd.wstaw(p1);
			zntd.wstaw(p2);
			zntd.ustaw(p3); // ustaw działa tak jak wstaw, OK
			System.out.println("zntd.get_rozmiar() : " + zntd.get_rozmiar());
			
			System.out.println("Zbiór wygląda tak: " + zntd);
			
			System.out.println("Szukaj(Key2) : " + zntd.szukaj("Key2"));
			
			
			
		}
		catch (Exception e) {
			System.out.println("Zgłoszono wyjątek!"); // 
		}
		
		return;
	}
	
	
}