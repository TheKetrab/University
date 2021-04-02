#include <iostream>
#include "wyr.hpp"

using namespace std;

int main()
{


    // pi-(2+x*7)
    wyrazenie *w1 = new odejmij(
        new pi(),
        new dodaj(
            new liczba(2),
            new mnoz(
                new zmienna("x"),
                new liczba(7)
            )
        )
    );


    // ((x-1)*x)/2
    wyrazenie *w2 = new dziel(
        new mnoz(
            new odejmij(
                new zmienna("x"),
                new liczba(1)
            ),
            new zmienna("x")
        ),
        new liczba(2)
    );

    // (3+5)/(2+x*7)
    wyrazenie *w3 = new dziel(
        new dodaj(
            new liczba(3),
            new liczba(5)
            ),
        new dodaj(
            new liczba(2),
            new mnoz(
                new zmienna("x"),
                new liczba(7)
            )
        )
    );


    // 2+x*7-(y*3+5)
    wyrazenie *w4 = new odejmij(
        new dodaj(
            new liczba(2),
            new mnoz(
                new zmienna("x"),
                new liczba(7)
            )
        ),
        new dodaj(
            new mnoz(
                new zmienna("y"),
                new liczba(3)
            ),
            new liczba(5)
        )
    );


    // cos((x+1)*x)/e^x^2
    wyrazenie *w5 = new dziel(
        new cos(
            new mnoz(
                new dodaj(
                    new zmienna("x"),
                    new liczba(1)
                ),
                new zmienna("x")
            )
        ),
        new potega(
            new pi,
            new potega(
                new zmienna("x"),
                new liczba(2)
            )
        )

    );


    /// WYPISZ
    cout<<w1->opis()<<endl;
    cout<<w2->opis()<<endl;
    cout<<w3->opis()<<endl;
    cout<<w4->opis()<<endl;
    cout<<w5->opis()<<endl;

    zmienna::add_val("x",1);
    zmienna::add_val("y",1);

    wyrazenie *tab[5] = { w1, w2, w3, w4, w5 };

    for(int i=0; i<5; i++) {
        cout<<"----- ----- -----"<<endl;
        cout<<"WYRAZENIE w"<<i+1<<endl;
        for(double j=0; j<=1; j+=0.01) {
            zmienna::set_val("x",j);
            cout<<tab[i]->oblicz()<<endl;

        }
    }


    return 0;
}

