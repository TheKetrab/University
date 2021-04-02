#include "iostream"
#include <exception>
#include <vector>
#ifndef DEF_MACIERZ
#define DEF_MACIERZ

namespace obliczenia {
// ---



class BladMacierz : public std::exception {

    std::string info;

public:
    BladMacierz();
    BladMacierz(const BladMacierz &b);
    BladMacierz& operator= (const BladMacierz &b);
    virtual ~BladMacierz();
    const char* what() const throw() override;
};

class RozmiaryNiePasuja : public BladMacierz {

    int r1n, r1m;
    int r2n, r2m;
    int rodzaj;

public:
    RozmiaryNiePasuja(int r1n, int r1m, int r2n, int r2m, int rodzaj);
    const char* what() const throw() override;
};

class ZleWejscieMacierzy : public BladMacierz {

    std::string wejscie;
    int powod;

public:
    ZleWejscieMacierzy(std::string w, int p);
    const char* what() const throw() override;

};

class DetZero : public BladMacierz {

public:
    DetZero() = default;
    const char* what() const throw() override;

};


/// indeksy w macierzy
///       --> n(j)
///  |   [ . . . ]
///  V   [ . . . ]
/// m(i) [ . . . ]
///
/// czyli macierz trzymamy jako
/// m wektorow n-dlugosci

class Macierz {

private:

    int n; // dlugosc wektora
    int m; // ilosc wektorow
    double **t; // [m][n] - wiersz, kolumna

    static double *wektor_jedno_jedynkowy(int n, int poz_1);
    static double *wektor_zerowy(int n);
    static double* wektor_bez_kolumny(const Macierz &m, int w_abstrakt, int k);
    static double* wektor_kolumna_do_wiersza(const Macierz &m, int k);

    // a[i][j] -> x=i, y=j
    static double iloczyn_pola(const Macierz &a, const Macierz &b, int x, int y);
    static std::vector<std::string> podziel_tekst(std::string s);


/// konstruktory

public:
    Macierz(int n);
    Macierz(int n, int m);
    Macierz(std::string s) throw (ZleWejscieMacierzy);
    Macierz(const Macierz &m, int w, int k); // ktory wiersz i ktora kolumne skreslic
    Macierz(const Macierz &m);
    Macierz(Macierz &&m);
    Macierz& operator= (const Macierz &m);
    Macierz& operator= (Macierz &&m);
    ~Macierz();

/// operacje na wierszach/kolumnach

public:
    void przestaw_wiersze(int w1, int w2);
    void przestaw_kolumny(int k1, int k2);
    void skaluj_wiersz(int w, double x);
    void skaluj_kolumne(int k, double x);
    void dodaj_do_wiersza_przeskalowany(int w1, int w2, double x);
    void dodaj_do_kolumny_przeskalowana(int k1, int k2, double x);
    void szukaj_wiersza_do_zmiany(int w1, int &w2, int k, bool &success) const;

    double det() const;
    Macierz odwrotna() const throw (DetZero);

    /// mnozenie przez skalar
    friend Macierz operator* (const Macierz &x, double y);
    Macierz& operator*= (double y);

    /// transpozycja
    friend Macierz operator! (const Macierz &x);

    /// dodawanie
    friend Macierz operator+ (const Macierz &x, const Macierz &y)
        throw (RozmiaryNiePasuja);
    Macierz& operator+= (const Macierz &y)
        throw (RozmiaryNiePasuja);

    /// odejmowanie
    friend Macierz operator- (const Macierz &x, const Macierz &y)
        throw (RozmiaryNiePasuja);
    Macierz& operator-= (const Macierz &y)
        throw (RozmiaryNiePasuja);

    /// mnozenie
    friend Macierz operator* (const Macierz &x, const Macierz &y)
        throw (RozmiaryNiePasuja);
    Macierz& operator*= (const Macierz &y)
        throw (RozmiaryNiePasuja);


/// wypisywanie

    void wypisz() const;
    friend std::ostream & operator<<(std::ostream &wy, const Macierz &x);
    friend std::istream & operator>>(std::istream &we, Macierz &x)
        throw (ZleWejscieMacierzy);

};




// ---
}

#endif
