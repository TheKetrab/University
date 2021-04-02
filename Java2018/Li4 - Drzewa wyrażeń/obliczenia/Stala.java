package obliczenia;

public class Stala extends Wyrazenie {

    public final double wartosc;

    public Stala(double wartosc) {
        this.wartosc = wartosc;
    }

    @Override
    public double oblicz() {
        return wartosc;
    }

    @Override
    public String toString() {
        return Double.toString(wartosc);
    }

}
