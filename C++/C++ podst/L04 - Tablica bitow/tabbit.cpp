#include <iostream>
#include <cmath>
#include "tabbit.hpp"



/* --- ref --- */

/*tab_bit::ref::ref(bool b, int ks, int kb)
{
    bit = b;
    ktore_slowo = ks;
    ktory_bit = kb;
}



*/


tab_bit::ref::operator bool() const
{
    bool b = tb.czytaj(i);
    return b;
}

tab_bit::ref::~ref()
{
    //delete tb;
    i = 0;
}



/* --- tab_bit --- */

// KONSTRUKTORY

tab_bit::tab_bit (int rozm)
{
    if(rozm<0) throw invalid_argument("Rozmiar tab_bit musi byc nieujemny!");

    dl = rozm;

    int is = ilosc_slow();

    tab = new slowo[is];

    // inicjowanie zerami
    for(int i=0; i<is; i++)
        tab[i] = 0;

}

tab_bit::tab_bit (slowo s)
{
    if(s<0) throw invalid_argument("s musi byc nieujemny!");

    dl = rozmiarSlowa;
    tab = new slowo[1];
    tab[0] = s;
}

tab_bit::~tab_bit()
{
    dl = 0;
    delete[] tab;
}

tab_bit::tab_bit(const tab_bit &tb)
{
    dl = tb.dl;

    int is = ilosc_slow();
    tab = new slowo[is];
    for(int i=0; i<is; i++)
        tab[i] = tb.tab[i];

}

tab_bit::tab_bit(tab_bit &&tb)
{
    dl = tb.dl;

    int is = ilosc_slow();
    tab = new slowo[is];
    swap(tab, tb.tab);

}

tab_bit::tab_bit(initializer_list<bool> args)
{
    dl = args.size();

    int is = ilosc_slow();
    tab = new slowo[is];


    for(int i=0; i!=(int)args.size(); ++i) {
        if(args.begin()[i]==0 || args.begin()[i]==1) pisz(i,args.begin()[i]);
        else {
            delete[] tab;
            throw invalid_argument("initializater_list zawiera argumenty inne niz 0 i 1 !");
        }
    }

}

// OPERATORY PRZYPISANIA

tab_bit& tab_bit:: operator = (const tab_bit &tb)
{
    dl = tb.dl;
    delete[] tab;

    int is = ilosc_slow();
    tab = new slowo[is];
    for(int i=0; i<is; i++)
        tab[i] = tb.tab[i];

    return *this;
}

tab_bit& tab_bit:: operator = (tab_bit &&tb)
{
    dl = tb.dl;
    delete[] tab;

    tab = tb.tab;

    return *this;
}


// METODY

//zwraca numer slowa, w ktorym znajduje sie ity bit
int tab_bit::f_ktore_slowo(int i) const
{
    return (int)(i/rozmiarSlowa);
}

//zwraca numer bitu w slowie
int tab_bit::f_ktory_bit(int i) const
{
    int ks = f_ktore_slowo(i);
    int is = ilosc_slow();

    //jesli to ostatnie slowo
    if(ks == is-1)
        return (dl/is - (i+1));

    return i%rozmiarSlowa;
}

bool tab_bit::wydobadz_bit(slowo s, int ktory) const
{
    return (s>>ktory)&1;
}

inline int tab_bit::rozmiar () const
{
    return dl;
}

int tab_bit::ilosc_slow() const
{
    int wynik = ceil( (double)dl / (double)rozmiarSlowa );
    return wynik;
}


bool tab_bit::czytaj(int i) const
{
    if(i>dl) throw invalid_argument("indeks nie istnieje w tablicy bitow.");
    if(i<0) throw invalid_argument("indeks nie moze byc ujemny!");

    int ks = f_ktore_slowo(i);
    int kb = f_ktory_bit(i);

    bool bit = wydobadz_bit(tab[ks],kb);
    return bit;

}

bool tab_bit::pisz(int i, bool b)
{
    if(i>dl) throw invalid_argument("indeks nie istnieje w tablicy bitow.");
    if(i<0) throw invalid_argument("indeks nie moze byc ujemny!");

    int ks = f_ktore_slowo(i);
    int kb = f_ktory_bit(i);

    bool bit_ktory_podmieniamy = wydobadz_bit(tab[ks],kb);

    if(b==1)
        tab[ks] = (1<<kb) | tab[ks];
    else
        tab[ks] = ~(1<<kb) & tab[ks];

    return bit_ktory_podmieniamy;

}


// jak wywolac ten pierwszy operator?
bool tab_bit::operator[] (int i) const
{
    return czytaj(i);
}

tab_bit::ref tab_bit::operator[] (int i)
{
    int ks = f_ktore_slowo(i);
    int kb = f_ktory_bit(i);
    bool b = wydobadz_bit(tab[ks],kb);

//    return ref(b,ks,kb);

}



// OPERATORY

tab_bit operator | (const tab_bit &x, const tab_bit &y) {

    if(x.dl!=y.dl) throw invalid_argument("Operator | zdefiniowany tylko dla tablic tej samej dlugosci");

    int is = x.ilosc_slow();
    tab_bit zwracana(x.dl);

    for(int i=0; i<is; i++)
        zwracana.tab[i] = x.tab[i] | y.tab[i];

    return zwracana;
}

tab_bit operator & (const tab_bit &x, const tab_bit &y) {

    if(x.dl!=y.dl) throw invalid_argument("Operator & zdefiniowany tylko dla tablic tej samej dlugosci");

    int is = x.ilosc_slow();
    tab_bit zwracana(x.dl);

    for(int i=0; i<is; i++)
        zwracana.tab[i] = x.tab[i]&y.tab[i];

    return zwracana;
}

tab_bit operator ^ (const tab_bit &x, const tab_bit &y) {

    if(x.dl!=y.dl) throw invalid_argument("Operator ^ zdefiniowany tylko dla tablic tej samej dlugosci");

    int is = x.ilosc_slow();
    tab_bit zwracana(x.dl);

    for(int i=0; i<is; i++)
        zwracana.tab[i] = x.tab[i] ^ y.tab[i];

    return zwracana;
}

tab_bit & tab_bit::operator |= (const tab_bit &x) {

    if(this->dl!=x.dl) throw invalid_argument("Operator |= zdefiniowany tylko dla tablic tej samej dlugosci");
    int is = x.ilosc_slow();

    for(int i=0; i<is; i++)
        this->tab[i] = x.tab[i] | this->tab[i];

    return *this;
}

tab_bit & tab_bit::operator &= (const tab_bit &x) {

    if(this->dl!=x.dl) throw invalid_argument("Operator &= zdefiniowany tylko dla tablic tej samej dlugosci");
    int is = x.ilosc_slow();

    for(int i=0; i<is; i++)
        this->tab[i] = x.tab[i] & this->tab[i];

    return *this;
}

tab_bit & tab_bit::operator ^= (const tab_bit &x) {

    if(this->dl!=x.dl) throw invalid_argument("Operator ^= zdefiniowany tylko dla tablic tej samej dlugosci");
    int is = x.ilosc_slow();

    for(int i=0; i<is; i++)
        this->tab[i] = x.tab[i] ^ this->tab[i];

    return *this;
}

tab_bit operator ! (const tab_bit &x) {

    int is = x.ilosc_slow();
    tab_bit zwracana(x.dl);

    for(int i=0; i<is; i++)
        zwracana.tab[i] = ~x.tab[i];

    return zwracana;
}


// funkcja zaprzyjazniona do wypisania tab_bit do strumienia
ostream & operator << (ostream &wy, const tab_bit &tb)
{

    tab_bit kopia(tb);

    int ile_tablic = ceil( (double)kopia.dl / (double)kopia.rozmiarSlowa );

    for(int i=0; i<ile_tablic-1; i++) {
        for(int j=kopia.rozmiarSlowa-1; j>=0; j--) {
            wy<<kopia.wydobadz_bit(kopia.tab[i], j);
        }
    }

    int ile_zostalo = kopia.dl % (kopia.rozmiarSlowa + 1);
    //cout<<"ILE ZOSTALO"<<ile_zostalo<<endl;
    for(int j=ile_zostalo-1; j>=0; j--)
        wy<<kopia.wydobadz_bit(kopia.tab[ile_tablic-1], j);


    return wy;


}

// funkcja zaprzyjazniona do odczytania tab_bit ze strumienia
istream & operator >> (istream &we, tab_bit &tb)
{
    string tablica;
    we>>tablica;

//    cout<<"STRING:"<<tablica<<endl;

    int dlugosc = tablica.length();
    tb.dl = dlugosc;
    int ile_tablic = ceil( (double)dlugosc / (double)tb.rozmiarSlowa );

//    cout<<"DL,IT:"<<dlugosc<<" "<<ile_tablic<<endl;

    delete[] tb.tab;

    tb.tab = new uint64_t[ile_tablic];

    //ciecie stringa na tablice
    int iw = 0; //indeks wejscia
    for(int i=0; i<ile_tablic-1; i++) {
        tb.tab[i]=0;
        for(int j=0; j<tb.rozmiarSlowa; j++) {
            tb.tab[i]=tb.tab[i]*2;
            tb.tab[i]=tb.tab[i]+(tablica[iw]-'0');
            iw++;
        }
    }

    int ile_zostalo = tb.dl % tb.rozmiarSlowa;

    tb.tab[ile_tablic-1]=0;
    for(int i=0; i<ile_zostalo; i++) {
        tb.tab[ile_tablic-1]=tb.tab[ile_tablic-1]*2;
        tb.tab[ile_tablic-1]=tb.tab[ile_tablic-1]+(tablica[iw]-'0');
        iw++;
    }
//    cout<<"ULL: "<<tb.tab[0]<<endl;

    return we;

}
