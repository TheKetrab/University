package narzedzia.kolekcje;
import narzedzia.Symbol;

import java.util.*;

public class Kolejka {

    private ArrayDeque<Symbol> k;

    public Kolejka() {
        k = new ArrayDeque<Symbol>();
    }

    public void put(Symbol s) {
        k.add(s);
    }

    public Symbol pop() {
        return k.pop();
    }

    public boolean empty() {
        return k.isEmpty();
    }

}
