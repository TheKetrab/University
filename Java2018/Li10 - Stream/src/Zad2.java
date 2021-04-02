import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


class IntDouble3 {

    int i;
    double a,b,c;

    IntDouble3(int i, double a, double b, double c) {
        this.i = i;
        this.a = a;
        this.b = b;
        this.c = c;
    }
}

class Triangle implements Comparable<Triangle> {

    private double a,b,c;

    Triangle(double a, double b, double c) throws InterpreterException {

        if (!(a > 0 && b > 0 && c > 0))
            throw new InterpreterException("Boki trójkąta muszą być dodatnie!");

        // nierownosc trojkata
        if (a + b > c && a + c > b && b + c > a) {
            this.a = a;
            this.b = b;
            this.c = c;
        } else {
            throw new InterpreterException(
                    "Długości: " + a + " " + b + " " + c +
                    " nie spełniają nierówności trójkąta."
            );
        }
    }

    boolean isRect() {

        double a2 = a*a, b2 = b*b, c2 = c*c;

        return a2 + b2 == c2
                || a2 + c2 == b2
                || b2 + c2 == a2;
    }

    private double surface() {

        // Heron
        double p = (a + b + c) / 2.0;
        return Math.sqrt(p * (p-a) * (p-b) * (p-c));

    }

    private double l() {
        return a+b+c;
    }

    static int compareByL(Triangle t1, Triangle t2) {

        double l1 = t1.l();
        double l2 = t2.l();

        return Double.compare(l1, l2);
    }

    @Override
    public String toString() {
        return "T(" + a + " " + b + " " + c + ")";
    }

    @Override // porownujemy pola
    public int compareTo(Triangle triangle) {

        var s1 = this.surface();
        var s2 = triangle.surface();

        return Double.compare(s1, s2);

    }
}


public class Zad2 {

    // patterns
    private String pattern_normal = "\\s*([0-9]+\\.[0-9]+|[0-9]+)\\s+([0-9]+\\.[0-9]+|[0-9]+)\\s+([0-9]+\\.[0-9]+|[0-9]+)\\s*(//.*)?";
    private String pattern_empty = "\\s*";
    private String pattern_comment = "\\s*//.*";
    private Pattern pattern = Pattern.compile(pattern_normal);

    private ArrayList<Triangle> list;
    boolean loadedFileSuccessfully;

    public Zad2(String inputPath) {
        list = new ArrayList<Triangle>();
        loadedFileSuccessfully = LoadFile(inputPath);

    }


    private boolean LoadFile(String inputPath) {

        try (BufferedReader fileReader = new BufferedReader(new FileReader(inputPath))) {

            for (String ln = fileReader.readLine(); ln != null; ln = fileReader.readLine()) {
                IntDouble3 info = isCorrectLine(ln);
                if (info.i == 0) throw new InterpreterException("incorrect line: " + ln);
                else if (info.i == 1) {
                    Triangle t = new Triangle(info.a,info.b,info.c);
                    list.add(t);
                }
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
     *  1 - linia z bokami
     *  2 - linia do zignorowania
     *  W przypadku 1 zwraca też długości boków jako Double
     */
    private IntDouble3 isCorrectLine(String line) {

        if (line.matches(pattern_normal)) {

            Matcher matcher = pattern.matcher(line);
            matcher.matches();

            String s[] = new String[3];
            Double d[] = new Double[3];

            for (int i=0; i<=2; i++) {
                s[i] = matcher.group(i+1);
                d[i] = Double.parseDouble(s[i]);
            }

            return new IntDouble3(1,d[0],d[1],d[2]);
        }
        else if (line.matches(pattern_empty)) {
            return new IntDouble3(2,0,0,0);
        }
        else if (line.matches(pattern_comment)) {
            return new IntDouble3(2,0,0,0);
        }
        else {
            return new IntDouble3(0,0,0,0);
        }

    }

    public void printFromLowestL() {

        System.out.println("sorted from lowest L: ");
        System.out.println(Arrays.toString(list.stream().sorted(Triangle::compareByL).toArray()));

    }

    public void printRectTriangle() {

        System.out.println("rect triangles: ");
        System.out.println(Arrays.toString((list.stream().filter(Triangle::isRect)).toArray()));

    }

    public void printMinMaxSurface() {

        // mix i max zwracaja opcje, mozna ja wyciagnac przez get,
        // ale gdybysmy nie podali zadnego trojkata w pliku, to zostalby
        // zgloszony wyjatek, a tak to mamy Option.empty
        var minS = list.stream().min(Comparator.naturalOrder());
        var maxS = list.stream().max(Comparator.naturalOrder());

        System.out.println("minS: " + minS.get());
        System.out.println("maxS: " + maxS.get());

    }

}
