package struktury;

/** Implementacja zbioru na tablicy.
@author Bartłomiej Grochowski
@version 1.0
*/
public class ZbiorNaTablicy extends Zbior {
	
	protected Para tablica[];
	protected int rozmiar;
	
	/** W konstruktorze podajemy rozmiar zbioru. */
	public ZbiorNaTablicy (int rozmiar) throws IllegalArgumentException {
		
		if (rozmiar < 2)
			throw new IllegalArgumentException("Rozmiar tablicy nie może być mniejszy niż 2.");
		
		tablica = new Para[rozmiar];	
		this.rozmiar = rozmiar;
	}
	
	
	
	
	/** metoda ma szukać pary z określonym kluczem */
	public Para szukaj (String k) throws Exception {
		
		for (int i=0; i<rozmiar; i++)
			if (tablica[i].klucz.equals(k))
				return tablica[i];	
	
		throw new Exception("Nie znaleziono pary o podanym kluczu.");
	}

	/** metoda ma wstawiać do zbioru nową parę */	
	public void wstaw (Para p) throws Exception {
		
		for (int i=0; i<rozmiar; i++)
			if (tablica[i] == null) {
				tablica[i] = p;
				return;
			}
		
		throw new Exception("Brak wolnego miejsca w zbiorze!");
	}

	/** metoda ma odszukać parę i zwrócić wartość związaną z kluczem */	
	public double czytaj (String k) throws Exception {
		
		for (int i=0; i<rozmiar; i++)
			if (tablica[i].klucz.equals(k))
				return tablica[i].get_wartosc();
		
		throw new Exception("Nie znaleziono pary o podanym kluczu.");
	}

	/** metoda ma wstawić do zbioru nową albo zaktualizować parę */	
	public void ustaw (Para p) throws Exception {
	
		for (int i=0; i<rozmiar; i++)
			if (tablica[i].equals(p))
				tablica[i] = p;
			
		wstaw(p);			
	}

	/** metoda ma usunąć wszystkie pary ze zbioru */	
	public void czysc () {
		
		for (int i=0; i<rozmiar; i++) 
			tablica[i] = null;

	}
	
	/** metoda ma podać ile par jest przechowywanych w zbiorze */
	public int ile () {
		
		int licznik = 0;
		
		for (int i=0; i<rozmiar; i++)
			if (tablica[i] != null) licznik++;
		
		return licznik;
	}

	/** metoda zwraca rozmiar zbioru */
	public int get_rozmiar() {
		return rozmiar;
	}
	
	
	
	
}