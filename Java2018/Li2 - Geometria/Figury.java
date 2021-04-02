package geometria;

import java.io.*;

public class Figury {
	
	// *****************
	// ===== POLA =====
	private Punkt tab_punktow[];
	private Wektor tab_wektorow[];
	private Prosta tab_prostych[];
	private Trojkat tab_trojkatow[];
	private Odcinek tab_odcinkow[];
	
	private int ilosc_punktow;
	private int ilosc_wektorow;
	private int ilosc_prostych;
	private int ilosc_trojkatow;
	private int ilosc_odcinkow;
	
	private final int ROZMIAR = 25;
		
	


	// *************************
	// ===== KONSTRUKTORY =====
	public Figury() {
		
		// inicjowanie pamieci globalnej
		tab_punktow = new Punkt[ROZMIAR];
		tab_wektorow = new Wektor[ROZMIAR];
		tab_prostych = new Prosta[ROZMIAR];
		tab_trojkatow = new Trojkat[ROZMIAR];
		tab_odcinkow = new Odcinek[ROZMIAR];

		ilosc_punktow=0;
		ilosc_wektorow=0;
		ilosc_prostych=0;
		ilosc_trojkatow=0;
		ilosc_odcinkow=0;
		
	}


	// *******************
	// ===== METODY =====	
	private int input() throws IOException {
		try {
			InputStreamReader isr = new InputStreamReader(System.in);
			BufferedReader br = new BufferedReader(isr);
		
			return Integer.parseInt(br.readLine());
		}
		catch (IOException i) {}
		return 0;
	}
	
	private void wybor_glowny() throws IOException {
		System.out.println(" --- --- --- --- --- ");
		System.out.println("Co chcesz zrobić?");
		System.out.println("1. Dodać obiekt.");
		System.out.println("2. Wywołać funkcję.");
		System.out.println("3. Wypisać bazę danych.");
		System.out.println("4. Zakończyć program.");
		
		int wybor = input();
		
		switch(wybor) {

		case 1:
			wybor_dodawanie_obiektu();
			break;
		case 2:
			wybor_wywolanie_funkcji();
			break;
		case 3:
			wypisz_baze_danych();
			wybor_glowny();
			break;

		default:
			break;
		}
	}

	private void wybor_dodawanie_obiektu() throws IOException {
		System.out.println(" --- --- --- --- --- ");
		System.out.println("Jaki obiekt chcesz dodać?");
		System.out.println("1. Punkt (x,y).");
		System.out.println("2. Wektor (dx,dy).");
		System.out.println("3. Wektor (W o V");
		System.out.println("4. Prosta (a,b,c).");
		System.out.println("5. Trojkat (p1,p2,p3).");
		System.out.println("6. Odcinek (p1, p2).");
		int wybor = input();

		wypisz_baze_danych();
		System.out.println("Podaj odpowiednie parametry oddzielając klawiszem ENTER.");

		switch(wybor) {

			case 1:
			{
				double x = input();
				double y = input();
				Punkt p = new Punkt(x,y);
				tab_punktow[ilosc_punktow]=p;
				ilosc_punktow++;
				break;
			}

			case 2:
			{
				double dx = input();
				double dy = input();
				Wektor v = new Wektor(dx,dy);
				tab_wektorow[ilosc_wektorow]=v;
				ilosc_wektorow++;
				break;
			}

			case 3:
			{
				int id_u = input();
				int id_v = input();

				Wektor w = Wektor.zloz(tab_wektorow[id_u],tab_wektorow[id_v]);
				tab_wektorow[ilosc_wektorow]=w;
				ilosc_wektorow++;
				break;
			}

			case 4:
			{
				double a = input();
				double b = input();
				double c = input();
				Prosta l = new Prosta(a,b,c);
				tab_prostych[ilosc_prostych]=l;
				ilosc_prostych++;
				break;
			}


			case 5:
			{
				int id_p1 = input();
				int id_p2 = input();
				int id_p3 = input();
				Trojkat t = new Trojkat(tab_punktow[id_p1],tab_punktow[id_p2],tab_punktow[id_p3]);
				tab_trojkatow[ilosc_trojkatow]=t;
				ilosc_trojkatow++;
				break;
			}

			case 6:
			{
				int id_p1 = input();
				int id_p2 = input();
				Odcinek o = new Odcinek(tab_punktow[id_p1],tab_punktow[id_p2]);
				tab_odcinkow[ilosc_odcinkow]=o;
				ilosc_odcinkow++;
				break;
			}


		}

		System.out.println("Twoja baza danych została uaktualniona!");
		wybor_glowny();
	}


	private void wypisz_baze_danych()
	{
		System.out.println(" --- --- --- --- --- ");
		System.out.println("Baza danych:");
		
		for(int i=0; tab_punktow[i]!=null && i<ROZMIAR; i++)
			System.out.println("Pkt "+i+" : " + wypisz_dane_punktu(i));

		for(int i=0; tab_wektorow[i]!=null && i<ROZMIAR; i++)
			System.out.println("Wek "+i+" : " + wypisz_dane_wektora(i));

		for(int i=0; tab_prostych[i]!=null && i<ROZMIAR; i++)
			System.out.println("Pro "+i+" : " + wypisz_dane_prostej(i));
		
		for(int i=0; tab_trojkatow[i]!=null && i<ROZMIAR; i++)
			System.out.println("Tro "+i+" : " + wypisz_dane_trojkata(i));
		
		for(int i=0; tab_odcinkow[i]!=null && i<ROZMIAR; i++)
			System.out.println("Odc "+i+" : " + wypisz_dane_odcinka(i));
		

	}

	private String wypisz_dane_punktu(int id) {
		return tab_punktow[id].get_string();
	}

	private String wypisz_dane_wektora(int id) {
		return tab_wektorow[id].get_string();
	}

	private String wypisz_dane_prostej(int id) {
		return tab_prostych[id].get_string();
	}

	private String wypisz_dane_trojkata(int id) {
		return tab_trojkatow[id].get_string();
	}

	private String wypisz_dane_odcinka(int id) {
		return tab_odcinkow[id].get_string();
	}


	private void wybor_wywolanie_funkcji() throws IOException {
		System.out.println(" --- --- --- --- --- ");
		System.out.println("Jaką funkcję chcesz wywołać?");
		System.out.println("1. Przesuń punkt p1. (p1, wektor)");
		System.out.println("2. Obróć punkt p1 wokół punktu p2. (p1, p2, alfa)");
		System.out.println("3. Odbij punkt p1 przez prostą l. (p1, l)");
		System.out.println("4. Pokaż złożenie wektorów. (u, v)");
		System.out.println("5. Czy proste są równoległe? (l, m)");	
		System.out.println("6. Czy proste są prostopadłe? (l, m)");	
		System.out.println("7. Pokaż punkt przecięcia prostych. (l, m)"); 
		System.out.println("8. Przesuń odcinek o1. (o1, wektor)");
		System.out.println("9. Obróć odcinek o1 wokół punktu p2. (o1, p2, alfa)");
		System.out.println("10. Odbij odcinek o1 przez prostą l. (o1, l)");
		System.out.println("11. Przesuń trójkąt t1. (t1, wektor)");
		System.out.println("12. Obróć trójkąt t1 wokół punktu p2. (t1, p2, alfa)");
		System.out.println("13. Odbij trójkąt t1 przez prostą l. (t1, l)");

		int wybor = input();

		wypisz_baze_danych();
		System.out.println("Podaj odpowiednie parametry.");

		switch(wybor) {

			case 1:
			{
				int id_p = input();
				int id_v = input();

				tab_punktow[id_p] = new Punkt(tab_punktow[id_p],tab_wektorow[id_v]);
				break;
			}

			case 2:
			{
				int id_p1 = input();
				int id_p2 = input();
				int alfa = input();

				tab_punktow[id_p1] = new Punkt(tab_punktow[id_p1],tab_punktow[id_p2],alfa);
				break;
			}


			case 3:
			{
				int id_p = input();
				int id_l = input();

				tab_punktow[id_p] = new Punkt(tab_punktow[id_p],tab_prostych[id_l]);
				break;
			}

			case 4:
			{
				int id_u = input();
				int id_v = input();

				Wektor w = Wektor.zloz(tab_wektorow[id_u],tab_wektorow[id_v]);
				System.out.println("Złożenie wektorów to: "+w.get_string());
				break;
			}

			case 5:
			{
				int id_l = input();
				int id_m = input();
				
				if(Prosta.czy_proste_rownolegle(tab_prostych[id_l],tab_prostych[id_m])) System.out.println("TAK");
				else System.out.println("NIE");
				break;
			}

			case 6:
			{
				int id_l = input();
				int id_m = input();
				
				if(Prosta.czy_proste_prostopadle(tab_prostych[id_l],tab_prostych[id_m])) System.out.println("TAK");
				else System.out.println("NIE");
				break;
			}

			case 7:
			{
				int id_l = input();
				int id_m = input();
				
				Punkt p = new Punkt(0,0);
				p = Prosta.przeciecie_sie_prostych(tab_prostych[id_l],tab_prostych[id_m]);
				System.out.println("Punkt przecięcia się prostych to: "+p.get_string());

				break;
			}

			case 8:
			{
				int id_o = input();
				int id_v = input();

				tab_odcinkow[id_o].przesun(tab_wektorow[id_v]);
				break;
			}

			case 9:
			{
				int id_o = input();
				int id_p = input();
				int alfa = input();

				tab_odcinkow[id_o].obroc(tab_punktow[id_p],alfa);
				break;
			}


			case 10:
			{
				int id_o = input();
				int id_l = input();

				tab_odcinkow[id_o].odbij(tab_prostych[id_l]);
				break;
			}
			
			case 11:
			{
				int id_t = input();
				int id_v = input();

				tab_trojkatow[id_t].przesun(tab_wektorow[id_v]);
				break;
			}

			case 12:
			{
				int id_t = input();
				int id_p = input();
				int alfa = input();

				tab_trojkatow[id_t].obroc(tab_punktow[id_p],alfa);
				break;
			}


			case 13:
			{
				int id_t = input();
				int id_l = input();

				tab_trojkatow[id_t].odbij(tab_prostych[id_l]);
				break;
			}

		}

		wybor_glowny();
	}




	// *****************************
	// ===== METODY STATYCZNE =====	
	public static void main(String[] args) {
		
		try {
			Figury f = new Figury();
			f.wybor_glowny();
		} catch (IOException io) {}

	}
	
	
}




