package geometria;
import java.io.*;

import static java.lang.Math.*;

public class Punkt {
	
	// *****************
	// ===== POLA =====
	private double x, y;
	
	
	// *************************
	// ===== KONSTRUKTORY =====
	public Punkt (double x, double y) {
		this.x = x; this.y = y;
	}
	
	public Punkt (Punkt p, Wektor v) {
	// przesuwa punkt o wektor
		x = p.get_x() + v.dx;
		y = p.get_y() + v.dy;
	}
	
	public Punkt (Punkt p, double alfa) {
	// obraca punkt o kat alfa wokol punktu (0,0)
		x = p.get_x()*cos(alfa) - p.get_y()*sin(alfa);
		y = p.get_x()*sin(alfa) - p.get_y()*cos(alfa);
	}
	
	public Punkt (Punkt p, Punkt centrum, double alfa) {
	// obraca punkt wokol innego punktu o alfa
	// --> przesuwamy wszystko do (0,0), obracamy i wracamy	
		Wektor v = new Wektor(centrum);
		Wektor minv = Wektor.minus_wektor(v);
		Punkt przesuniety_razem_z_centrum = new Punkt(p,minv);
		Punkt obrocony_w_centrum = new Punkt(przesuniety_razem_z_centrum,alfa);
		
		// musimy przesunac ten punkt o wektor V
		x = obrocony_w_centrum.x + v.dx;
		y = obrocony_w_centrum.y + v.dy;
		
	}
	
	public Punkt (Punkt p, Prosta l) {
	// tworzy odbicie symetryczne wzgledem prostej
		
		Prosta prostopadla = Prosta.prostopadla_przechodzaca_przez_punkt(l,p);
		Punkt przeciecie = Prosta.przeciecie_sie_prostych(l,prostopadla);
		
		Wektor w = new Wektor(p,przeciecie);
		Wektor podwojny = new Wektor(w.dx*2,w.dy*2);
		
		Punkt odbity = new Punkt(p,podwojny);
		p.x = odbity.x;
		p.y = odbity.y;
	}

	
	
	// *******************
	// ===== METODY =====
	public double get_x() {
		return x;
	}
	
	public double get_y() {
		return y;
	}
	
	public void przesun(Wektor v) {
		x += v.dx;
		y += v.dy;
	}
	
	public void obroc(Punkt centrum, double alfa) {
		Punkt obrocony = new Punkt(this,centrum,alfa);
		this.x = obrocony.x;
		this.y = obrocony.y;
	}
	
	public void odbij(Prosta l) {
		Punkt odbity = new Punkt(this,l);
		this.x = odbity.x;
		this.y = odbity.y;
	}
	
	public String get_string () {
		return "Punkt("+x+","+y+")";
	}
	
	
	
	// *****************************
	// ===== METODY STATYCZNE =====
	static public double odl_punktow (Punkt p1, Punkt p2) {
		return sqrt( pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2) );
	}
	
	
}