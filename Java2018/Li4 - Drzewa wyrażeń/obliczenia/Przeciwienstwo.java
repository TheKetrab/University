package obliczenia;

public class Przeciwienstwo extends Operator1Arg {

    public Przeciwienstwo(Wyrazenie w) {
        super(w);
    }

    @Override
    public double oblicz() {
        return (-1)*w1.oblicz();
    }

    @Override
    public String toString() {
        return "-("+w1.toString()+")";
    }

}
