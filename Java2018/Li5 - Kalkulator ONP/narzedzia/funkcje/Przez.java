package narzedzia.funkcje;

import narzedzia.Funkcja2Arg;
import narzedzia.wyjatki.ONP_SymbolNieoznaczony;

public class Przez extends Funkcja2Arg {

    @Override
    public double oblicz() throws ONP_SymbolNieoznaczony {
        if (args.get(0) == 0)
            throw new ONP_SymbolNieoznaczony("Dzielenie przez zero!");
        return args.get(1) / args.get(0);
    }

}
