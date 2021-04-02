#include <utility>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "wyr.hpp"


/// * ----- * ----- * ----- * ----- *


/// operator1arg
operator1arg::operator1arg( wyrazenie *l ) {
    left = l;
}

/// operator2arg
operator2arg::operator2arg( wyrazenie *l, wyrazenie *r )
    : operator1arg(l) {
    right = r;
}

/// * ----- * ----- * ----- * ----- *


/// liczba
liczba::liczba( double x ) {
    this->x = x;
}

double liczba::oblicz() const {
    return x;
}

std::string liczba::opis() const {
    return std::to_string(x);
}

/// stala
stala::stala( double x ) : x(x) {}

double stala::oblicz() const {
    return x;
}

std::string stala::opis() const {
    return std::to_string(x);
}

/// pi
pi::pi() : stala(3.14159) {}
std::string pi::opis() const {
    return "pi";
}

/// e
e::e() : stala(2.71828) {}
std::string e::opis() const {
    return "e";
}

/// fi
fi::fi() : stala(1.61803) {}
std::string fi::opis() const {
    return "fi";
}


/// zmienna
std::vector< std::pair <std::string,double>> zmienna::vec;

zmienna::zmienna(std::string s) { this->s = s; }

double zmienna::get_val(std::string s) {

    for (auto x : vec)
        if (s == x.first) return x.second;

    vec.push_back({s,0});
    return vec.back().second;
}

void zmienna::set_val(std::string s, double val) {

    for (auto &x : vec)
        if (s == x.first) {
            x.second=val;
            return;
        }
    std::cerr<<"Nie znaleziono klucza: "<<s<<std::endl;

}

void zmienna::add_val(std::string s, double val) {
    vec.push_back({s,val});
}


double zmienna::oblicz() const {
    get_val(s);
}

std::string zmienna::opis() const {
    return s;
}



/// sin
sin::sin(wyrazenie *w) : operator1arg(w) {}

double sin::oblicz() const {
    return std::sin( left->oblicz() );
}

std::string sin::opis() const {
    return "sin(" + left->opis() + ")";
}

/// cos
cos::cos(wyrazenie *w) : operator1arg(w) {}

double cos::oblicz() const {
    return std::cos( left->oblicz() );
}

std::string cos::opis() const {
    return "cos(" + left->opis() + ")";
}

/// exp
exp::exp(wyrazenie *w) : operator1arg(w) {}

double exp::oblicz() const {
    return std::exp( left->oblicz() );
}

std::string exp::opis() const {
    return "exp(" + left->opis() + ")";
}

/// ln
ln::ln(wyrazenie *w) : operator1arg(w) {}

double ln::oblicz() const {
    return std::log( left->oblicz() );
}

std::string ln::opis() const {
    return "ln(" + left->opis() + ")";
}

/// bezwzgl
bezwzgl::bezwzgl(wyrazenie *w) : operator1arg(w) {}

double bezwzgl::oblicz() const {
    return std::abs( left->oblicz() );
}

std::string bezwzgl::opis() const {
    return "|" + left->opis() + "|";
}

/// odwrot
odwrot::odwrot(wyrazenie *w) : operator1arg(w) {}

double odwrot::oblicz() const {
    return (1.0) / ( left->oblicz() );
}

std::string odwrot::opis() const {
    return "1/(" + left->opis() + ")";
}

/// przeciw
przeciw::przeciw(wyrazenie *w) : operator1arg(w) {}

double przeciw::oblicz() const {
    return (-1.0) * ( left->oblicz() );
}

std::string przeciw::opis() const {
    return "-(" + left->opis() + ")";
}

/// * ----- * ----- * ----- * ----- *


/// dodaj
dodaj::dodaj(wyrazenie *w1, wyrazenie *w2) : operator2arg(w1,w2) {}

double dodaj::oblicz() const {
    return (left->oblicz() + right->oblicz() );
}

std::string dodaj::opis() const {
    return "(" + left->opis() + " + " + right->opis() + ")";
}

/// odejmij
odejmij::odejmij(wyrazenie *w1, wyrazenie *w2) : operator2arg(w1,w2) {}

double odejmij::oblicz() const {
    return (left->oblicz() - right->oblicz() );
}

std::string odejmij::opis() const {
    return "(" + left->opis() + " - " + right->opis() + ")";
}


/// mnoz
mnoz::mnoz(wyrazenie *w1, wyrazenie *w2) : operator2arg(w1,w2) {}

double mnoz::oblicz() const {
    return (left->oblicz() * right->oblicz() );
}

std::string mnoz::opis() const {
    return "(" + left->opis() + " * " + right->opis() + ")";
}

/// dziel
dziel::dziel(wyrazenie *w1, wyrazenie *w2) : operator2arg(w1,w2) {}

double dziel::oblicz() const {
    return (left->oblicz() / right->oblicz() );
}

std::string dziel::opis() const {
    return "(" + left->opis() + " / " + right->opis() + ")";
}


/// logarytm
logarytm::logarytm(wyrazenie *w1, wyrazenie *w2) : operator2arg(w1,w2) {}

double logarytm::oblicz() const {
    return log( right->oblicz() ) / log( left->oblicz() );
}

std::string logarytm::opis() const {
    return "log_[" + left->opis() + "](" + right->opis() + ")";
}

/// potega
potega::potega(wyrazenie *w1, wyrazenie *w2) : operator2arg(w1,w2) {}

double potega::oblicz() const {
    return pow( left->oblicz() , right->oblicz() );
}

std::string potega::opis() const {
    return "(" + left->opis() + ")^[" + right->opis() + "]";
}



/// modulo
modulo::modulo(wyrazenie *w1, wyrazenie *w2) : operator2arg(w1,w2) {}

double modulo::oblicz() const {
    return ( ((int)left->oblicz()) % ((int)right->oblicz()) );
}

std::string modulo::opis() const {
    return "(_" + left->opis() + "%_" + right->opis() + ")";
}
