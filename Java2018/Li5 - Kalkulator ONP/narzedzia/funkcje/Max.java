package narzedzia.funkcje;

import narzedzia.Funkcja2Arg;

public class Max extends Funkcja2Arg {

    @Override
    public double oblicz() {
        return Math.max(args.get(1),args.get(0));
    }
}
