

class TupleIIB {

    public int x, y;
    public boolean b;

    public TupleIIB (int x, int y, boolean b) {
        this.x = x; this.y = y; this.b = b;
    }
}

public class Game {

    private int dim = 19;
    public Cell[][] tab;
    public Cell start;
    public Cell finish;

    private int factorWrongWays = 5; // 80%
    private int lengthToExit = 75;

    public Game() throws Exception {

        tab = new Cell[dim][dim];
        InitializeCorrectWay();
        InitializeWrongWays();

    }

    public int dim() {
        return dim;
    }

    public void InitializeCorrectWay() throws Exception {

        int x = dim-1, y = dim-1;
        Cell child;

        Cell temp = new Cell(x,y); // prawy gorny rog - finish
        tab[x][y] = temp;
        this.finish = temp;

        int n = (int)(Math.random() * 10) + lengthToExit; // sciezka ma dlugosc max: lengthToExit+10

        for (int i=0; i<n; i++) {

            // trzeba sprawdzic czy jest gdzie isc, jak nie ma to break, bo sie zagniezdzilismy
            TupleIIB t = ModifyXY(x,y);
            if (t.b == false) break;
            x = t.x; y = t.y;

            // tworzymy nowe pole, nastepnikiem jest to, ktore bylo nowe do tej pory
            child = temp;
            temp = new Cell(x,y);

            child.SetFather(temp);
            temp.AddChild(child);

            tab[x][y] = temp;

        }

        this.start = temp;

    }

    public void InitializeWrongWays() throws Exception {

        Cell temp = start;
        int x=0, y=0;

        while (temp != finish) {

            x = temp.x();
            y = temp.y();

            int rand = (int)(Math.random() * factorWrongWays);

            if (rand != 0) {

                TupleIIB t = ModifyXY(x,y);

                if (t.b == true) { // jesli udalo sie znalezc nowe dostepne (x,y)

                    x = t.x; y = t.y;
                    Cell wrongChild = InitializeWrongCell(x,y);
                    wrongChild.SetFather(temp);
                    temp.AddChild(wrongChild);
                    continue; // byc moze chcemy miec jeszcze wiecej dzieci
                }
            }

            // korzystamy z tego, ze pierwsze dziecko, to poprawna sciezka
            temp = temp.GetChild(0);

        }

    }

    Cell InitializeWrongCell(int x, int y) throws Exception {

        Cell res = new Cell(x,y);
        tab[x][y] = res; // zwiazanie wskaznika w tablicy
        int base_x = x; int base_y = y;

        int rand = (int)(Math.random() * factorWrongWays);

        while (rand != 0) { // 80%

            x = base_x; y = base_y;
            TupleIIB t = ModifyXY(x,y);

            if (t.b == true) { // jesli udalo sie znalezc nowe dostepne (x,y)

                x = t.x; y = t.y;
                Cell wrongChild = InitializeWrongCell(x,y);
                wrongChild.SetFather(res);
                res.AddChild(wrongChild);
            }

            else
                break; // jesli nie ma wolnego to uciekaj z petli


            // byc moze chcemy wiecej dzieci
            rand = (int)(Math.random() * factorWrongWays);
        }

        return res;
    }


    public void PrintCorrectWay() {

        Cell temp = start;
        while (true) {
            System.out.print(" -> "+temp);
            if (temp == finish) break;
            temp = temp.GetChild(0);
        }
    }

    public void PrintPtrTable() {

        System.out.println();

        for (int i=dim-1; i>=0; i--) {
            for (int j=0; j<dim; j++)
                if (tab[i][j] == start)
                    System.out.print("[S]");
                else if (tab[i][j] == finish)
                    System.out.print("[M]");
                else if (tab[i][j] == null)
                    System.out.print("[O]");
                else
                    System.out.print("[X]");
            System.out.print("\n");
        }

        System.out.println();

    }







    TupleIIB ModifyXY(int x, int y) throws Exception {

        int newWay = FindNewXY(x,y);
        if (newWay == 0) return new TupleIIB(x,y,false); // brak ruchow
        else if (newWay == 1) y++; // GoUp
        else if (newWay == 2) y--; // GoDown
        else if (newWay == 3) x--; // GoLeft
        else if (newWay == 4) x++; // GoRight;
        else throw new Exception("newWay returns:" + newWay);

        return new TupleIIB(x,y,true);
    }



    int FindNewXY(int x, int y) {

        int counter = 0;
        int ways[] = new int[4];

        if (PossibleGoUp(x,y)) { // 1 = GoUp
            ways[counter] = 1;
            counter++;
        }

        if (PossibleGoDown(x,y)) { // 2 = GoDown
            ways[counter] = 2;
            counter++;
        }

        if (PossibleGoLeft(x,y)) { // 3 = GoLeft
            ways[counter] = 3;
            counter++;
        }

        if (PossibleGoRight(x,y)) { // 4 = GoRight
            ways[counter] = 4;
            counter++;
        }

        if (counter == 0) return 0;
        int rand = (int)(Math.random() * counter);
        return ways[rand];
    }




    boolean PossibleGoAnywhere(int x, int y) {

        if (PossibleGoUp(x,y)) return true;
        if (PossibleGoDown(x,y)) return true;
        if (PossibleGoLeft(x,y)) return true;
        if (PossibleGoRight(x,y)) return true;

        return false;
    }

    private boolean PossibleGoUp(int x, int y) {

        if (y+1 >= dim || tab[x][y+1] != null)
            return false;

        return true;
    }

    private boolean PossibleGoDown(int x, int y) {

        if (y <= 0 || tab[x][y-1] != null)
            return false;

        return true;
    }

    private boolean PossibleGoLeft(int x, int y) {

        if (x <= 0 || tab[x-1][y] != null)
            return false;

        return true;
    }

    private boolean PossibleGoRight(int x, int y) {

        if (x+1 >= dim || tab[x+1][y] != null)
            return false;

        return true;
    }

    public boolean ChildOrFatherOnUp(int x, int y) {

        if (tab[x][y] == null) return false;
        if (y+1 >= dim) return false;

        Cell thisCell = tab[x][y];
        Cell up = tab[x][y+1];

        if (thisCell.father == up)
            return true;

        for (int i=0; i<thisCell.Children(); i++)
            if (thisCell.GetChild(i) == up)
                return true;

        return false;

    }

    public boolean ChildOrFatherOnDown(int x, int y) {

        if (tab[x][y] == null) return false;
        if (y <= 0) return false;

        Cell thisCell = tab[x][y];
        Cell down = tab[x][y-1];

        if (thisCell.father == down)
            return true;

        for (int i=0; i<thisCell.Children(); i++)
            if (thisCell.GetChild(i) == down)
                return true;

        return false;

    }

    public boolean ChildOrFatherOnLeft(int x, int y) {

        if (tab[x][y] == null) return false;
        if (x <= 0) return false;

        Cell thisCell = tab[x][y];
        Cell left = tab[x-1][y];

        if (thisCell.father == left)
            return true;

        for (int i=0; i<thisCell.Children(); i++)
            if (thisCell.GetChild(i) == left)
                return true;

        return false;

    }

    public boolean ChildOrFatherOnRight(int x, int y) {

        if (tab[x][y] == null) return false;
        if (x+1 >= dim) return false;

        Cell thisCell = tab[x][y];
        Cell right = tab[x+1][y];

        if (thisCell.father == right)
            return true;

        for (int i=0; i<thisCell.Children(); i++)
            if (thisCell.GetChild(i) == right)
                return true;

        return false;

    }


}