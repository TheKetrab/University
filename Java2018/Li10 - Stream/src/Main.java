public class Main {

    public static void main(String[] args) {

        System.out.println(" ===== ZADANIE 1 ===== ");
        Zad1 z1 = new Zad1("dane1.txt");
        if (z1.loadedFileSuccessfully) {
            z1.printOrder();
            z1.printPrimes();
            z1.printSum();
        }

        System.out.println(" ===== ZADANIE 2 ===== ");
        Zad2 z2 = new Zad2("dane2.txt");
        if (z2.loadedFileSuccessfully) {
            z2.printFromLowestL();
            z2.printRectTriangle();
            z2.printMinMaxSurface();
        }

    }
}
