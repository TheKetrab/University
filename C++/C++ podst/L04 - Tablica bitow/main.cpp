#include <iostream>
#include "tabbit.hpp"

using namespace std;

int main()
{
    // REF NIE DZIALA !!!



    /*
    tab_bit t(46); // tablica 46-bitowa (zainicjalizowana zerami)
    uint64_t a = 45;
    tab_bit u(a); // tablica 64-bitowa (sizeof(uint64_t)*8)
    tab_bit v(t); // tablica 46-bitowa (skopiowana z t)
    tab_bit w(tab_bit(8)={1, 0, 1, 1, 0, 0, 0, 1 }); // tablica 8-bitowa (przeniesiona)
//    v[0] = 1; // ustawienie bitu 0-go bitu na 1
//    t[45] = true; // ustawienie bitu 45-go bitu na 1
//    bool b = t[1]; // odczytanie bitu 1-go
    u[45] = u[46] = u[63]; // przepisanie bitu 63-go do bitow 45-go i 46-go
    cout<<t<<endl; // wysietlenie zawartości tablicy bitów na ekranie

    tab_bit y(1);

    cin>>y;
    tab_bit x = {0,0,0,1,1,1};

    cout<<x<<endl;
    cout<<y<<endl;


    cout<<"KONIEC PROGRAMU"<<endl;

*/

    // MOJE TESTY
    tab_bit tx(1);
    tab_bit ty(1);

    uint64_t a = 450;
    uint64_t b = 66;

    tab_bit ta(a);
    tab_bit tb(b);


    cout<<ta<<" < -- TA"<<endl;
    cout<<tb<<" < -- TB"<<endl;

    cout<<(ta&tb)<<" <-- KONIUNKCJA"<<endl;
    cout<<(ta|tb)<<" <-- ALTERNATYWA"<<endl;
    cout<<(ta^tb)<<" <-- XOR"<<endl;
    cout<<(!ta)<<" < -- NOT TA"<<endl;

    cout<<"Podaj dwa ciagi zerojedynkowe (tej samej, ale dowolnej dlugosci)."<<endl;
    cin>>tx;
    cin>>ty;

    cout<<tx<<" <-- TX"<<endl;
    cout<<ty<<" <-- TY"<<endl;

    cout<<(tx&ty)<<" <-- KONIUNKCJA"<<endl;
    cout<<(tx|ty)<<" <-- ALTERNATYWA"<<endl;
    cout<<(tx^ty)<<" <-- XOR"<<endl;
    cout<<(!tx)<<" <-- NOT TX"<<endl;

/*
    tab_bit x = {0,0,1,1,0};
    tab_bit y = {1,1,1,0,0};

    x&=y;
    cout<<x;
*/
    return 0;
}
