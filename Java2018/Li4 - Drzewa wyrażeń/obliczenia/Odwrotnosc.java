package obliczenia;

public class Odwrotnosc extends Operator1Arg {

    public Odwrotnosc(Wyrazenie w) {
        super(w);
    }

    @Override
    public double oblicz() {
        return 1 / w1.oblicz();
    }

    @Override
    public String toString() {
        return "( 1 / "+w1.toString()+")";
    }

}
