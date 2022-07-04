#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "spheres.h"
#include <math.h>
#include <stdint.h>

// ***** ***** ***** ***** ***** ***** *****
//      INITIALIZATION AND OTHERS
// ***** ***** ***** ***** ***** ***** *****

void initializeElems()
{
    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            tab[j][i].isFree=1;
            tab[j][i].color=0;
            tab[j][i].image=0;
        }
    }
}

Data *initializeData()
{
    Data *res=malloc(sizeof(Data));
    res->first=NULL;
    res->second=NULL;
    res->third=NULL;
    res->fourth=NULL;
    res->fiveth=NULL;
    res->number=0;

    return res;
}

void startGame()
{
    occupiedElems=0;
    randomizeSpheres(&first,&second,&third);
    randomizeSpheresPositions(&first,&second,&third);
    randomizeSpheres(&first,&second,&third);
}

_Bool isFree(int x, int y)
{
    if(tab[x][y].isFree==1) return 1;
    return 0;
}

void gameover()
{
    printLog("GAMEOVER");
    if(totalScore > scoreArray[9].score) showGameOverDialog(totalScore);
    newGame();

}

void addScore(int count)
{
    totalScore+=count;

    int howManyDigits = floor(log10((double)totalScore)) + 1;

    char str[12 + howManyDigits];
    sprintf(str,"YOUR SCORE: %d",totalScore);
    g_object_set( G_OBJECT( yourScoreWidget ), "label", str, NULL );
}

void saveGame()
{
    printLog("--- SAVING ---");
    FILE *p = fopen("./data/savegame.txt", "w");
    if(p==NULL) printLog("Error - unable to open file");

    fprintf(p,"%d\n",totalScore);
    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++)
            fprintf(p,"%d",tab[j][i].color);
        putc('\n',p);
    }

    fclose(p);
}

void loadGame()
{
    printLog("--- LOADING ---");
    FILE *p = fopen("./data/savegame.txt", "r");
    if(p==NULL) printLog("Error - unable to open file");

    occupiedElems=0;
    int totalScore_fromFile=0;

    int c='0';
    while((c=getc(p))!='\n')
        totalScore_fromFile = (totalScore_fromFile*10)+c-'0';

    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            c=getc(p);
            if(c-'0'==0) {
                // algorithm removes sphere anyway
                removeSphere(j,i);
                occupiedElems++;
            }
            else insertSphere(c-'0',j,i);

        }

        getc(p); // newline
    }

    totalScore=totalScore_fromFile;
    char str[25];
    sprintf(str,"YOUR SCORE: %d",totalScore);
    g_object_set( G_OBJECT( yourScoreWidget ), "label", str, NULL );

    fclose(p);

}

void newGame()
{
    totalScore=0;
    char str[25];
    sprintf(str,"YOUR SCORE: %d",totalScore);
    g_object_set( G_OBJECT( yourScoreWidget ), "label", str, NULL );

    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            if(tab[j][i].color!=0) removeSphere(j,i);

    initializeElems();
    startGame();
}


void initializeHighScores(Data *best[])
{
    FILE *p=fopen("./data/highscores.txt","r");
    if(p==NULL) printLog("Error - unable to open file with high scores");

    for(int i=0; i<10; i++)
    {
        int c, playerScore=0;
        for(int j=0; (c=getc(p))!=';'; j++) {
            scoreArray[i].name[j]=c;
        }
        for(int j=0; (c=getc(p))!='\n'; j++) {
            playerScore=(playerScore*10)+c-'0';
        }
        scoreArray[i].score=playerScore;

        char xxx[20];
        sprintf(xxx,"%s",scoreArray[i].name);
        g_object_set(G_OBJECT(best[i]->second), "label", xxx, NULL );

        sprintf(xxx,"%d",scoreArray[i].score);
        g_object_set(G_OBJECT(best[i]->third), "label", xxx, NULL );

    }

    for(int i=0; i<10; i++) {
        char strInfo[50];
        sprintf(strInfo,"scoreArray[%d]: %s, %d",i,scoreArray[i].name,scoreArray[i].score);
        printLog(strInfo);
    }

    fclose(p);
}

void saveHighScores()
{
    FILE *p=fopen("./data/highscores.txt","w");
    if(p==NULL) printLog("Error - unable to open file with high scores");

    char str[50];
    for(int i=9; i>=0; i--) {
        sprintf(str,"%s;%d\n",scoreArray[i].name,scoreArray[i].score);
        fputs(str,p);
    }

    fclose(p);
}

void quicksort(Record tab[], int l, int r)
{
    int v=tab[(l+r)/2].score;
    int i,j,x; char str[20];
    i=l;
    j=r;
    do
    {
        while(tab[i].score<v) i++;
        while(tab[j].score>v) j--;
        if(i<=j)
        {
            x=tab[i].score;
            sprintf(str,"%s",tab[i].name);
            tab[i].score=tab[j].score;
            sprintf(tab[i].name,"%s",tab[j].name);
            tab[j].score=x;
            sprintf(tab[j].name,"%s",str);

            i++;
            j--;
        }
    }
    while(i<=j);
    if(j>l) quicksort(tab,l,j);
    if(i<r) quicksort(tab,i,r);
}


void showGameOverDialog(int collectedScore)
{

    /* --- Create the dialog --- */
    GtkWidget *dialog_window = gtk_dialog_new ();
    GtkWidget *dialog_label = gtk_label_new ("Write your name.");
    GtkWidget *dialog_entry = gtk_entry_new ();
    GtkWidget *dialog_button = gtk_button_new_with_label ("OK");

    /* --- Window properties --- */
    gtk_window_set_resizable (GTK_WINDOW(dialog_window),FALSE);
    gtk_window_set_position(GTK_WINDOW(dialog_window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(dialog_window), 10);

    /* --- Boxes --- */
    GtkWidget *dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog_window));

    GtkWidget *dialog_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(dialog_content), dialog_vbox);
    gtk_box_pack_start( GTK_BOX(dialog_window), dialog_vbox, TRUE, TRUE, 0 );

    gtk_container_add(GTK_CONTAINER(dialog_vbox), dialog_label);
    gtk_container_add(GTK_CONTAINER(dialog_vbox), dialog_entry);
    gtk_container_add(GTK_CONTAINER(dialog_vbox), dialog_button);

    /* --- Data --- */
    Data *dialog_data=initializeData();
    dialog_data->first = dialog_entry;
    dialog_data->second = dialog_window;
    dialog_data->number = collectedScore;

    g_signal_connect(G_OBJECT(dialog_button),"clicked",G_CALLBACK(passText),dialog_data);

    /* --- Show them all --- */
    gtk_widget_show_all(dialog_window);
}

void passText( GtkWidget *widget, Data *data )
{
    GtkWidget *data_entry = data->first;
    GtkWidget *data_window = data->second;
    sprintf(scoreArray[9].name,"%s",gtk_entry_get_text(GTK_ENTRY(data_entry)));
    gtk_widget_destroy (GTK_WIDGET (data_window));

    scoreArray[9].score = data->number;
    quicksort(scoreArray,0,9);
    saveHighScores();

    for(int i=0; i<10; i++) {
        char str[10];
        sprintf(str,"%d",scoreArray[i].score);
        g_object_set( G_OBJECT( bestPlayers[9-i]->second ), "label", scoreArray[i].name, NULL );
        g_object_set( G_OBJECT( bestPlayers[9-i]->third ), "label", str, NULL );
    }
}

void showHelpDialog()
{

    /* --- Create the dialog --- */
    GtkWidget *dialog_window = gtk_dialog_new ();
    GtkWidget *dialog_mainbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,30);
    GtkWidget *dialog_box_naglowek = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);
    GtkWidget *dialog_box_into = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);

    GtkWidget *dialog_label_0 = gtk_label_new ("HELP");
    GtkWidget *dialog_label_1 = gtk_label_new ("1. You must put 5 balls of the same color in line.");
    GtkWidget *dialog_label_2 = gtk_label_new ("2. For each broken ball you will get 1 point.");
    GtkWidget *dialog_label_3 = gtk_label_new ("3. A colourful sphere can replace any sphere.");
    GtkWidget *dialog_label_4 = gtk_label_new ("4. The game is over if the board is full.");


    /* --- Window properties --- */
    gtk_window_set_resizable (GTK_WINDOW(dialog_window),FALSE);
    gtk_window_set_position(GTK_WINDOW(dialog_window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(dialog_window), 10);

    /* --- Boxes --- */
    GtkWidget *dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog_window));

    //GtkWidget *dialog_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(dialog_content), dialog_mainbox);
    gtk_box_pack_start( GTK_BOX(dialog_window), dialog_mainbox, TRUE, TRUE, 0 );

    gtk_container_add(GTK_CONTAINER(dialog_mainbox), dialog_box_naglowek);
    gtk_container_add(GTK_CONTAINER(dialog_mainbox), dialog_box_into);

    gtk_container_add(GTK_CONTAINER(dialog_box_naglowek), dialog_label_0);
    gtk_container_add(GTK_CONTAINER(dialog_box_into), dialog_label_1);
    gtk_container_add(GTK_CONTAINER(dialog_box_into), dialog_label_2);
    gtk_container_add(GTK_CONTAINER(dialog_box_into), dialog_label_3);
    gtk_container_add(GTK_CONTAINER(dialog_box_into), dialog_label_4);

    /* --- Show them all --- */
    gtk_widget_show_all(dialog_window);
}

void printLog(const char* str)
{
    if (logsAllowed)
        printf("%s\n",str);
}