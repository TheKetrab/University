#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include "wym.hpp"

namespace obliczenia {


/// konstruktory
Wymierna::Wymierna(int l, int m) throw (MianownikZero, MianownikUjemny) {

    if(m==0)
        throw MianownikZero();
    if(m<0)
        throw MianownikUjemny(l,m);

    licz=l, mian=m;
    skroc(*this);
}

Wymierna::Wymierna(int l)
    : Wymierna(l,1) {}

Wymierna::Wymierna(const Wymierna &w) {
    this->licz = w.licz;
    this->mian = w.mian;
}

Wymierna& Wymierna::operator= (const Wymierna &w) {
    this->licz = w.licz;
    this->mian = w.mian;
    return *this;
}

/// konwersje
Wymierna::operator double () {
    double x = (double)licz / (double)mian;
    return x;
}

Wymierna::operator int () {
    int x = (int)((double)*this);
    return x;
}


/// pomocniczze funkcje
bool Wymierna::przekroczy_zakres(const int a, const int b, char op) {

    using limits = std::numeric_limits <int>;

    if(op=='+') {

        if((a>0 && b>0 && a>limits::max()-b)
         ||(a<0 && b<0 && a<limits::min()-b))
             return true;
        else
            return false;
    }

    else if(op=='-') {

        if((a<0 && b>0 && a<limits::min()+b)
         ||(a>0 && b<0 && a>limits::max()+b))
             return true;
        else
            return false;
    }

    else if(op=='*') {

        if(((a>1 && b>1) && (a>limits::max()/b))
         ||((a<-1 && b<-1) && (a<limits::max()/b))
         ||((a<-1 && b>1) && (a<limits::min()/b))
         ||((a>1 && b<-1) && (a>limits::min()/b)))
            return true;
        else
            return false;
    }

    else if(op=='/') {

        //nigdy nie wyjdziemy
        return false;

    }

    else {

        std::cerr<<"Niezidentyfikowany operator "<<op<<std::endl;
        return true;
    }

}

int Wymierna::nwd(int a, int b) {

    int c;
    while(b!=0) {
        c = a%b;
        a = b;
        b = c;
    }

    return a;
}

int Wymierna::nww(int a, int b) throw (PrzekroczenieZakresu) {

    if(przekroczy_zakres(a,b,'*'))
        throw PrzekroczenieZakresu(a,b,'*');

    return (a*b)/nwd(a,b);
}

void skroc(Wymierna &w) {

    int dzielnik = abs(Wymierna::nwd(w.licz,w.mian));
    w.licz = w.licz / dzielnik;
    w.mian = w.mian / dzielnik;
}

void normalizuj(Wymierna &w1, Wymierna &w2) {

    int wielokrotnosc = Wymierna::nww(abs(w1.mian),abs(w2.mian));
    int mnoznikA = wielokrotnosc / (w1.mian);
    int mnoznikB = wielokrotnosc / (w2.mian);

    w1.licz = mnoznikA * w1.licz;
    w2.licz = mnoznikB * w2.licz;
    w1.mian = wielokrotnosc;
    w2.mian = wielokrotnosc;
}


/// operatory
Wymierna& Wymierna::operator+= (const Wymierna& w) throw (PrzekroczenieZakresu) {

    Wymierna k = w;
    normalizuj(*this,k);

    if(przekroczy_zakres(licz,k.licz,'+'))
        throw PrzekroczenieZakresu(licz,k.licz,'+');

    licz += k.licz;
    skroc(*this);
    return *this;
}

Wymierna& Wymierna::operator-= (const Wymierna& w) throw (PrzekroczenieZakresu) {

    Wymierna k = w;
    normalizuj(*this,k);

    if(przekroczy_zakres(licz,w.licz,'-'))
        throw PrzekroczenieZakresu(licz,k.licz,'-');

    licz -= k.licz;
    skroc(*this);
    return *this;
}

Wymierna& Wymierna::operator*= (const Wymierna& w) throw (PrzekroczenieZakresu) {

    if(przekroczy_zakres(licz,w.licz,'*'))
        throw PrzekroczenieZakresu(licz,w.licz,'*');

    if(przekroczy_zakres(mian,w.mian,'*'))
        throw PrzekroczenieZakresu(mian,w.mian,'*');

    licz *= w.licz;
    mian *= w.mian;
    skroc(*this);
    return *this;
}

Wymierna& Wymierna::operator/= (const Wymierna& w) throw (MianownikZero) {

    if(w.licz==0)
        throw MianownikZero();

    // pomnoz razy odwrotnosc
    Wymierna k = w;
    *this *= !k;
    return *this;
}

Wymierna& Wymierna::operator! () throw (MianownikZero) {

    if(licz==0)
        throw MianownikZero();
    if(licz<0)
        licz *= -1, mian *= -1;

    std::swap(licz,mian);
    return *this;

}

Wymierna& Wymierna::operator- () {

    licz *= -1;
    return *this;
}


Wymierna operator+ (const Wymierna &x, const Wymierna &y) {

    Wymierna result = x;
    result += y;
    return result;

}

Wymierna operator- (const Wymierna &x, const Wymierna &y) {

    Wymierna result = x;
    result -= y;
    return result;

}

Wymierna operator* (const Wymierna &x, const Wymierna &y) {

    Wymierna result = x;
    result *= y;
    return result;

}

Wymierna operator/ (const Wymierna &x, const Wymierna &y) {

    Wymierna result = x;
    result /= y;
    return result;

}

std::string Wymierna::ulamek_okresowy() const {

    int i; //iterator do sprawdzenia od kiedy jest okres
    bool okres = true;

    // wektor par: reszta z dzielenia, dzielenie
    std::vector<std::pair<int,int>> vec;

    // -----
    int reszta;
    if(licz<0)
        reszta = (-licz) % mian;
    else
        reszta = licz % mian;
    // -----

    int dzielenie;

    bool koniec = false;
    while(!koniec) {


        reszta = reszta%mian;

        if(reszta==0) { okres=false; break; }

        // szukanie w srodowisku
        i=0;
        for( ; i<(int)(vec.size()); i++)
            if(reszta == vec[i].first) {
                koniec = true;
                break;
            }

        if(koniec==true) break;

        // jesli przeszedl po calym vectorze i nie znalazl,
        // to dorzuca wartosc do vectora
        reszta*=10; //symulacja 'dopisania' zera w dzieleniu pisemnym
        dzielenie = reszta/mian;

        vec.push_back({reszta/10,dzielenie});

    }

    // WYPISYWANIE
    std::string wynik;

    // --> poczatek: czy jest ujemna?
    if(licz<0)
        wynik='-' + std::to_string((int)(-licz/mian));
    else
        wynik=std::to_string((int)(licz/mian));

    // --> czy ma cos po przecinku?
    if(licz%mian==0)
        return wynik;


    wynik+=",";

    if(okres) {
        for(int j=0; j<i; j++)
            wynik += std::to_string(vec[j].second);
        wynik+="(";
        for(int j=i; j<(int)vec.size(); j++)
            wynik += std::to_string(vec[j].second);
        wynik+=")";
    }
    else {
        for(int j=0; j<(int)vec.size(); j++)
            wynik+=std::to_string(vec[j].second);

    }

    return wynik;
}


void Wymierna::wypisz() {
    std::cout<<licz<<"/"<<mian<<std::endl;
}


std::ostream& operator<< (std::ostream &wyj, const Wymierna &w) {

    wyj<<w.ulamek_okresowy();
    return wyj;

}


WyjatekWymierny::WyjatekWymierny() {
    info = "Nieznany powod";
}

WyjatekWymierny::~WyjatekWymierny() {}

const char* WyjatekWymierny::what() const throw() {
    return info.c_str();
}

MianownikZero::MianownikZero() {
    info = "Mianownik stal sie zerem!";
}

MianownikZero::~MianownikZero() {}

const char* MianownikZero::what() const throw() {
    return info.c_str();
}

PrzekroczenieZakresu::PrzekroczenieZakresu(int a, int b, char op) {
    info = std::to_string(a) + op + std::to_string(b);
}

PrzekroczenieZakresu::~PrzekroczenieZakresu() {}

const char* PrzekroczenieZakresu::what() const throw() {
    return info.c_str();
}

MianownikUjemny::MianownikUjemny(int l, int m) {
    info = std::to_string(l) + "/" + std::to_string(m);
}

MianownikUjemny::~MianownikUjemny() {}

const char* MianownikUjemny::what() const throw() {
    return info.c_str();
}


}
