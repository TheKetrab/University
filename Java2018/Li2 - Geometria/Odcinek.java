package geometria;
import java.io.*;


public class Odcinek {
	
	// *****************
	// ===== POLA =====
	private Punkt p1, p2;
		
	

	// *************************
	// ===== KONSTRUKTORY =====
	public Odcinek(Punkt p1, Punkt p2) throws IllegalArgumentException {

		if (p1.get_x() == p2.get_x() && p1.get_y() == p2.get_y())
			throw new IllegalArgumentException("Podano identyczne punkty przy tworzeniu odcinka");
		
		this.p1 = p1; this.p2 = p2;
	}	


	// *******************
	// ===== METODY =====	
	public void przesun(Wektor v) {
		
		p1.przesun(v);
		p2.przesun(v);
	}
	
	public void obroc(Punkt p, double alfa) {
		
		p1.obroc(p, alfa);
		p2.obroc(p, alfa);
	}
	
	public void odbij(Prosta l) {
		
		p1.odbij(l);
		p2.odbij(l);
	}
		
	public String get_string () {
		return "Odcinek[ "+p1.get_string()+" , "+p2.get_string()+" ]";
	}
	
	
}