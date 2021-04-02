package obliczenia;

public class WartBezwzgl extends Operator1Arg {

    public WartBezwzgl(Wyrazenie w) {
        super(w);
    }

    @Override
    public double oblicz() {
        return Math.abs(w1.oblicz());
    }

    @Override
    public String toString() {
        return "|"+w1.toString()+"|";
    }

}
