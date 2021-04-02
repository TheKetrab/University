package struktury;

import java.io.*;


/** Klasa przechowująca klucz i wartość.
@author Bartłomiej Grochowski
@version 1.0
*/
public class Para {
	
	/** Klucz identyfikujący wartość. */
	public final String klucz;
	private double wartosc;

	/** W konstruktorze należy podać klucz i wartość. */
	public Para (String klucz, double wartosc) throws IllegalArgumentException {
		if (klucz == null || klucz.equals(""))
			throw new IllegalArgumentException("Nie podano klucza pary.");
		
		this.klucz = klucz;
		this.wartosc = wartosc;
	}
	
	/** Zwraca wartość. */
	public double get_wartosc() {
		return wartosc;
	}
	
	/** Ustawia wartość. */
	public void set_wartosc(double wartosc) {
		this.wartosc = wartosc;
	}
	
	
	/** Zwraca wartość jako String. */
	public String toString() {
		return Double.toString(wartosc);
	}
	
	/** Dwa obiekty są równe, jeśli mają takie same klucze. */
	public boolean equals(Object obj) { // TODO: spr czy ok
		if (obj instanceof Para) {
			Para porownywana = (Para)obj;
			return klucz.equals( porownywana.wartosc );
		}
		
		return false;
	}
}