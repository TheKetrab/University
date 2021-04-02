package obliczenia;

public class Cosinus extends Operator1Arg {

    public Cosinus(Wyrazenie w) {
        super(w);
    }

    @Override
    public double oblicz() {
        return Math.cos(w1.oblicz());
    }

    @Override
    public String toString() {
        return "cos("+w1.toString()+")";
    }


}
