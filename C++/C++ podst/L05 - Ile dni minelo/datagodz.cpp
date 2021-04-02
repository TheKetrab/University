#include <iostream>
#include "datagodz.hpp"

using namespace std;

/// ----- ----- -----
/// konstruktory
/// ----- ----- -----

DataGodz::DataGodz(int d, int m, int r, int go, int mi, int se) {

    dzien = d;
    miesiac = m;
    rok = r;
    godzina = go;
    minuta = mi;
    sekunda = se;
}

DataGodz::DataGodz(int go, int mi, int se) {
    dzien = 1;
    miesiac = 1;
    rok = 0;
    godzina = go;
    minuta = mi;
    sekunda = se;
}

DataGodz::DataGodz() {

    time_t czas;
    struct tm *aktualnyczas;

    time(&czas);
    aktualnyczas = localtime(&czas);
    rok = (1900 + aktualnyczas->tm_year);
    dzien = (0 + aktualnyczas->tm_mday);
    miesiac = (1 + aktualnyczas->tm_mon);
    godzina = aktualnyczas->tm_hour;
    minuta = aktualnyczas->tm_min;
    sekunda = aktualnyczas->tm_sec;

}


DataGodz::DataGodz(const DataGodz &dg) {

    dzien = dg.dzien;
    miesiac = dg.miesiac;
    rok = dg.rok;
    sekunda = dg.sekunda;
    minuta = dg.minuta;
    godzina = dg.godzina;

}

DataGodz::DataGodz(DataGodz &dg) {

    dzien = dg.dzien;
    miesiac = dg.miesiac;
    rok = dg.rok;
    sekunda = dg.sekunda;
    minuta = dg.minuta;
    godzina = dg.godzina;
}

DataGodz& DataGodz::operator= (const DataGodz &dg) {

    dzien = dg.dzien;
    miesiac = dg.miesiac;
    rok = dg.rok;
    sekunda = dg.sekunda;
    minuta = dg.minuta;
    godzina = dg.godzina;

    return *this;
}


DataGodz::~DataGodz() {
    rok = 0;
    miesiac = 1;
    dzien = 1;
    godzina = 0;
    minuta = 0;
    sekunda = 0;
}





/// ----- ----- -----
/// metody
/// ----- ----- -----


int DataGodz::get_godzina() const { return godzina; }
int DataGodz::get_minuta() const { return minuta; }
int DataGodz::get_sekunda() const { return sekunda; }


ostream & operator << (ostream &wy, const DataGodz &dg)
{
    wy<<dg.dzien<<"."<<dg.miesiac<<"."<<dg.rok<<"r "<<dg.godzina<<":"<<dg.minuta<<"'"<<dg.sekunda;
    return wy;

}


uint64_t DataGodz::ilesekunduplynelo() const {

    const uint64_t sDzien = 86400;
    const uint64_t sGodz = 3600;
    const uint64_t sMin = 60;


    uint64_t wynik = 0;

    wynik += (uint64_t)(iledniuplynelo()) * sDzien;
    wynik += (uint64_t)godzina * sGodz;
    wynik += (uint64_t)minuta * sMin;
    wynik += (uint64_t)sekunda;

    return wynik;
}



/// ----- ----- -----
/// operatory
/// ----- ----- -----


int64_t operator- (const DataGodz &dg1, const DataGodz &dg2) {

    const int sDzien = 86400;
    const int sGodz = 3600;
    const int sMin = 60;


    int roznica_dni = dg1.iledniuplynelo() - dg2.iledniuplynelo();

    int roznica_w_jeden_dzien = 0;
    roznica_w_jeden_dzien += (dg1.get_godzina() - dg2.get_godzina()) * sGodz;
    roznica_w_jeden_dzien += (dg1.get_minuta() - dg2.get_minuta()) * sMin;
    roznica_w_jeden_dzien += (dg1.get_sekunda() - dg2.get_sekunda());

    int64_t wynik = abs ( sDzien * roznica_dni + roznica_w_jeden_dzien );


    return wynik;

}

DataGodz & DataGodz::operator+= (int s) {

    const int sDzien = 86400;
    const int sGodz = 3600;
    const int sMin = 60;

    int ilosc_sekund = godzina*sGodz + minuta*sMin + sekunda;
    ilosc_sekund += s;

    // JESLI PRZEKRACZA DZIEN
    if(ilosc_sekund >= sDzien) {
        cerr<<"BLAD: Dodawanie sekund przekracza dzien. Niezdefiniowane."<<endl;
        return *this;
    }

    godzina = ilosc_sekund/sGodz;
    ilosc_sekund = ilosc_sekund%sGodz;

    minuta = ilosc_sekund/sMin;
    ilosc_sekund = ilosc_sekund%sMin;

    sekunda = ilosc_sekund;

    return *this;

}

DataGodz & DataGodz::operator-= (int s) {

    const int sGodz = 3600;
    const int sMin = 60;

    int ilosc_sekund = godzina*sGodz + minuta*sMin + sekunda;
    ilosc_sekund -= s;

    // JESLI PRZEKRACZA DZIEN
    if(ilosc_sekund < 0) {
        cerr<<"BLAD: Odejmowanie sekund przekracza dzien. Niezdefiniowane."<<endl;
        return *this;
    }

    godzina = ilosc_sekund/sGodz;
    ilosc_sekund = ilosc_sekund%sGodz;

    minuta = ilosc_sekund/sMin;
    ilosc_sekund = ilosc_sekund%sMin;

    sekunda = ilosc_sekund;

    return *this;

}

DataGodz & DataGodz::operator++ () { *this+=1; }

DataGodz DataGodz::operator++ (int) {

    DataGodz result(*this);
    ++(*this);
    return result;
}

DataGodz & DataGodz::operator-- () { *this-=1; }

DataGodz DataGodz::operator-- (int) {

    DataGodz result(*this);
    --(*this);
    return result;
}


bool operator<(const DataGodz& dg1, const DataGodz& dg2) {

    if(dg1.iledniuplynelo() > dg2.iledniuplynelo()) return false;
    if(dg1.iledniuplynelo() < dg2.iledniuplynelo()) return true;

    if(dg1.godzina > dg2.godzina) return false;
    if(dg1.godzina < dg2.godzina) return true;

    if(dg1.minuta > dg2.minuta) return false;
    if(dg1.minuta < dg2.minuta) return true;

    if(dg1.sekunda > dg2.sekunda) return false;
    if(dg1.sekunda < dg2.sekunda) return true;

    // jak tu dojdzie, tzn ze sa rowne
    return false;

}

bool operator==(const DataGodz& dg1, const DataGodz& dg2) {

    if(( dg1.dzien == dg2.dzien )
    && ( dg1.miesiac == dg2.miesiac )
    && ( dg1.rok == dg2.rok )
    && ( dg1.godzina == dg2.godzina )
    && ( dg1.minuta == dg2.minuta )
    && ( dg1.sekunda == dg2.sekunda ))
    return true;

    return false;
}

