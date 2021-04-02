package obliczenia;

public class Dodawanie extends Operator2Arg {

    public Dodawanie(Wyrazenie w1, Wyrazenie w2) {
        super(w1,w2);
    }

    @Override
    public double oblicz() {
        return w1.oblicz() + w2.oblicz();
    }

    @Override
    public String toString() {
        return "("+w1.toString()+"+"+w2.toString()+")";
    }

}
