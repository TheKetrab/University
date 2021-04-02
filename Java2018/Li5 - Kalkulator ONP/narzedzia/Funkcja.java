package narzedzia;

import narzedzia.wyjatki.WyjatekONP;

import java.util.ArrayList;

public abstract class Funkcja extends Symbol implements Funkcyjny {

    private int arnosc;
    public ArrayList<Double> args = new ArrayList<Double>();

    protected Funkcja(int arnosc) {
        this.arnosc = arnosc;
    }

    public int arnosc() {
        return arnosc;
    }

    public int brakujaceArgumenty () {
        return arnosc - args.size();
    }

    public void dodajArgument (double arg) throws WyjatekONP {
        args.add(arg);

    }

}
