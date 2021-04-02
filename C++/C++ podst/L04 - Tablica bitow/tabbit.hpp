#include <iostream>
#include <bitset>

#ifndef DEF_TABBIT
#define DEF_TABBIT


using namespace std;

class tab_bit
{
    // ----- ----- -----
    typedef uint64_t slowo;             // komorka w tablicy
    static const int rozmiarSlowa=64;   // rozmiar slowa w bitach

    // ----- ----- -----
    class ref
    {
        int i;
        tab_bit& tb;

    public:
        ref(int i, tab_bit& tb) : i(i),tb(tb) {};
        operator bool() const;
        ref operator= (bool b)
        {
            tb.pisz(i,b);
        }

        ~ref();

    };

    // ----- ----- -----
protected:
    int dl;         // liczba bitów
    slowo *tab;     // tablica bitów

public:
    explicit tab_bit (int rozm);        // wyzerowana tablica bitow [0...rozm]
    explicit tab_bit (slowo s);         // tablica bitów [0...rozmiarSlowa]

    // zainicjalizowana wzorcem
    tab_bit (const tab_bit &tb);                // konstruktor kopiujący
    tab_bit (tab_bit &&tb);                     // konstruktor przenoszący
    tab_bit(initializer_list<bool> args);
    tab_bit & operator = (const tab_bit &tb);   // przypisanie kopiujące
    tab_bit & operator = (tab_bit &&tb);        // przypisanie przenoszące
    ~tab_bit ();                                // destruktor

private:
    bool czytaj (int i) const;          // metoda pomocnicza do odczytu bitu
    bool pisz (int i, bool b);          // metoda pomocnicza do zapisu bitu

public:
    bool operator[] (int i) const;      // indeksowanie dla stałych tablic bitowych
    ref operator[] (int i);             // indeksowanie dla zwykłych tablic bitowych
    inline int rozmiar () const;        // rozmiar tablicy w bitach


// operatory bitowe: | i |=, & i &=, ^ i ^= oraz !
public:
    friend tab_bit operator | (const tab_bit &x, const tab_bit &y);
    friend tab_bit operator & (const tab_bit &x, const tab_bit &y);
    friend tab_bit operator ^ (const tab_bit &x, const tab_bit &y);

    tab_bit & operator |= (const tab_bit &x);
    tab_bit & operator &= (const tab_bit &x);
    tab_bit & operator ^= (const tab_bit &x);

    friend tab_bit operator ! (const tab_bit &x);


// zaprzyjaźnione operatory strumieniowe: << i >>
public:
    friend istream & operator >> (istream &we, tab_bit &tb);
    friend ostream & operator << (ostream &wy, const tab_bit &tb);


// inne metody
private:
    int f_ktore_slowo(int i) const;
    int f_ktory_bit(int i) const;
    bool wydobadz_bit(slowo s, int ktory) const;
    int ilosc_slow() const;
};


#endif
