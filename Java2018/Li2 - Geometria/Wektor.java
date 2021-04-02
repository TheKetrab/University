package geometria;
import java.io.*;

import static java.lang.Math.*;

public class Wektor {
	
	// *****************
	// ===== POLA =====
	public final double dx, dy;
	
	


	// *************************
	// ===== KONSTRUKTORY =====
	public Wektor(double dx, double dy) {
		this.dx = dx; this.dy = dy;
	}
	
	public Wektor(Punkt p) {
		dx = p.get_x();
		dy = p.get_y();
	}
	
	public Wektor(Punkt a, Punkt b) { // wektor AB
		dx = b.get_x()-a.get_x();
		dy = b.get_y()-a.get_y();
	}

	// *******************
	// ===== METODY =====	
	public double dlugosc() {
		return sqrt(dx*dx + dy*dy);
	}

	public String get_string () {
		return "Wektor["+dx+" , "+dy+"]";
	}

	// *****************************
	// ===== METODY STATYCZNE =====	
	public static Wektor zloz(Wektor w1, Wektor w2) {
		return new Wektor(w1.dx+w2.dx , w1.dy+w2.dy);
	}
	
	static public Wektor minus_wektor(Wektor v) {
		return new Wektor(-v.dx,-v.dy);
	}	

	
}