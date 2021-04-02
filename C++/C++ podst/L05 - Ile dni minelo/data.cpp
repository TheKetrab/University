#include <iostream>
#include <ctime>
#include <cmath>
#include "data.hpp"

using namespace std;


/// ----- ----- -----
/// konstruktory
/// ----- ----- -----

Data::Data(int d, int m, int r) {

    if(poprawna(d,m,r)) {
        dzien = d;
        miesiac = m;
        rok = r;
    }

    else throw invalid_argument("Podana data nie jest poprawna!");
}

Data::Data() {

    time_t czas;
    struct tm *aktualnyczas;

    time(&czas);
    aktualnyczas = localtime(&czas);
    rok = (1900 + aktualnyczas->tm_year);
    dzien = (0 + aktualnyczas->tm_mday);
    miesiac = (1 + aktualnyczas->tm_mon);
}

Data::~Data() {
    rok = 0;
    miesiac = 1;
    dzien = 1;
}



/// ----- ----- -----
/// tablice
/// ----- ----- -----

int Data::dniwmiesiacach[2][13] = {
    {0,31,28,31,30,31,30,31,31,30,31,30,31}, // lata zwykłe
    {0,31,29,31,30,31,30,31,31,30,31,30,31} // lata przestępne
};

int Data::dniodpoczroku[2][13] = {
    {0,31,59,90,120,151,181,212,243,273,304,334,365}, // lata zwykłe
    {0,31,60,91,121,152,182,213,244,274,305,335,366} // lata przestępne
};




/// ----- ----- -----
/// metody
/// ----- ----- -----

int Data::get_dzien() { return dzien; }
int Data::get_miesiac() { return miesiac; }
int Data::get_rok() { return rok; }

bool Data::przestepny(int rok) {

    if(rok%400==0) return true;
    else if(rok%100==0) return false;
    else if(rok%4==0) return true;
    else return false;
}


void Data::wypisz_date() {

    cout<<dzien<<" "<<miesiac<<" "<<rok<<endl;
}

bool Data::poprawna(int d, int m, int r) {

    bool sukces = true;

    //inicjacja roku
    if(r<0) {
        cerr<<"Rok nie moze byc < niz 0."<<endl;
        sukces = false;
    }

    //inicjacja miesiaca
    if(m<=0 || m>12) {
        cerr<<"Miesiac powinien byc liczba z przedzialu [1,12]."<<endl;
        sukces = false;
    }

    //inicjacja dnia
    if(d<=0) {
        cerr<<"Dzien nie moze byc liczba niedodatnia!"<<endl;
        sukces = false;
    }

    else if(przestepny(r) && d>dniwmiesiacach[1][m]) {
        cerr<<"Miesiac nie ma tyle dni! (rok przestepny)"<<endl;
        sukces = false;
    }

    else if(!przestepny(r) && d>dniwmiesiacach[0][m]) {
        cerr<<"Miesiac nie ma tyle dni! (rok zwykly)"<<endl;
        sukces = false;
    }

    //sukces?
    if(sukces) return true;
    return false;
}

int Data::iledniuplynelo() const {


    /*

    // NIE DZIALA DOBRZE...

    // chcemy zamienic date na dni...

    const int dniw400 = 146097;
    const int dniw100 = 36525;
    const int dniw4 = 1461;
    const int dniw1 = 365; // rok zwykly

    int r = rok;
    int wynik = 0;

    // kolejno dzielimy...
    wynik += dniw400 * (int)(r/400);
    r = r%400;



    bool rok_n_setny = false;
    if(r/100>=1) rok_n_setny = true;
    bool rok_dokladnie_setny = false;
    if(r%100==0) rok_dokladnie_setny = true;

    wynik += dniw100 * (int)(r/100);
    r = r%100;


    wynik += dniw4 * (int)(r/4);
    r = r%4;



    cout<<"Raport dni: "<<wynik<<endl;

    // rok n-setny to A*400 + B*100 + C, gdzie b>0
    // jesli jest n-setny, to trzeba odjac,
    // bo wtedy ta jedna czworka nie ma 366,
    // a my ja liczymy. jednak jesli rok jest dokladnie
    // setny (np. 200, 1300, itp), to nie mozna odejmowac,
    // a komputer dobrze wyliczy dni korzystajac z tablicy

    if(rok_n_setny==true && rok_dokladnie_setny==false) { cout<<"ROK N-SETNY!"<<endl; wynik--; }

    // jesli koniec to spoko - jesli dni<366
    if(r!=0) {


    //else
        wynik += dniw1 * (int)(r/1);
        wynik++; // bo tten pierwszy rok jest rokiem przestepnym, musimy go uwzglednic
    }
    //r = r%1;
    cout<<"Raport dni: "<<wynik<<endl;

    // --- lata skonczone
    // --> teraz dni


    wynik += dniodpoczroku[ przestepny(rok) ][ miesiac-1 ];
    wynik += dzien-1; // -1, bo liczymy od 1 stycznia 0r
    cout<<"WYNIK: "<<wynik<<endl;
    return wynik;


    */


    // ILE DNI UPLYNELO
    return iledniuplynelo2();


}

int Data::iledniuplynelo2() const {

    int r = 0;
    int wynik=0;
    while(r < rok) {
        //cout<<"rok, wynik: "<<r<<" "<<wynik<<endl;
        if(przestepny(r)) wynik+=366;
        else wynik+=365;

        r++;
    }

    wynik+=dniodpoczroku[ przestepny(rok) ][ miesiac-1 ];
    wynik+=dzien-1;

    return wynik;

}



Data& Data::jakadata(int dni) {


    int r=0;
    int lprze = 0;
    int lzwy = 0;

    while(dni>=366) {
        //cout<<"rok, dni:"<<r<<" "<<dni<<endl;
        if(przestepny(r)) { dni-=366; lprze++; }
        else { lzwy++; dni-=365; }
        r++;

    }

    if(!przestepny(r) && dni>=365) {
        dni-=365;
        r++;
    }

    miesiac = 1;
    dzien = 1;

    md(miesiac,dzien,dni,przestepny(r));
    rok = r;

    return *this;

}

void Data::md (int &m, int &d, int dni, bool przest) {


    // dni - ile dni pozostalo do zamiany na mies i dni
    // przest - czy szukamy w roku przestepnym

    int i=1;

    // znajdywanie miesiaca
    while(dni >= (int)(dniodpoczroku[przest][i])) i++;

    m = i;

    // redukcja dni z uzyciem poprzedniego miesiaca
    dni = dni - dniodpoczroku[przest][m-1];
    d += dni;


}


/// ----- ----- -----
/// operatory
/// ----- ----- -----


int Data::operator- (const Data &d) {

    return abs( iledniuplynelo() - d.iledniuplynelo() );
}

bool Data::ostatnidzienroku() {
    if (miesiac==12 && dzien==31) return true;
    return false;
}

bool Data::ostatnidzienmiesiaca() {
    if(dzien == dniwmiesiacach[ przestepny(rok) ][ miesiac ]) return true;
    return false;
}

bool Data::pierwszydzienroku() {
    if(dzien==1 && miesiac==1) return true;
    return false;
}

bool Data::pierwszydzienmiesiaca() {
    if(dzien==1) return true;
    return false;
}


Data & Data::operator++ () {

    if(ostatnidzienroku()) {
        dzien = 1;
        miesiac = 1;
        rok++;
        return *this;
    }

    else if(ostatnidzienmiesiaca()) {
        dzien = 1;
        miesiac++;
        return *this;
    }

    else {
        dzien++;
        return *this;
    }

}

Data Data::operator++ (int) {

    Data result(*this);
    ++(*this);
    return result;
}


Data & Data::operator-- () {

    if(pierwszydzienroku()) {
        dzien = 31;
        miesiac = 12;
        rok--;
        return *this;
    }

    else if(pierwszydzienmiesiaca()) {
        dzien = dniwmiesiacach[ przestepny(rok) ][miesiac-1];
        miesiac--;
        return *this;
    }

    else {
        dzien--;
        return *this;
    }

}

Data Data::operator-- (int) {

    Data result(*this);
    --(*this);
    return result;
}

Data & Data::operator+= (int d) {

    int uplynelo = iledniuplynelo();
    uplynelo+=d;

    return jakadata(uplynelo);
}

Data & Data::operator-= (int d) {

    int uplynelo = iledniuplynelo();
    uplynelo-=d;

    return jakadata(uplynelo);
}


