package obliczenia;

public abstract class Operator2Arg extends Operator1Arg {

    protected Wyrazenie w2;

    public Operator2Arg (Wyrazenie w1, Wyrazenie w2) {
        super(w1);
        this.w2 = w2;
    }


}
