import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.function.IntPredicate;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.IntStream;


class IntString {

    int i;
    String s;

    IntString(int i, String s) {
        this.i = i; this.s = s;
    }

}

public class Zad1 {

    // patterns
    private String pattern_normal = "\\s*(0|[1-9][0-9]*)\\s*(//.*)?";
    private String pattern_empty = "\\s*";
    private String pattern_comment = "\\s*//.*";
    private Pattern pattern = Pattern.compile(pattern_normal);

    private ArrayList<Integer> list;
    boolean loadedFileSuccessfully;

    public Zad1(String inputPath) {
        list = new ArrayList<Integer>();
        loadedFileSuccessfully = LoadFile(inputPath);
    }

    /** return true if whole file parsed correctly */
    private boolean LoadFile(String inputPath) {

        try (BufferedReader fileReader = new BufferedReader(new FileReader(inputPath))) {

            for (String ln = fileReader.readLine(); ln != null; ln = fileReader.readLine()) {
                IntString info = isCorrectLine(ln);
                if (info.i == 0) throw new InterpreterException("incorrect line: " + ln);
                else if (info.i == 1) list.add(Integer.parseInt(info.s));
                else if (info.i == 2);
                else throw new InterpreterException("unknown state of 'info': " + info.i);
            }

            return true;

        } catch (IOException | InterpreterException e) {
            e.printStackTrace();
        }

        return false;
    }

    /** Zwraca informację o rodzaju patternu:
     *  0 - błąd
     *  1 - linia z liczbą
     *  2 - linia do zignorowania
     *  W przypadku 1 zwraca też liczbę jako String
     */
    private IntString isCorrectLine(String line) {

        if (line.matches(pattern_normal)) {

            Matcher matcher = pattern.matcher(line);
            matcher.matches();

            String num = matcher.group(1);

            return new IntString(1,num);
        }

        else if (line.matches(pattern_empty)) {
            return new IntString(2,"empty");
        }

        else if (line.matches(pattern_comment)) {
            return new IntString(2,"comment");
        }

        else {
            return new IntString(0,"error");
        }

    }

    public void printOrder() {

        System.out.println("sorted: ");
        System.out.println(Arrays.toString(list.stream().sorted((f1, f2) -> Integer.compare(f2, f1)).toArray()));
    }

    public void printPrimes() {

        // n jest pierwsze, jesli n>1 i ze strumienia [2,sqrt(n)], zaden x nie dzieli n
        IntPredicate isPrime = (n -> (n > 1 && IntStream.range(2, (int) (Math.sqrt(n))).noneMatch(x -> n%x == 0)));

        System.out.println("primes: ");
        System.out.println(Arrays.toString(list.stream().filter(isPrime::test).toArray()));
    }


    public void printSum() {

        var lowerThan1000 = list.stream().filter(x -> x<1000);
        int sum = lowerThan1000.mapToInt(Integer::intValue).sum();

        System.out.println("sum lowerThan1000: ");
        System.out.println(sum);

    }


}
