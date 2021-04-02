#include <iostream>
#include "wym.hpp"

using namespace std;
using namespace obliczenia;

// ULAMEK -1 / 4983724 byl obliczany 4 minuty.
// ale sie policzyl!


int main()
{
    Wymierna x(22,66);
    Wymierna y(3,5);
    cout<<"1/3 + 3/5 = "; (x+y).wypisz();
    cout<<"1/3 - 3/5 = "; (x-y).wypisz();
    cout<<"1/3 * 3/5 = "; (x*y).wypisz();
    cout<<"1/3 / 3/5 = "; (x/y).wypisz();


    Wymierna z(-3);
    cout<<"!(-3/1) = "; (!z).wypisz();

    Wymierna a(-1,6);
    cout<<"- (-1/6) = "; (-a).wypisz();

    (Wymierna(1,2) + Wymierna(1,2)).wypisz();

    //Wymierna b(1,-2);
    //cout<<b;

    cout<<endl<<"OPERATOR <<"<<endl;
    cout<<Wymierna(1,3)<<endl;
    cout<<Wymierna(1,5)<<endl;
    cout<<Wymierna(2,1)<<endl;
    cout<<Wymierna(234,888)<<endl;

    return 0;
}
