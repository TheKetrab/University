package narzedzia;

import narzedzia.kolekcje.Zbior;
import narzedzia.wyjatki.ONP_NieznanySymbol;
import narzedzia.wyjatki.WyjatekONP;

public class Zmienna extends Operand {

    private String nazwa;

    public Zmienna(String nazwa) {
        this.nazwa = nazwa;
    }

    @Override
    public double oblicz() throws ONP_NieznanySymbol {
        return Zbior.get(nazwa);
    }
}
