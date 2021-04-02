import java.awt.*;

public class Main {




    public static void main(String[] args) throws Exception {



        EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {

                try {
                    Game g = new Game();
                    g.PrintCorrectWay();
                    //g.PrintPtrTable();

                    GameWindow gameWindow = new GameWindow(g);
                    gameWindow.showCanvas();
                    System.out.println("Witaj w grze!");
					
                } catch (Exception e) {}

            }
        });


    }




}
