package narzedzia.funkcje;

import narzedzia.Funkcja1Arg;

public class Exp extends Funkcja1Arg {

    @Override
    public double oblicz() {
        return Math.exp(args.get(0));
    }
}
