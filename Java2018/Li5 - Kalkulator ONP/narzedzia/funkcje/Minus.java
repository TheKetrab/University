package narzedzia.funkcje;

import narzedzia.Funkcja2Arg;

public class Minus extends Funkcja2Arg {

    @Override
    public double oblicz() {
        return args.get(1) - args.get(0);
    }

}
