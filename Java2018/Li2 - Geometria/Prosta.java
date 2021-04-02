package geometria;
import java.io.*;

import static java.lang.Math.*;

public class Prosta {
	
	// *****************
	// ===== POLA =====
	public final double a, b, c;
	

	// *************************
	// ===== KONSTRUKTORY =====
	public Prosta(double a, double b, double c) {
		this.a = a;
		this.b = b;
		this.c = c;
	};


	// *******************
	// ===== METODY =====	
	public double get_a() {
		return a;
	}
	
	public double get_b() {
		return b;
	}
	
	public double get_c() {
		return c;
	}
	
	public String get_string () {
		return "Prosta[ A="+a+", B="+b+", C="+c+" ]";
	}

	
	// *****************************
	// ===== METODY STATYCZNE =====	
	public static Prosta przesun(Prosta p, Wektor v) {
		// rownanie nowej prostej: Ax + By + (C-Ap-Bq) = 0
		double new_c = p.c - (p.a * v.dx) - (p.b * v.dy);
		return new Prosta(p.a,p.b,new_c);
	}
	
	public static Prosta prostopadla_przechodzaca_przez_punkt(Prosta l, Punkt p) {
		// prostopadle gdy A1*A2= -B1*B2
		// jesli A2 = 1, to B2 = -A1/B1
		return new Prosta(1,													// A
						 -1 * (l.get_a() / l.get_b()),							// B
						 -1 * (l.get_a()*p.get_x() + l.get_b()*p.get_y()));		// C
		
		
	}
	
	
	public static boolean czy_proste_prostopadle(Prosta l, Prosta m) {
		//A1*A2 + B1*B2 = 0
		if(l.get_a() * m.get_a() + l.get_b() * m.get_b() == 0) return true;
		return false;
	}

	public static boolean czy_proste_rownolegle(Prosta l, Prosta m) {
		//A1*B2 - A2*B1 = 0
		if(l.get_a() * m.get_b() - m.get_a() * l.get_b() == 0) return true;
		return false;
	}

	public static Punkt przeciecie_sie_prostych(Prosta l, Prosta m) throws IllegalArgumentException {
		double d = l.get_a() * m.get_b() - m.get_a() * l.get_b();

		if(d==0) throw new IllegalArgumentException("Próbujesz znaleźć punkt przecięcia równoleglych prostych.");

		double x = ( l.get_b() * m.get_c() - m.get_b() * l.get_c() ) / d;
		double y = (-1.0) * ( l.get_a() * m.get_c() - m.get_a() * l.get_c() ) / d;

		Punkt p = new Punkt(x,y);

		return p;
	}
	
	public static double odl_punktu_od_prostej(Punkt p, Prosta l) {
		
	    double licznik = abs( l.get_a()*p.get_x() + l.get_b()*p.get_y() + l.get_c() );
		double mianownik = sqrt( l.get_a()*l.get_a() + l.get_b()*l.get_b() );
	
		return ( licznik / mianownik );

	}
	
}