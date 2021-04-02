package obliczenia;

public class Potegowanie extends Operator2Arg {

    public Potegowanie(Wyrazenie w1, Wyrazenie w2) {
        super(w1,w2);
    }

    @Override
    public double oblicz() {
        return Math.pow(w1.oblicz(),w2.oblicz());
    }

    @Override
    public String toString() {
        return "("+w1.toString()+"^"+w2.toString()+")";
    }

}
