package narzedzia.funkcje;

import narzedzia.Funkcja1Arg;

public class Acot extends Funkcja1Arg {


    @Override
    public double oblicz() {
        return Math.atan(1.0 / args.get(0));
    }
}
