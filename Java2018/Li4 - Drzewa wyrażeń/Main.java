import obliczenia.*;

public class Main {

    public static void main(String[] args) {

        Zmienna.hm.put("x",2.0);
        Zmienna.hm.put("y",2.0);

        Wyrazenie w1 = new Dodawanie(new Liczba(3),
                                    new Liczba(5));


        Wyrazenie w2 = new Dodawanie(new Liczba(2),
                                    new Mnozenie(new Zmienna("x"),
                                                new Liczba(7)));
        Wyrazenie w3 = new Dzielenie
                            (new Odejmowanie(
                                    new Mnozenie(
                                            new Liczba(3),
                                            new Liczba(11)),
                                    new Liczba(1)),
                            new Dodawanie(
                                    new Liczba(7),
                                    new Liczba(5)));

        Wyrazenie w4 = new Arctan(
                        new Dzielenie(
                                new Mnozenie(
                                        new Dodawanie(
                                                new Zmienna("x"),
                                                new Liczba(13)),
                                        new Zmienna("x")),
                                new Liczba(2)));

        Wyrazenie w5 = new Dodawanie(
                            new Potegowanie(
                                    new Liczba(2),
                                    new Liczba(5)),
                            new Mnozenie(
                                    new Zmienna("x"),
                                    new Logarytmowanie(
                                            new Liczba(2),
                                            new Zmienna("y"))));


        System.out.println("w1: " + w1 + " = " + w1.oblicz());
        System.out.println("w2: " + w2 + " = " + w2.oblicz());
        System.out.println("w3: " + w3 + " = " + w3.oblicz());
        System.out.println("w4: " + w4 + " = " + w4.oblicz());
        System.out.println("w5: " + w5 + " = " + w5.oblicz());

    }
}
