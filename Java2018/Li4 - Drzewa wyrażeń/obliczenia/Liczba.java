package obliczenia;

public class Liczba extends Wyrazenie {

    private double liczba;

    public Liczba(double liczba) {
        this.liczba = liczba;
    }

    @Override
    public double oblicz() {
        return liczba;
    }

    @Override
    public String toString() {
        return Double.toString(liczba);
    }

}