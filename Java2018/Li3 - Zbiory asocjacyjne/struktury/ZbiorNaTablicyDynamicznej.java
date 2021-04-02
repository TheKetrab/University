package struktury;

/** Implementacja zbioru na tablicy dynamicznej.
@author Bartłomiej Grochowski
@version 1.0
*/
public class ZbiorNaTablicyDynamicznej extends ZbiorNaTablicy {
		
	public ZbiorNaTablicyDynamicznej (int rozmiar) throws IllegalArgumentException {
		super(rozmiar);
	}

	/** metoda ma wstawiać do zbioru nową parę, gdy brakuje miejsca rozszerza zbiór */	
	public void wstaw (Para p) {
		
		for (int i=0; i<rozmiar; i++)
			if (tablica[i] == null) {
				tablica[i] = p;
				return;
			}
		
		int nowy_rozmiar = rozmiar * 2;
		Para nowa_tablica[] = new Para[nowy_rozmiar];
		
		// przepisanie wskaznikow na pary
		for (int i=0; i<rozmiar; i++)
			nowa_tablica[i] = tablica[i];
		
		nowa_tablica[rozmiar] = p; // wstawienie nowej pary
		
		// zmiana wskaznikow
		tablica = nowa_tablica;
		rozmiar = nowy_rozmiar;
	}

	
	/** możemy wypisać zbiór */
	public String toString() {
		
		String s = "";
		for (int i=0; i<rozmiar; i++)
			if (tablica[i] != null)
				s += "("+tablica[i].toString()+")";
			
		return s;
	}
	
}