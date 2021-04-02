#include <iostream>
#include "macierz.hpp"


using namespace std;
using namespace obliczenia;

int main(int argc, char *argv[])
{
/*

    // TRANSPOZYCJA
    cout<<"Przyklad: "<<endl;

    cout<<"A:"<<endl;
    Macierz A("(_2_3_)(_1_2_3_4_5_6_)");
    A.wypisz();
    cout<<"transponowana: "<<endl;
    (!A).wypisz();

    cout<<"B:"<<endl;
    Macierz B("(_4_2_)(_1_2_3_4_5_6_7_8_)");
    B.wypisz();
    cout<<"transponowana:"<<endl;
    (!B).wypisz();
*/
/*
    // MNOZENIE
    Macierz D("(_2_2_)(_-2_3_-4_7_)");
    cout<<"D * D.odwrotna()"<<endl;
    (D*=D.odwrotna()).wypisz();

    cout<<"E: "<<endl;
    Macierz E("(_3_3_)(_2_5_7_6_3_4_5_-2_-3_)");
    E.wypisz();

    cout<<"ODWROTNA DO E: "<<endl;
    Macierz F = E.odwrotna();
    F.wypisz();

    cout<<"---id?---"<<endl;
    E*=F;
    E.wypisz();
*/
   // return 0;

    if(argc==1) {
        cout<<"---> BRAK ARGUMENTOW"<<endl;
        cout<<"Zostana przeprowadzone domyslne testy:"<<endl;

        cout<<endl<<"----- ----- -----"<<endl;
        cout<<"MACIERZ 1:"<<endl;
        Macierz a("(_2_2_)(_4_5_1_8_)");
        a.wypisz();
        cout<<"Wyznacznik: "<<a.det()<<endl;
        cout<<"Odwrotna:"<<endl;
        (a.odwrotna()).wypisz();

        cout<<endl<<"----- ----- -----"<<endl;
        cout<<"MACIERZ 2:"<<endl;
        Macierz b("(_3_3_)(_-1_-2_4_0_1_-2_1_-3_5_)");
        b.wypisz();
        cout<<"Wyznacznik: "<<b.det()<<endl;
        cout<<"Odwrotna:"<<endl;
        (b.odwrotna()).wypisz();

        cout<<endl<<"----- ----- -----"<<endl;
        cout<<"MACIERZ 3:"<<endl;
        Macierz c("(_5_5_)(_1_1_1_1_-9_1_1_1_-9_1_1_1_-9_1_1_1_-9_1_1_1_-9_1_1_1_1_)");
        c.wypisz();
        cout<<"Wyznacznik: "<<c.det()<<endl;
        cout<<"Odwrotna:"<<endl;
        (c.odwrotna()).wypisz();

        cout<<endl<<"----- ----- -----"<<endl;
        cout<<"Test uzytkownika:"<<endl;
        Macierz m(1);
        cout<<"cin>>m "; cin>>m;
        cout<<"cout<<m "<<m<<endl;
        cout<<"Wyznacznik: "<<m.det()<<endl;
        cout<<"Odwrotna:"<<endl;
        (m.odwrotna()).wypisz();

    }

    else {

        for(int i=1; i<argc; i++) {

            cout<<endl<<"----- ----- -----"<<endl;
            cout<<"MACIERZ: "<<i<<endl;
            Macierz test(argv[i]);
            test.wypisz();
            cout<<"Wyznacznik: "<<test.det()<<endl;
            cout<<"Odwrotna:"<<endl;
            (test.odwrotna()).wypisz();
        }


    }

    return 0;
}
