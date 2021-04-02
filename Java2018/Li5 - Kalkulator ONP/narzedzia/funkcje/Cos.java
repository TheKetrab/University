package narzedzia.funkcje;

import narzedzia.Funkcja1Arg;

public class Cos extends Funkcja1Arg {

    @Override
    public double oblicz() {
        return Math.cos(args.get(0));
    }
}
