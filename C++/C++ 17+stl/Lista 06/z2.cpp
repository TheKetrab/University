// cpp stl Bartlomiej Grochowski 300951
// zadanie 2 - algorytmy stl

#include <iostream>
#include <list>
#include <algorithm>
#include <sstream>

using namespace std;

class point {

public:
    float x, y;
    struct RGB { int r, g, b; } rgb;
    string name;

public:
    point() = default;
    point(float x, float y, int r, int g, int b, string name)
        : x(x), y(y), rgb({r,g,b}), name(name) {}

    void print() const {
        const int margin = 20;
        cout << name << string(margin - name.length(), ' ');

        stringstream ss; ss.precision(2);
        ss << "{ " << x << " , " << y << " }";
        string xy = ss.str();
        
        cout << xy << string(margin - xy.length(), ' ');
        cout << "R:" << rgb.r << " G:"<< rgb.g << " B:"<<rgb.b<<endl;
    }

    float luminance() const {
        return 0.3 * rgb.r + 0.59 * rgb.g + 0.11 * rgb.b;
    }

};

// dane
list<point> points = {
    point(1,2,      100,235,0  ,    "poczatek"),            // 1
    point(3,12,     75 ,10 ,15 ,    "przyklad"),            // 2
    point(8,-21.3,  200,255,10 ,    "kolejny punkt"),       // 3
    point(6,10,     65 ,87 ,66 ,    "punkt A"),             // 4
    point(1.8,2.5,  86 ,60 ,128,    "bardzo dluga nazwa"),  // 5
    point(0.9,12.5, 13 ,166,99 ,    "X"),                   // 6
    point(7,13,     12 ,240,123,    "RGB"),                 // 7
    point(5,24,     175,250,250,    "Y"),                   // 8
    point(5,6,      58 ,125,255,    "konkretny punkt"),     // 9
    point(8.,11,    230,96 ,68 ,    "szkodliwy punkt"),     // 10
    point(9.5,12.2, 125,157,0  ,    "jasny punkt"),         // 11
    point(0,0,      211,122,78 ,    "srodek"),              // 12
    point(6.5,1,    255,255,255,    "bialy"),               // 13
    point(2,-5,     0  ,0  ,0  ,    "czarny"),              // 14
    point(8,11,     54 ,69 ,60 ,    "pietnasty"),           // 15
    point(77,4,     90 ,55 ,62 ,    "szesnasty"),           // 16
    point(33,0.5,   123,248,155,    "ostatni")              // 17
};

// usun dluzsze niz 5 znakow
void A() {

    list<point> la(points);

    // remove_if przenosi to, co nie usunie na początek i zwraca
    // ostatni element 'p' tego, co powinno pozostać - dlatego trzeba
    // jeszcze usunąć pozostałe śmieci z przedziału [p , end) (erase)
    la.erase(remove_if(la.begin(),la.end(),
        [](const point &p) { return p.name.length() > 5; }),
        la.end());

    for (auto x : la)
        x.print();
}

// zlicza ile punktów jest w I, II, III, IV ćw u. wsp.
void B() {

    int x1 = count_if(points.begin(),points.end(),
        [](const point &p) { return p.x >= 0 && p.y >= 0; });

    int x2 = count_if(points.begin(),points.end(),
        [](const point &p) { return p.x <= 0 && p.y >= 0; });

    int x3 = count_if(points.begin(),points.end(),
        [](const point &p) { return p.x <= 0 && p.y <= 0; });

    int x4 = count_if(points.begin(),points.end(),
        [](const point &p) { return p.x >= 0 && p.y <= 0; });

    cout << "I:   " << x1 << endl;
    cout << "II:  " << x2 << endl;
    cout << "III: " << x3 << endl;
    cout << "IV:  " << x4 << endl;
}

// posortuj po jasności
void C() {

    list<point> lc(points);
    lc.sort([](const point &p1, const point &p2) {
        return p1.luminance() > p2.luminance(); });

    for (auto x : lc)
        x.print();
}

// kopiuj jeśli
void D() {

    list<point> ld(points);
    list<point> ld_dark(ld.size());

    auto it = copy_if(ld.begin(),ld.end(),ld_dark.begin(),
        [](const point &p) { return p.luminance() < 64; });

    // ld_dark byl wczesniej wiekszy - trzeba odsmiecic
    ld_dark.resize(distance(ld_dark.begin(),it));

    for (auto x : ld_dark)
        x.print();
}

#define TEST(f,msg) \
    cout << "----- ----- ----- ----- -----" << endl; \
    cout << "  " << msg << endl; \
    cout << "----- ----- ----- ----- -----" << endl; \
    f(); cout << endl;

int main() {

    TEST(A,"Nazwy krótsze niż 5 znaków");
    TEST(B,"Ile punktów w ćwiartkach uk. wsp.");
    TEST(C,"Posortowane po jasności");
    TEST(D,"Ciemne punkty");

    return 0;
}