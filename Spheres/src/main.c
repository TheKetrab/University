#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "spheres.h"
#include <string.h>

/* ------------------------------
    / ----- ----- ----- /
    /      SPHERES
    / ----- ----- ----- /
    Game was written using GTK
    for semestral project of C course.

    Author: Bartlomiej Grochowski
    Index nr: 300951
 ------------------------------ */


int main(int argc,char *argv[])
{

    /* --- === MAIN === --- */
    srand(time(NULL));
    gtk_init (&argc, &argv);
    if (argc > 1 && strcmp(argv[1],"-d")==0)
        logsAllowed = 1;
        
    /* --- Start program --- */
    buildUI();
    startGame();
    gtk_main ();

    return 0;
}


void buildUI()
{
    /* --- CSS -- */
    GtkCssProvider* Provider = gtk_css_provider_new();
    GdkDisplay* Display = gdk_display_get_default();
    GdkScreen* Screen = gdk_display_get_default_screen(Display);

    gtk_style_context_add_provider_for_screen(Screen, GTK_STYLE_PROVIDER(Provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(Provider), "src/mystyle.css", NULL);

    /* --- Window --- */
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"SPHERES");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 30);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);


    /* --- === BOXES === --- */

    /* --- Main box --- */

    GtkWidget *outsideBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(window), outsideBox);

    GtkWidget *headBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(outsideBox), headBox);
    GdkPixbuf *headerPix = gdk_pixbuf_new_from_file_at_size ("./data/title.png",225,65,NULL);
    GtkWidget *header = gtk_image_new_from_pixbuf (headerPix);

    gtk_box_pack_start (GTK_BOX(headBox), header, TRUE, TRUE, 0);

    GtkWidget *preMainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start (GTK_BOX(outsideBox), preMainBox, TRUE, TRUE, 0);

    GtkWidget *mainBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start (GTK_BOX(preMainBox), mainBox, TRUE, TRUE, 0);


    /* --- Highscores --- */

    GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_box_pack_start(GTK_BOX(mainBox), box1, TRUE, TRUE, 0);

    yourScoreWidget = gtk_label_new("YOUR SCORE: 0");
    GtkWidget *highScoresLabel = gtk_label_new("HIGH SCORES:");
    GtkWidget *boxForHSGrid = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    GtkWidget *HSGrid = gtk_grid_new();

    gtk_box_pack_start(GTK_BOX(boxForHSGrid), HSGrid, TRUE, TRUE, 0);
    gtk_grid_set_row_spacing(GTK_GRID(HSGrid), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(HSGrid), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(HSGrid), 5);
    gtk_grid_set_column_homogeneous(GTK_GRID(HSGrid), FALSE);

    for(int i=0; i<10; i++)
        bestPlayers[i]=initializeData();

    for(int i=0; i<10; i++) {
        char napis[2];
        sprintf(napis,"%d",i+1);
        bestPlayers[i]->first = gtk_label_new(napis);
        gtk_grid_attach(GTK_GRID(HSGrid), bestPlayers[i]->first, 1, i, 1, 1);
    }

    for(int i=0; i<10; i++) {
        bestPlayers[i]->second = gtk_label_new("...");
        gtk_grid_attach(GTK_GRID(HSGrid), bestPlayers[i]->second, 2, i, 5, 1);
    }

    for(int i=0; i<10; i++) {
        bestPlayers[i]->third = gtk_label_new("...");
        gtk_grid_attach(GTK_GRID(HSGrid), bestPlayers[i]->third, 7, i, 3, 1);
    }

    gtk_container_add(GTK_CONTAINER(box1), yourScoreWidget);
    gtk_container_add(GTK_CONTAINER(box1), highScoresLabel);
    gtk_container_add(GTK_CONTAINER(box1), boxForHSGrid);

    initializeHighScores(bestPlayers);

    /* --- Game --- */
    GtkWidget *box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(mainBox), box2, TRUE, TRUE, 0);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_box_pack_start(GTK_BOX(box2), grid, TRUE, TRUE, 0);

    initializeElems();
    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            char color[2];
            sprintf(color," ");
            button[j][i] = gtk_button_new_with_label(color);
            g_signal_connect(G_OBJECT(button[j][i]), "clicked",G_CALLBACK(clickOnSphere),NULL);
            gtk_grid_attach(GTK_GRID(grid), button[j][i], j, i, 1, 1);
        }
    }

    /* --- Menu --- */
    GtkWidget *box3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(mainBox), box3, TRUE, TRUE, 0);

    GtkWidget *newGameButton = gtk_button_new_with_label ("NEW GAME");
    GtkWidget *loadGameButton = gtk_button_new_with_label ("LOAD GAME");
    GtkWidget *saveGameButton = gtk_button_new_with_label ("SAVE GAME");
    GtkWidget *helpButton = gtk_button_new_with_label ("HELP");
    GtkWidget *exitButton = gtk_button_new_with_label ("EXIT");
    GtkWidget *nextSpheresLabel = gtk_label_new("Next spheres:");
    nextSpheresColors = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    GdkPixbuf *spherePix = gdk_pixbuf_new_from_file_at_size ("./data/sphere1.png",30,30,NULL);
    next1 = gtk_image_new_from_pixbuf (spherePix);
    next2 = gtk_image_new_from_pixbuf (spherePix);
    next3 = gtk_image_new_from_pixbuf (spherePix);

    gtk_container_add(GTK_CONTAINER(nextSpheresColors), next1);
    gtk_container_add(GTK_CONTAINER(nextSpheresColors), next2);
    gtk_container_add(GTK_CONTAINER(nextSpheresColors), next3);

    gtk_box_pack_start (GTK_BOX(box3), newGameButton, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(box3), loadGameButton, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(box3), saveGameButton, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(box3), helpButton, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(box3), exitButton, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(box3), nextSpheresLabel, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(box3), nextSpheresColors);

    g_signal_connect(G_OBJECT(newGameButton), "clicked",G_CALLBACK(newGame),NULL);
    g_signal_connect(G_OBJECT(loadGameButton), "clicked",G_CALLBACK(loadGame),NULL);
    g_signal_connect(G_OBJECT(saveGameButton), "clicked",G_CALLBACK(saveGame),NULL);
    g_signal_connect(G_OBJECT(helpButton), "clicked", G_CALLBACK(showHelpDialog), (gpointer)window);
    g_signal_connect(G_OBJECT(exitButton),"clicked",G_CALLBACK(gtk_main_quit),NULL);

    gtk_widget_show_all(window);

}