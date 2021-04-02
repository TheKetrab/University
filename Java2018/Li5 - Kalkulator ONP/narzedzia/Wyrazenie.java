package narzedzia;

import narzedzia.funkcje.*;
import narzedzia.kolekcje.Kolejka;
import narzedzia.kolekcje.Lista;
import narzedzia.kolekcje.Stos;
import narzedzia.kolekcje.Zbior;
import narzedzia.wyjatki.*;

import java.io.*;
import java.util.regex.Pattern;

public class Wyrazenie implements Obliczalny {

    public boolean obliczone;
    private double wynik = 0;

    private Kolejka kolejka = new Kolejka(); // kolejka symboli wyrażenia ONP (elementy typu Symbol)
    private Stos stos = new Stos(); // stos z wynikami pośrednimi obliczeń (elementy typu Double)
    private Zbior zmienne; // lista zmiennych czyli pary klucz-wartość (String-Double)

    public Wyrazenie (String onp, Lista zm) throws WyjatekONP, IOException {

        // zapisanie logu do pliku
        System.out.println("ONP: " + onp);
        FileWriter log = new FileWriter("calc.log", true);
        log.write(onp);
        log.write("\r\n"); // zeby na windowsie bylo ok
        log.close();
        // ----- ----- ----- ----- -----


        String[] splitted = onp.split(" ");
        int length = splitted.length;

        // parsowanie napisu na kolejke symboli
        for (int i = 0; i < length; i++) {

            String symbol = splitted[i];

            // jesli jest liczba typu double
            if (Pattern.matches("\\d+.?\\d*", symbol))
                kolejka.put(new Liczba(Double.parseDouble(symbol)));

            // jesli jest operatorem
            else if (symbol.equals("+")) kolejka.put(new Plus());
            else if (symbol.equals("-")) kolejka.put(new Minus());
            else if (symbol.equals("*")) kolejka.put(new Razy());
            else if (symbol.equals("/")) kolejka.put(new Przez());

                // jesli jest funkcja
            else if (symbol.equals("abs")) kolejka.put(new Abs());
            else if (symbol.equals("acot")) kolejka.put(new Acot());
            else if (symbol.equals("atan")) kolejka.put(new Atan());
            else if (symbol.equals("ceil")) kolejka.put(new Ceil());
            else if (symbol.equals("cos")) kolejka.put(new Cos());
            else if (symbol.equals("e")) kolejka.put(new E());
            else if (symbol.equals("exp")) kolejka.put(new Exp());
            else if (symbol.equals("floor")) kolejka.put(new Floor());
            else if (symbol.equals("frac")) kolejka.put(new Frac());
            else if (symbol.equals("ln")) kolejka.put(new Ln());
            else if (symbol.equals("log")) kolejka.put(new Log());
            else if (symbol.equals("max")) kolejka.put(new Max());
            else if (symbol.equals("min")) kolejka.put(new Min());
            else if (symbol.equals("minus")) kolejka.put(new Minus());
            else if (symbol.equals("phi")) kolejka.put(new Phi());
            else if (symbol.equals("pi")) kolejka.put(new Pi());
            else if (symbol.equals("plus")) kolejka.put(new Plus());
            else if (symbol.equals("pow")) kolejka.put(new Pow());
            else if (symbol.equals("przez")) kolejka.put(new Przez());
            else if (symbol.equals("razy")) kolejka.put(new Razy());
            else if (symbol.equals("sgn")) kolejka.put(new Sgn());
            else if (symbol.equals("sin")) kolejka.put(new Sin());

                // jesli jest zmienna
            else if (Pattern.matches("\\p{Alpha}\\p{Alnum}*", symbol))
                kolejka.put(new Zmienna(symbol));

                // niezidentyfikowany obiekt
            else throw new ONP_NieznanySymbol("Nieznany symbol: " + symbol);


        }

        double wynik = oblicz();

        // dodawanie zmiennych
        while (!zm.empty()) {
            String klucz = zm.pop();
            if (Zbior.isKeyword(klucz))
                throw new ONP_Keyword("Słowo " + klucz + "to słowo kluczowe!");
            Zbior.add(klucz,wynik);
        }





    }



    @Override
    public double oblicz() throws WyjatekONP {

        if (obliczone)
            return wynik;

        while(!kolejka.empty()) {

            Symbol s = kolejka.pop();

            if (s instanceof Operand)
                stos.push( ((Operand)s).oblicz() );

            else if (s instanceof Funkcja) {

                Funkcja fun = (Funkcja)s;

                while (fun.brakujaceArgumenty() > 0) {

                    if (stos.empty())
                        throw new ONP_BledneWyrazenie("Brakuje argumentow do funkcji: " + fun);

                    double d = stos.pop();
                    fun.dodajArgument(d);
                }

                stos.push( fun.oblicz() );

            }

        }

        if (stos.size() != 1)
            throw new ONP_Stos("Po ewaluacji RPN stos.size != 1 !!");

        double res = stos.pop();

        obliczone = true;
        wynik = res;

        return res;


    }
}
