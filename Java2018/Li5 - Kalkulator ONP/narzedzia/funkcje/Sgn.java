package narzedzia.funkcje;

import narzedzia.Funkcja1Arg;

public class Sgn extends Funkcja1Arg {

    @Override
    public double oblicz() {
        return Math.signum(args.get(0));
    }

}
