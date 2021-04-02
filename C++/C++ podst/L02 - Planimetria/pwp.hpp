#ifndef PWP_INCLUDED
#define PWP_INCLUDED

class punkt;
class wektor;
class prosta;

/* --- PUNKT, WEKTOR, PROSTA --- */

class punkt {
    public:
        const double x, y;

        // ---
        punkt()=default;
        punkt(double a, double b);
        punkt(punkt a, wektor v);
        punkt(punkt &p);

        //brak przypisania kopiujacego
        punkt& operator=(const punkt&) = delete;
        punkt& operator=(punkt &&a) = delete;
};

class wektor {
    public:
        const double dx, dy;

        // ---
        wektor()=default;
        wektor(double a, double b);
        wektor(wektor &w);

        //brak przypisania kopiujacego
        wektor& operator=(const wektor&) = delete;
        wektor& operator=(wektor &&v) = delete;
};

class prosta {
    private:
        double a=0, b=1, c=0;

        //klasa niekopiowalna
        prosta& operator=(prosta &);
        prosta& operator=(const prosta&);
        prosta(const prosta&);
    public:

        // ---
        prosta(punkt a, punkt b);
        prosta(wektor v);
        prosta(double a, double b, double c);
        prosta(prosta &l, wektor v);
        prosta()=default;

        // ---
        double get_a() const;
        double get_b() const;
        double get_c() const;

        bool czy_wektor_prostopadly(wektor v);
        bool czy_wektor_rownolegly(wektor v);
        bool czy_punkt_nalezy(punkt p);
        double odl_pkt_od_prostej(punkt p);
};

wektor skladaj(wektor a, wektor b);
bool czy_proste_rownolegle(prosta &l, prosta &m);
bool czy_proste_prostopadle(prosta &l, prosta &m);
punkt przeciecie_sie_prostych(prosta &l, prosta &m);
void wypisz_dane_prostej(prosta &p);
double mi(double a, double b, double c);

#endif
