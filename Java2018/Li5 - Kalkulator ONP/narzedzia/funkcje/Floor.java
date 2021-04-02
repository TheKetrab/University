package narzedzia.funkcje;

import narzedzia.Funkcja1Arg;

public class Floor extends Funkcja1Arg {

    @Override
    public double oblicz() {
        return Math.floor(args.get(0));
    }
}
