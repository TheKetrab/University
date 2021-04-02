#include <iostream>
#include <cmath>
#include "pwp.hpp"
#include "test.hpp"

#define PI 3.14159265

using namespace std;


// ----- ----- ----- -----
// Metody klasy punkt
// ----- ----- ----- -----

punkt::punkt(double a, double b) : x(a), y(b) {}
punkt::punkt(punkt a, wektor v) : x(a.x + v.dx), y(a.y + v.dy) {}
punkt::punkt(punkt &p) : x(p.x), y(p.y) {}

// ----- ----- ----- -----
// Metody klasy wektor
// ----- ----- ----- -----

wektor::wektor(double a, double b) : dx(a), dy(b) {}
wektor::wektor(wektor &w) : dx(w.dx), dy(w.dy) {}

// ----- ----- ----- -----
// Metody klasy prosta
// ----- ----- ----- -----

prosta::prosta(punkt a, punkt b) {
    if(a.x == b.x && a.y == b.y)
        throw invalid_argument("nie mozna jednoznacznie utworzyc prostej");

    this->a = b.y - a.y;
    this->b = a.x - b.x;
    this->c = b.x*a.y - a.x*b.y;

    double m=mi(this->a,this->b,this->c);
    this->a = m*this->a;
    this->b = m*this->b;
    this->c = m*this->c;
}

prosta::prosta(prosta &l, wektor v) {

    // rownanie nowej prostej:
    // Ax + By + (C-Ap-Bq) = 0

    this->a = l.a;
    this->b = l.b;
    this->c = l.c - (l.a * v.dx) - (l.b * v.dy);

    double m=mi(this->a,this->b,this->c);
    this->a = m*this->a;
    this->b = m*this->b;
    this->c = m*this->c;
}

prosta::prosta(wektor v) {

    if(v.dx == 0 && v.dy == 0)
        throw invalid_argument("nie mozna utworzyc prostej na podstawie wektora zerowego");

    // wektor [A,B] jest prostopadly
    // do prostej Ax + By + C = 0

    // ma przechodzic przez punkt
    // (dx,dy) czyli C = -A*dx - B*dy

    this->a = v.dx;
    this->b = v.dy;
    this->c = (-1.0) * (v.dx * v.dx + v.dy * v.dy);

    double m=mi(this->a,this->b,this->c);
    this->a = m*this->a;
    this->b = m*this->b;
    this->c = m*this->c;
}

prosta::prosta(double a, double b, double c) {

    if(a==0 && b==0)
        throw invalid_argument("parametry A i B prostej nie moga byc jednoczesnie rowne zero");

    this->a = a;
    this->b = b;
    this->c = c;

    double m=mi(this->a,this->b,this->c);
    this->a = m*this->a;
    this->b = m*this->b;
    this->c = m*this->c;
}

double prosta::get_a() const { return a; }
double prosta::get_b() const { return b; }
double prosta::get_c() const { return c; }

bool prosta::czy_wektor_prostopadly(wektor v) {
    if(v.dx == 0 && v.dy == 0)
        throw invalid_argument("nie da sie okreslic prostopadlosci wektora zerowego");

    // jesli stosunki sa rowne, to tak.
    // (wspolczynniki prostej wyznaczaja
    // rownolegly wektor [a,b] )
    if((v.dx / a) == (v.dy / b)) return true;
    return false;
}

bool prosta::czy_wektor_rownolegly(wektor v) {
    if(v.dx == 0 && v.dy == 0)
        throw invalid_argument("nie da sie okreslic rownoleglosci wektora zerowego");

    // jesli stosunki sa rowne, to tak.
    // (wspolczynniki prostej wyznaczaja
    // rownolegly wektor [-b,a] )
    if((v.dx / -b) == (v.dy / a)) return true;
    return false;
}

bool prosta::czy_punkt_nalezy(punkt p) {
    if (this->a * p.x + this->b * p.y + this->c == 0) return true;
    return false;
}

double prosta::odl_pkt_od_prostej(punkt p)
{
    double x = p.x;
    double y = p.y;

    return ( abs(a*x + b*y + c) / sqrt( a*a + b*b ) );
}

// ----- ----- ----- -----
// Funkcje globalne
// ----- ----- ----- -----

bool czy_proste_prostopadle(prosta &l, prosta &m) {
    //A1*A2 + B1*B2 = 0
    if(l.get_a() * m.get_a() + l.get_b() * m.get_b() == 0) return true;
    return false;
}

bool czy_proste_rownolegle(prosta &l, prosta &m) {
    //A1*B2 - A2*B1 = 0
    if(l.get_a() * m.get_b() - m.get_a() * l.get_b() == 0) return true;
    return false;
}

punkt przeciecie_sie_prostych(prosta &l, prosta &m) {
    double d = l.get_a() * m.get_b() - m.get_a() * l.get_b();

    if(d==0) throw invalid_argument("probujesz znalezc punkt przeciecia rownoleglych prostych...");

    double x = ( l.get_b() * m.get_c() - m.get_b() * l.get_c() ) / d;
    double y = (-1.0) * ( l.get_a() * m.get_c() - m.get_a() * l.get_c() ) / d;

    punkt p(x,y);

    return p;
}

double mi(double a, double b, double c)
{
    double mianownik = sqrt( a*a + b*b );

    if(c<0) return (1.0 / mianownik);
    else return (-1.0 / mianownik);
}

void wypisz_dane_prostej(prosta &p)
{
    //double alpha = acos(p.get_a()) * 180.0/PI;
    cout<<"A: "<<p.get_a()<<" , B: "<<p.get_b()<<" , C: "<<p.get_c()<</*" , alpha: "<<alpha<<*/endl;
}

wektor skladaj(wektor a, wektor b)
{
    wektor c(a.dx+b.dx,a.dy+b.dy);
    return c;
}
