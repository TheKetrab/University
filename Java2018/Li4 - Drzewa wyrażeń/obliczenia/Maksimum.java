package obliczenia;

public class Maksimum extends Operator2Arg {

    public Maksimum(Wyrazenie w1, Wyrazenie w2) {
        super(w1,w2);
    }

    @Override public double oblicz() {
        double res1 = w1.oblicz();
        double res2 = w2.oblicz();
        return (res1>res2 ? res1 : res2);
    }

    @Override
    public String toString() {
        return "max("+w1.toString()+","+w2.toString()+")";
    }


}
