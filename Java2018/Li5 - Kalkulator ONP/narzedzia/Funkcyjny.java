package narzedzia;

import narzedzia.wyjatki.WyjatekONP;

public interface Funkcyjny extends Obliczalny {
    int arnosc ();
    int brakujaceArgumenty ();
    void dodajArgument (double arg) throws WyjatekONP;

}
