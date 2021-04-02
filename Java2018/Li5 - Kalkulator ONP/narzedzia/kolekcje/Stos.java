package narzedzia.kolekcje;
import java.util.*;

public class Stos {

    private ArrayDeque<Double> s;

    public Stos() {
        s = new ArrayDeque<>();
    }

    public void push (double d) {
        s.push(d);
    }

    public double pop () {
        return s.pop();
    }

    public int size() {
        return s.size();
    }

    public boolean empty() {
        return s.isEmpty();
    }


}
