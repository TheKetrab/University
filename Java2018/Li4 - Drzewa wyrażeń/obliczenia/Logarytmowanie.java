package obliczenia;

public class Logarytmowanie extends Operator2Arg {

    public Logarytmowanie(Wyrazenie w1, Wyrazenie w2) {
        super(w1,w2);
    }

    @Override
    public double oblicz() {
        return Math.log(w2.oblicz()) / Math.log(w1.oblicz());
    }

    @Override
    public String toString() {
        return "Log_"+w1.toString()+"^"+w2.toString();
    }

}
