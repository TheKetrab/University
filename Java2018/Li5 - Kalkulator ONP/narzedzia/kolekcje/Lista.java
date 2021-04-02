package narzedzia.kolekcje;
import java.util.*;

public class Lista {

    // w liscie przechowujemy zmienne do zainicjowania
    private LinkedList<String> l;

    public Lista () {
        l = new LinkedList<>();
    }

    public void dodaj(String s) {
        l.add(s);
    }

    public boolean empty() {
        return l.isEmpty();
    }

    public String pop() {
        return l.pollFirst();
    }
}
