#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "spheres.h"
#include <stdint.h>


// ***** ***** ***** ***** ***** ***** *****
//      ALGORITHMS OF SEARCHING AND SETTING
// ***** ***** ***** ***** ***** ***** *****

void printSearchingTable()
{
    printf("-----\n");
    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++)
            printf("%d",searchArray[j][i]);

        putchar('\n');
    }
    printf("-----\n");
}

void randomizeSpheres(int *x, int *y, int *z)
{

    *x=(rand()%8)+1;
    *y=(rand()%8)+1;
    *z=(rand()%8)+1;

    char str[25];
    GdkPixbuf *pixbuf;

    sprintf(str,"./data/sphere%d.png",(int8_t)*x);
    pixbuf = gdk_pixbuf_new_from_file_at_size (str,30,30,NULL);
    gtk_image_set_from_pixbuf (GTK_IMAGE(next1), pixbuf);

    sprintf(str,"./data/sphere%d.png",(int8_t)*y);
    pixbuf = gdk_pixbuf_new_from_file_at_size (str,30,30,NULL);
    gtk_image_set_from_pixbuf (GTK_IMAGE(next2), pixbuf);

    sprintf(str,"./data/sphere%d.png",(int8_t)*z);
    pixbuf = gdk_pixbuf_new_from_file_at_size (str,30,30,NULL);
    gtk_image_set_from_pixbuf (GTK_IMAGE(next3), pixbuf);

}

void insertSphere(int k, int x, int y) // color, coordinates
{
    char str[25];
    str[0]='\0';
    g_object_set( G_OBJECT( button[x][y] ), "label", NULL, NULL );

    sprintf(str,"./data/sphere%d.png",k);
    GdkPixbuf *pix=gdk_pixbuf_new_from_file_at_size (str,30,30,NULL);
    tab[x][y].image=gtk_image_new_from_pixbuf(pix);
    tab[x][y].color=k;
    tab[x][y].isFree=0;
    gtk_button_set_image(GTK_BUTTON(button[x][y]),tab[x][y].image);

    occupiedElems++;

    if(occupiedElems>=81) gameover();

    char strInfo[50];
    sprintf(strInfo,"INSERTED SPHERE OF COLOR %d TO: %d %d",k,x,y);
    printLog(strInfo);
}

void removeSphere(int x, int y) // coordinates
{
    tab[x][y].color=0;
    tab[x][y].isFree=1;

    char str[1];
    str[0]='\0';

    gtk_button_set_image(GTK_BUTTON(button[x][y]),0);
    g_object_set( G_OBJECT( button[x][y] ), "label", str, NULL );

    occupiedElems--;

    char strInfo[50];
    sprintf(strInfo,"REMOVED SPHERE FROM: %d %d",x,y);
    printLog(strInfo);
}

void randomizeSpheresPositions(int *x, int *y, int *z)
{
    if(occupiedElems+3>=81) { gameover(); return; }

    // ALGORYTM WOLNY - DO POPRAWY TODO
    // chyba zrobie cos z tym, ze wszystkie isFree daje do tablicy
    // o dlugosci 81-occupiedElems i wybiera losowo z niej trzy...?

    int i,j;
    // ... x ...
    do {
        i=rand()%9;
        j=rand()%9;
    } while (tab[j][i].isFree==0);
    tab[j][i].color=*x;
    insertSphere(*x,j,i);
    tryToRemoveSpheres(tab[j][i].color,j,i);

    // ... y ...
    do {
        i=rand()%9;
        j=rand()%9;
    } while (tab[j][i].isFree==0);
    tab[j][i].color=*y;
    insertSphere(*y,j,i);
    tryToRemoveSpheres(tab[j][i].color,j,i);

    // ... z ...
    do {
        i=rand()%9;
        j=rand()%9;
    } while (tab[j][i].isFree==0);
    tab[j][i].color=*z;
    insertSphere(*z,j,i);
    tryToRemoveSpheres(tab[j][i].color,j,i);

}

void isReachableRec(int x, int y, int a, int b) // startpoint, destpoint
{
    char str[100];
    sprintf(str,"---- IS REACHABLE REC ---- args:  x=%d y=%d, destination: a=%d b=%d",x,y,a,b);
    printLog(str);

    if(x==a && y==b) { reachable=1; return; }

        searchArray[x][y]=1; // it means, that on that elem function was called

        if(reachable==0 && x+1<9 && isFree(x+1,y) && searchArray[x+1][y]==0) isReachableRec(x+1,y,a,b);
        if(reachable==0 && y+1<9 && isFree(x,y+1) && searchArray[x][y+1]==0) isReachableRec(x,y+1,a,b);
        if(reachable==0 && y-1>=0 && isFree(x,y-1) && searchArray[x][y-1]==0) isReachableRec(x,y-1,a,b);
        if(reachable==0 && x-1>=0 && isFree(x-1,y) && searchArray[x-1][y]==0) isReachableRec(x-1,y,a,b);

        return;
}

_Bool isReachable(int x, int y, int a, int b) // startpoint, destpoint
{
    // clearing searchArray
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            searchArray[j][i]=0;

    // suppose, that it's impossible to reach
    reachable=0;
    searchCounter=0;
    isReachableRec(x,y,a,b);

    return reachable;
}

void moveSphere(GtkWidget *sphere1, GtkWidget *sphere2)
{
    gint x, y; // coordinates sph1
    gint a, b; // coordinates sph2

    gtk_container_child_get (GTK_CONTAINER(grid), sphere1, "left-attach", &x, "top-attach", &y, NULL);
    gtk_container_child_get (GTK_CONTAINER(grid), sphere2, "left-attach", &a, "top-attach", &b, NULL);


    if(!(x==a && y==b) && tab[x][y].isFree==0) {
        if(isReachable(x,y,a,b)) {
            insertSphere(tab[x][y].color,a,b);
            removeSphere(x,y);

            if(tryToRemoveSpheres(tab[a][b].color,a,b)==1);
            else {
                randomizeSpheresPositions(&first,&second,&third);
                randomizeSpheres(&first,&second,&third);
            }
        }
    }

}

void tryToRemoveSpheresRec_1(int k, int x, int y)
{

    // UP
    if(y-1>=0 && (tab[x][y-1].color==k || tab[x][y-1].color==8) && searchArray[x][y-1]==0) {
        removingSpheres_1++;
        searchArray[x][y-1]=1;
        tryToRemoveSpheresRec_1(k,x,y-1);
    }

    // DOWN
    if(y+1<9 && (tab[x][y+1].color==k || tab[x][y+1].color==8) && searchArray[x][y+1]==0) {
        removingSpheres_1++;
        searchArray[x][y+1]=1;
        tryToRemoveSpheresRec_1(k,x,y+1);
    }
}

void tryToRemoveSpheresRec_2(int k, int x, int y)
{
    // RIGHT
    if(x+1<9 && (tab[x+1][y].color==k || tab[x+1][y].color==8) && searchArray[x+1][y]==0) {
        removingSpheres_2++;
        searchArray[x+1][y]=2;
        tryToRemoveSpheresRec_2(k,x+1,y);
    }

    // LEFT
    if(x-1>=0 && (tab[x-1][y].color==k || tab[x-1][y].color==8) && searchArray[x-1][y]==0) {
        removingSpheres_2++;
        searchArray[x-1][y]=2;
        tryToRemoveSpheresRec_2(k,x-1,y);
    }
}

void tryToRemoveSpheresRec_3(int k, int x, int y)
{
    // UP-RIGHT
    if(x+1<9 && y-1>=0 && (tab[x+1][y-1].color==k || tab[x+1][y-1].color==8) && searchArray[x+1][y-1]==0) {
        removingSpheres_3++;
        searchArray[x+1][y-1]=3;
        tryToRemoveSpheresRec_3(k,x+1,y-1);
    }

    // DOWN-LEFT
    if(x-1>=0 && y+1<9 && (tab[x-1][y+1].color==k || tab[x-1][y+1].color==8) && searchArray[x-1][y+1]==0) {
        removingSpheres_3++;
        searchArray[x-1][y+1]=3;
        tryToRemoveSpheresRec_3(k,x-1,y+1);
    }
}

void tryToRemoveSpheresRec_4(int k, int x, int y)
{
    // UP-LEFT
    if(x-1>=0 && y-1>=0 && (tab[x-1][y-1].color==k || tab[x-1][y-1].color==8) && searchArray[x-1][y-1]==0) {
        removingSpheres_4++;
        searchArray[x-1][y-1]=4;
        tryToRemoveSpheresRec_4(k,x-1,y-1);
    }

    // DOWN-RIGHT
    if(x+1>=0 && y+1<9 && (tab[x+1][y+1].color==k || tab[x+1][y+1].color==8) && searchArray[x+1][y+1]==0) {
        removingSpheres_4++;
        searchArray[x+1][y+1]=4;
        tryToRemoveSpheresRec_4(k,x+1,y+1);
    }
}

// unused!
void clearSearchArrayFromParam(int n)
{
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            if(searchArray[j][i]==n) searchArray[j][i]=0;
}

_Bool tryToRemoveSpheres(int k, int x, int y) // color, coordinates
{

    // clearing SearchArray
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            searchArray[j][i]=0;

    // preparation and initiation search array
    removingSpheres_1=1; // |
    removingSpheres_2=1; // -
    removingSpheres_3=1; // '/'
    removingSpheres_4=1; // '\'

    searchArray[x][y]=5; // startPoint


    // if user inserted colorful sphere
    if(k==8) {
        // then try to remove using every color
        if(tryToRemoveSpheres(1,x,y)==1) return 1;
        if(tryToRemoveSpheres(2,x,y)==1) return 1;
        if(tryToRemoveSpheres(3,x,y)==1) return 1;
        if(tryToRemoveSpheres(4,x,y)==1) return 1;
        if(tryToRemoveSpheres(5,x,y)==1) return 1;
        if(tryToRemoveSpheres(6,x,y)==1) return 1;
        if(tryToRemoveSpheres(7,x,y)==1) return 1;

    }
    // if user inserted sphere at color k
    else {
        tryToRemoveSpheresRec_1(k,x,y);
        tryToRemoveSpheresRec_2(k,x,y);
        tryToRemoveSpheresRec_3(k,x,y);
        tryToRemoveSpheresRec_4(k,x,y);
    }

    // removing spheres
    int pointsToAdd=1; // 1 point for 'startPoint'

    if(removingSpheres_1>=5) {
        removeSphere(x,y); // startPoint
        for(int i=0; i<9; i++)
            for(int j=0; j<9; j++)
                if(searchArray[j][i]==1) { removeSphere(j,i); pointsToAdd++; }
    }

    if(removingSpheres_2>=5) {
        removeSphere(x,y); // startPoint
        for(int i=0; i<9; i++)
            for(int j=0; j<9; j++)
                if(searchArray[j][i]==2) { removeSphere(j,i); pointsToAdd++; }
    }

    if(removingSpheres_3>=5) {
        removeSphere(x,y); // startPoint
        for(int i=0; i<9; i++)
            for(int j=0; j<9; j++)
                if(searchArray[j][i]==3) { removeSphere(j,i); pointsToAdd++; }
    }

    if(removingSpheres_4>=5) {
        removeSphere(x,y); // startPoint
        for(int i=0; i<9; i++)
            for(int j=0; j<9; j++)
                if(searchArray[j][i]==4) { removeSphere(j,i); pointsToAdd++; }
    }

    // ADDING POINTS TO SCORE
    if(pointsToAdd>1)
        // if added sth besides point for the 'startPoint'
        addScore(pointsToAdd);

    if(pointsToAdd>1) return 1;
    else return 0;
}


void clickOnSphere(GtkWidget *clicked, gpointer data)
{
    gint x, y; // coordinates clicked widget
    gtk_container_child_get (GTK_CONTAINER(grid), clicked, "left-attach", &x, "top-attach", &y, NULL);


    if (actualPressed==NULL && tab[x][y].isFree==0) { 
        char str[100];
        sprintf(str,"clickOnSphere %d %d: nth was pressed",x,y);
        printLog(str);
        actualPressed=clicked;
    }

    else if (actualPressed==NULL && tab[x][y].isFree==1) {
        char str[100];
        sprintf(str,"clickOnSphere %d %d: nth was pressed, you clicked on empty elem",x,y);
        printLog(str);
        actualPressed=NULL;
    }
    
    else if (actualPressed == clicked) {
        char str[100];
        sprintf(str,"clickOnSphere %d %d: you clicked on the same sphere",x,y);
        printLog(str);
    }
    
    else if(tab[x][y].isFree==0) {
        char str[100];
        sprintf(str,"clickOnSphere %d %d: you changed actual pressed sphere",x,y);
        printLog(str);
        actualPressed=clicked;
    }

    else {
        char str[100];
        sprintf(str,"clickOnSphere %d %d: sth was pressed",x,y);
        printLog(str);
        moveSphere(actualPressed,clicked);
        actualPressed=NULL;
    }

}

