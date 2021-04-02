package narzedzia.funkcje;

import narzedzia.Funkcja2Arg;

public class Min extends Funkcja2Arg {

    @Override
    public double oblicz() {
        return Math.min(args.get(1),args.get(0));
    }

}
