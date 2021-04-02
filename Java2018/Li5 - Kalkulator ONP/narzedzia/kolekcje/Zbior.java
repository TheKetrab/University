package narzedzia.kolekcje;
import narzedzia.wyjatki.ONP_NieznanySymbol;

import java.util.*;

public class Zbior {

    private static TreeMap<String,Double> z = new TreeMap<>();

    private static String[] keywords = {
            "calc", "clear", "exit",
            "min", "max", "log", "pow",
            "abs", "sgn", "floor", "ceil", "frac", "sin", "cos", "atan", "acot", "ln", "exp",
            "e", "pi", "phi"
    };

    public static boolean isKeyword(String s) {

        for (String keyword : keywords)
            if (s.equals(keyword))
                return true;

        return false;
    }

    public static double get(String key) throws ONP_NieznanySymbol {

        Double d = z.get(key);

        if (d == null)
            throw new ONP_NieznanySymbol("Nieznana zmienna: " + key);

        return d.doubleValue();
    }

    public static void add(String key, double val) {
        z.put(key,val);
    }

    public static double remove(String key) {
        return z.remove(key);
    }

    public static void clear() {
        z.clear();
    }
}
