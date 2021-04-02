#ifndef DEF_WYM
#define DEF_WYM

namespace obliczenia {



class WyjatekWymierny : public std::exception {

protected:
    std::string info;

public:
    WyjatekWymierny();
    WyjatekWymierny(std::string tresc);
    virtual ~WyjatekWymierny();
    const char* what() const throw() override;
};

class MianownikZero : public WyjatekWymierny {

public:
    MianownikZero();
    virtual ~MianownikZero();
    const char* what() const throw() override;

};

class PrzekroczenieZakresu : public WyjatekWymierny {

public:
    PrzekroczenieZakresu(int a, int b, char op);
    virtual ~PrzekroczenieZakresu();
    const char* what() const throw() override;

};

class MianownikUjemny : public WyjatekWymierny {

public:
    MianownikUjemny(int l, int m);
    virtual ~MianownikUjemny();
    const char* what() const throw() override;

};










class Wymierna {

    int licz, mian;

public:
    Wymierna(int l, int m) throw (MianownikZero, MianownikUjemny);
    Wymierna(int l);
    Wymierna(const Wymierna &w);
    Wymierna& operator= (const Wymierna &w);
    operator double ();
    explicit operator int ();


public:
    static int nww(int x, int y) throw (PrzekroczenieZakresu);
    static int nwd(int x, int y);
    static bool przekroczy_zakres(const int a, const int b, char op);

public:
    int get_licz();
    int get_mian();

public:
    Wymierna& operator += (const Wymierna &w) throw (PrzekroczenieZakresu);
    Wymierna& operator -= (const Wymierna &w) throw (PrzekroczenieZakresu);
    Wymierna& operator *= (const Wymierna &w) throw (PrzekroczenieZakresu);
    Wymierna& operator /= (const Wymierna &w) throw (MianownikZero);
    friend Wymierna operator + (const Wymierna &x, const Wymierna &y);
    friend Wymierna operator - (const Wymierna &x, const Wymierna &y);
    friend Wymierna operator * (const Wymierna &x, const Wymierna &y);
    friend Wymierna operator / (const Wymierna &x, const Wymierna &y);

    Wymierna& operator - ();
    Wymierna& operator ! () throw (MianownikZero);

    std::string ulamek_okresowy() const;


public:
    void wypisz();
    friend void skroc(Wymierna &w);
    friend void normalizuj(Wymierna &w1, Wymierna &w2);
    friend std::ostream& operator<< (std::ostream &wyj, const Wymierna &w);

};




}

#endif
