package narzedzia.funkcje;

import narzedzia.Funkcja1Arg;

public class Ceil extends Funkcja1Arg {

    @Override
    public double oblicz() {
        return Math.ceil(args.get(0));
    }
}
