import java.util.Vector;

public class Cell {

    public Cell father;
    public Vector<Cell> children;

    private int x;
    private int y;

    public Cell(int x, int y) {
        this.x = x;
        this.y = y;
        children = new Vector<Cell>();
    }

    public int Children() {
        return children.size();
    }

    public void AddChild(Cell child) {
        children.add(child);
    }

    public void SetFather(Cell father) {
        this.father = father;
    }

    public int x() {
        return x;
    }

    public int y() {
        return y;
    }

    public Cell GetChild(int n) {
        return children.get(n);
    }

    @Override
    public String toString() {
        return "["+x+","+y+"]";
    }

}