package obliczenia;

public class Minimum extends Operator2Arg {

    public Minimum(Wyrazenie w1, Wyrazenie w2) {
        super(w1,w2);
    }

    @Override public double oblicz() {
        double res1 = w1.oblicz();
        double res2 = w2.oblicz();
        return (res1<res2 ? res1 : res2);
    }

    @Override
    public String toString() {
        return "min("+w1.toString()+","+w2.toString()+")";
    }

}
