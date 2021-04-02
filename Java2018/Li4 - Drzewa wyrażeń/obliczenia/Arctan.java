package obliczenia;

public class Arctan extends Operator1Arg {

    public Arctan(Wyrazenie w) {
        super(w);
    }

    @Override
    public double oblicz() {
        return Math.atan(w1.oblicz());
    }

    @Override
    public String toString() {
        return "arctg("+w1.toString()+")";
    }
}
