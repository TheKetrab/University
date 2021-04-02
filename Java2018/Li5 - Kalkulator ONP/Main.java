import narzedzia.Wyrazenie;
import narzedzia.kolekcje.Lista;
import narzedzia.kolekcje.Zbior;
import narzedzia.wyjatki.ONP_Keyword;
import narzedzia.wyjatki.WyjatekONP;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {

    public static void printMatches(Matcher matcher) {
        System.out.println("amount: " + matcher.groupCount());
        for(int i=0; i< matcher.groupCount(); i++)
            System.out.println(i + " : ;" + matcher.group(i) + ";");
    }

    public static void main(String[] args) {

        while (true) {

            try {

                // ------------------ spacja, cyfry, litery, oeratory --- zmienne i przypisania
                String pattern_calc = "calc\\s*((\\s*(\\p{Alnum}|\\+|-|\\*|/|\\.)*)*)\\s*((\\s+\\p{Alnum}* =)*)";

                // ----------------------------- opcjonalne zmienne
                String pattern_clear = "clear\\s*((\\s*\\p{Alpha}\\p{Alnum}*)*)";

                String pattern_exit = "exit";


                Scanner wejscie = new Scanner(System.in);

                while (true) {

                    String line = wejscie.nextLine();

                    if (line.matches(pattern_calc)) {

                        Pattern pattern = Pattern.compile(pattern_calc);
                        Matcher matcher = pattern.matcher(line);
                        matcher.matches();

                        String wyrazenieONP = matcher.group(1);
                        String[] lista_zmiennych = matcher.group(4).split(" ");
                        Lista zmienne = new Lista();

                        int lista_zmiennych_size = lista_zmiennych.length;
                        for (int i = 0; i < lista_zmiennych_size; i++)
                            if (!lista_zmiennych[i].equals("="))
                                zmienne.dodaj(lista_zmiennych[i]);

                        Wyrazenie w = new Wyrazenie(wyrazenieONP, zmienne);
                        System.out.println(w.oblicz());

                    } else if (line.matches(pattern_clear)) {

                        Pattern pattern = Pattern.compile(pattern_clear);
                        Matcher matcher = pattern.matcher(line);
                        matcher.matches();

                        String lista_zmiennych_do_usuniecia = matcher.group(1);
                        if (lista_zmiennych_do_usuniecia.equals(""))
                            Zbior.clear();
                        else {
                            String[] splitted = lista_zmiennych_do_usuniecia.split(" ");
                            int n = splitted.length;
                            for (int i = 0; i < n; i++)
                                if (Zbior.isKeyword(splitted[i]))
                                    throw new ONP_Keyword("Probujesz usunac słowo kluczowe: " + splitted[i]);
                                else
                                    Zbior.remove(splitted[i]);
                        }

                    } else if (line.matches(pattern_exit)) {
                        break;
                    } else assert false; // wlaczone -ea do kompilatora
                }


            } catch (WyjatekONP w) {
                System.out.println("Blad ONP: " + w);
                continue;
            } catch (AssertionError a) {
                System.out.println("Nieznana komenda.");
                continue;
            } catch (FileNotFoundException e) {
                System.out.println("Nie znaleziono pliku calc.log - utworz go!");
                continue;
            } catch (IOException e) {
                System.out.println("IOException");
            }


            break;

        }

    }
}
