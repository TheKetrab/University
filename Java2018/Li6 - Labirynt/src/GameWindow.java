import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.awt.Dimension;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;



















public class GameWindow implements KeyListener {

    private JFrame mainFrame;
    private JPanel controlPanel;
    private MyCanvas canvas;

    public int xCharacter;
    public int yCharacter;

    private int cellSize = 30;
    private Game game;

    public GameWindow(Game game) {

        this.game = game;

        xCharacter = game.start.x();
        yCharacter = game.start.y();

        prepareGUI();
        canvas = new MyCanvas();
        controlPanel.add(canvas);
        mainFrame.addKeyListener(this);

    }


    private void prepareGUI() {

        // glowne okno
        mainFrame = new JFrame("Labirynt");
        mainFrame.setSize(650,650);
        mainFrame.setLayout(new GridLayout(1, 1));
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
        mainFrame.setLocation(dim.width/2-mainFrame.getSize().width/2, dim.height/2-mainFrame.getSize().height/2);

        controlPanel = new JPanel();
        controlPanel.setLayout(new FlowLayout());

        //mainFrame.add(headerLabel);
        mainFrame.add(controlPanel);
        mainFrame.setVisible(true);
    }

    public void showCanvas() {

        mainFrame.setVisible(true);
    }



    @Override
    public void keyReleased(KeyEvent key) {

    }

    @Override
    public void keyPressed(KeyEvent key) {


        if (key.getKeyCode() == KeyEvent.VK_UP) {
            if (game.ChildOrFatherOnUp(xCharacter, yCharacter)) {
                canvas.PaintCell(xCharacter, yCharacter, canvas.getGraphics());
                yCharacter++;
                canvas.PaintCharacter(canvas.getGraphics());
            }
        }

        else if (key.getKeyCode() == KeyEvent.VK_DOWN) {
            if (game.ChildOrFatherOnDown(xCharacter, yCharacter)) {
                canvas.PaintCell(xCharacter, yCharacter, canvas.getGraphics());
                yCharacter--;
                canvas.PaintCharacter(canvas.getGraphics());
            }
        }

        else if (key.getKeyCode() == KeyEvent.VK_LEFT) {
            if (game.ChildOrFatherOnLeft(xCharacter, yCharacter)) {
                canvas.PaintCell(xCharacter, yCharacter, canvas.getGraphics());
                xCharacter--;
                canvas.PaintCharacter(canvas.getGraphics());
            }
        }

        else if (key.getKeyCode() == KeyEvent.VK_RIGHT) {
            if (game.ChildOrFatherOnRight(xCharacter, yCharacter)) {
                canvas.PaintCell(xCharacter, yCharacter, canvas.getGraphics());
                xCharacter++;
                canvas.PaintCharacter(canvas.getGraphics());
            }
        }

        //controlPanel.getComponent(0).repaint();
    }

    @Override
    public void keyTyped(KeyEvent key) {

    }




    /** ----- ----- CANVAS ----- ----- */
    class MyCanvas extends Canvas {

        private Image arrowUp;
        private Image arrowDown;
        private Image arrowLeft;
        private Image arrowRight;
        private Image eksplorator;
        private Image background;
        private Image exit;

        public MyCanvas () {
            //setBackground (Color.GREEN);
            setSize(cellSize*game.dim(), cellSize*game.dim());
            LoadImages();
        }


        private void LoadImages() {

            try {
                String filename;

                filename = "img/ArrowUp.png";
                arrowUp = ImageIO.read(new File(filename));

                filename = "img/ArrowDown.png";
                arrowDown = ImageIO.read(new File(filename));

                filename = "img/ArrowLeft.png";
                arrowLeft = ImageIO.read(new File(filename));

                filename = "img/ArrowRight.png";
                arrowRight = ImageIO.read(new File(filename));

                filename = "img/Eksplorator.png";
                eksplorator = ImageIO.read(new File(filename));

                filename = "img/Background.png";
                background = ImageIO.read(new File(filename));

                filename = "img/Exit.png";
                exit = ImageIO.read(new File(filename));


            } catch (Exception e) {
                System.out.println("Nie udalo sie odczytac obrazu");
            }
        }

        public void PaintCell(int x, int y, Graphics g) {


            Cell cellToDraw = game.tab[x][y];

            g.drawImage(background, x * cellSize, Math.abs(y-18) * cellSize, cellSize, cellSize, null);

            if (game.ChildOrFatherOnUp(x, y)) {
                g.drawImage(arrowUp, x * cellSize, Math.abs(y-18) * cellSize, cellSize, cellSize, null);
            }

            if (game.ChildOrFatherOnDown(x, y)) {
                g.drawImage(arrowDown, x * cellSize, Math.abs(y-18) * cellSize, cellSize, cellSize, null);
            }

            if (game.ChildOrFatherOnLeft(x, y)) {
                g.drawImage(arrowLeft, x * cellSize, Math.abs(y-18) * cellSize, cellSize, cellSize, null);
            }

            if (game.ChildOrFatherOnRight(x, y)) {
                g.drawImage(arrowRight, x * cellSize, Math.abs(y-18) * cellSize, cellSize, cellSize, null);
            }

            if (cellToDraw == game.finish) {
                g.drawImage(exit, x * cellSize, Math.abs(y - 18) * cellSize, cellSize, cellSize, null);
            }

        }

        public void PaintCharacter(Graphics g) {
            g.drawImage(eksplorator, xCharacter * cellSize, Math.abs(yCharacter-18) * cellSize, cellSize, cellSize, null);
        }


        @Override
        public void paint (Graphics g) {

            for (int y=0; y<game.dim(); y++) {
                for (int x=0; x<game.dim(); x++) {
                    PaintCell(x,y,g);
                }

            }

            PaintCharacter(g);

        }
    }
// ===== ===== ===== ===== =====





}