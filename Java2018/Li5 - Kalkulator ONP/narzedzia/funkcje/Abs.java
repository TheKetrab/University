package narzedzia.funkcje;

import narzedzia.Funkcja1Arg;

public class Abs extends Funkcja1Arg {

    @Override
    public double oblicz() {
        return Math.abs(args.get(0));
    }

}
