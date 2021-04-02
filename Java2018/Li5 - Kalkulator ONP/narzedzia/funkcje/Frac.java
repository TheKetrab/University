package narzedzia.funkcje;

import narzedzia.Funkcja1Arg;
import narzedzia.wyjatki.ONP_SymbolNieoznaczony;

public class Frac extends Funkcja1Arg {

    // odwrotnosc
    @Override
    public double oblicz() throws ONP_SymbolNieoznaczony {
        if (args.get(0) == 0)
            throw new ONP_SymbolNieoznaczony("Dzielenie przez zero: Frac");
        return (1.0 / args.get(0));
    }
}
