package geometria;
import java.io.*;


public class Trojkat {
	
	// *****************
	// ===== POLA =====
	private Punkt p1, p2, p3;


	// *************************
	// ===== KONSTRUKTORY =====
	public Trojkat(Punkt p1, Punkt p2, Punkt p3) throws IllegalArgumentException {
		
		double p1_p2 = Punkt.odl_punktow(p1,p2);
		double p1_p3 = Punkt.odl_punktow(p1,p3);
		double p2_p3 = Punkt.odl_punktow(p2,p3);
		
		if ((p1_p2 + p1_p3 <= p2_p3)
		 || (p1_p2 + p2_p3 <= p1_p3)
		 || (p1_p3 + p2_p3 <= p1_p2))
			throw new IllegalArgumentException("Podane punkty nie spełniają nierówności trójkąta.");
		
		this.p1 = p1; this.p2 = p2; this.p3 = p3;
	}	


	// *******************
	// ===== METODY =====	
	public void przesun (Wektor v) {
		p1.przesun(v);
		p2.przesun(v);
		p3.przesun(v);
	}
	
	public void obroc (Punkt p, double alfa) {
	// obraca figure o alfa wokol punktu p
		p1.obroc(p,alfa);
		p2.obroc(p,alfa);
		p3.obroc(p,alfa);	
	}
	
	public void odbij (Prosta l) {
		
		p1.odbij(l);
		p2.odbij(l);
		p3.odbij(l);
		
	}
	
	public String get_string () {
		return "Trojkat[ "+p1.get_string()+" , "+p2.get_string()+" , "+p3.get_string()+" ]";
	}

	

}