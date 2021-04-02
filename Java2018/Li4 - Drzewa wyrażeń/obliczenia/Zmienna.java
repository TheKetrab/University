package obliczenia;
import java.util.HashMap;

public class Zmienna extends Wyrazenie {

    public static final HashMap<String, Double> hm = new HashMap<String, Double>();
    private String nazwa;

    public Zmienna(String nazwa) {
        this.nazwa = nazwa;
    }

    @Override
    public double oblicz() {
        Double d = hm.get(nazwa);
        if (d == null) return 0;
        return d;
    }

    @Override
    public String toString() {
        return nazwa;
    }
}
