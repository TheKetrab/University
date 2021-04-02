package narzedzia.funkcje;

import narzedzia.Funkcja2Arg;

public class Pow extends Funkcja2Arg {

    @Override
    public double oblicz() {
        return Math.pow(args.get(1),args.get(0));
    }

}
