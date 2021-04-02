package narzedzia.funkcje;

import narzedzia.Funkcja1Arg;

public class Atan extends Funkcja1Arg {

    @Override
    public double oblicz() {
        return Math.atan(args.get(0));
    }
}
