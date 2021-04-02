#include <iostream>
#include "wyd.hpp"

using namespace std;


/// ----- ----- -----
/// konstruktory
/// ----- ----- -----

Wydarzenie::Wydarzenie(DataGodz dg, string nazwa) {

    this->dg = dg;
    this->nazwa = nazwa;

}


Wydarzenie::Wydarzenie(const Wydarzenie &w) {

    nazwa = w.nazwa;
    dg = w.dg;
}

Wydarzenie::Wydarzenie(Wydarzenie &w) {

    nazwa = w.nazwa;
    dg = w.dg;
}

Wydarzenie& Wydarzenie::operator= (const Wydarzenie &w) {

    nazwa = w.nazwa;
    dg = w.dg;

    return *this;
}



/// ----- ----- -----
/// inne
/// ----- ----- -----

ostream & operator << (ostream &wy, const Wydarzenie &w)
{
    wy<<w.nazwa<<" - "<<w.dg;
    return wy;

}

bool operator<(const Wydarzenie& w1, const Wydarzenie& w2) {
    return (w1.dg<w2.dg);
}


