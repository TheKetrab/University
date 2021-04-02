package obliczenia;

public class Sinus extends Operator1Arg {

    public Sinus(Wyrazenie w) {
        super(w);
    }

    @Override
    public double oblicz() {
        return Math.sin(w1.oblicz());
    }

    @Override
    public String toString() {
        return "sin("+w1.toString()+")";
    }

}
