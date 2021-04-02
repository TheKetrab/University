package narzedzia;

public class Liczba extends Operand {
    private double liczba;

    public Liczba(double liczba) {
        this.liczba = liczba;
    }

    @Override
    public double oblicz() {
        return liczba;
    }
}
