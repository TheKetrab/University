package narzedzia.funkcje;

import narzedzia.Funkcja1Arg;
import narzedzia.wyjatki.ONP_SymbolNieoznaczony;

public class Ln extends Funkcja1Arg {

    @Override
    public double oblicz() throws ONP_SymbolNieoznaczony {
        if (args.get(0) <= 0)
            throw new ONP_SymbolNieoznaczony("Ln z liczby niedodatniej.");
        return Math.log(args.get(0));
    }
}
