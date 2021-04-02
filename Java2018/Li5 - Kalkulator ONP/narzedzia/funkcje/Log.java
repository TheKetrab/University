package narzedzia.funkcje;

import narzedzia.Funkcja2Arg;
import narzedzia.wyjatki.ONP_SymbolNieoznaczony;

public class Log extends Funkcja2Arg {

    // log_a_b
    // a b log
    // a = get(1), b = get(0)

    @Override
    public double oblicz() throws ONP_SymbolNieoznaczony {
        if (args.get(0) < 0)
            throw new ONP_SymbolNieoznaczony("LOG: b<0");
        if (args.get(1) < 0)
            throw new ONP_SymbolNieoznaczony("LOG: a<0");
        if (args.get(1) == 1)
            throw new ONP_SymbolNieoznaczony("LOG: a=1");

        return Math.log(args.get(1)) / Math.log(args.get(0));
    }
}
