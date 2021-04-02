package narzedzia.funkcje;

import narzedzia.Funkcja1Arg;

public class Sin extends Funkcja1Arg {

    @Override
    public double oblicz() {
        return Math.sin(args.get(0));
    }

}
