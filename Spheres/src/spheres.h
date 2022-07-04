#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* --- === STRUCTS === --- */

/// Element of an array
typedef struct elem {
    _Bool isFree; // 1 yes, 0 no
    int color; //0 empty, 1,2,3,4,5,6,7,8-mix
    GtkWidget *image;
} Elem;

/// Record of high scores table
typedef struct record {
    char name[20];
    int score;
} Record;

/// Block of pointers to widgets that you can send as param
typedef struct data {
    GtkWidget *first;
    GtkWidget *second;
    GtkWidget *third;
    GtkWidget *fourth;
    GtkWidget *fiveth;
    int number;
} Data;

/* --- === GLOBAL === --- */

Data *bestPlayers[10];
Elem tab[9][9]; //x,y
Record scoreArray[10];

_Bool logsAllowed;
_Bool reachable; // is it possible to send sphere to this point?
int totalScore;
int removingSpheres_1;
int removingSpheres_2;
int removingSpheres_3;
int removingSpheres_4;
int searchCounter;
int occupiedElems; // how many elems in array is occupied?
int first,second,third; // colors of spheres
int searchArray[9][9]; // 0,1,2,3,4,5-(startPoint)
int currentColor; // it has to be remembered, to not join two different executing the same function rec_nr

GtkWidget *next1;
GtkWidget *next2;
GtkWidget *next3;
GtkWidget *grid;
GtkWidget *button[9][9];
GtkWidget *actualPressed;
GtkWidget *nextSpheresColors;
GtkWidget *yourScoreWidget;

/* --- === FUNCTIONS === --- */
void buildUI();

/* --- GAME --- */
_Bool isFree(int x, int y);
void addScore(int ammount);
void randomizeSpheres(int *x, int *y, int *z);
void insertSphere(int k, int x, int y); // color, coordinates
void removeSphere(int x, int y); // coordinates
void randomizeSpheresPositions(int *x, int *y, int *z);
void startGame();
void isReachableRec(int x, int y, int a, int b); // startpoint, destpoint
_Bool isReachable(int x, int y, int a, int b); // startpoint, destpoint
void moveSphere(GtkWidget *sphere1, GtkWidget *sphere2);
void tryToRemoveSpheresRec_1(int k, int x, int y);
void tryToRemoveSpheresRec_2(int k, int x, int y);
void tryToRemoveSpheresRec_3(int k, int x, int y);
void tryToRemoveSpheresRec_4(int k, int x, int y);
_Bool tryToRemoveSpheres(int k, int x, int y);

/* --- SYSTEM --- */
void gameover();
void initializeHighScores(Data *best[]);
void initializeElems();
void clickOnSphere(GtkWidget *clicked, gpointer data);
void saveGame();
void loadGame();
void newGame();
void saveHighScores();
void showGameOverDialog(int uzbierany_wynik);
void passText( GtkWidget *widget, Data *data );
Data *initializeData();
void showHelpDialog();
void printLog(const char* str);
