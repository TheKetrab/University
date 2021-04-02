#ifndef STOS_DEF
#define STOS_DEF

using namespace std;

class stos {

    int pojemnosc;
    int ile=0; // rozmiar zajmowanego
    string *elementy;

    // -----

    bool pelny();
    bool pusty();


    public:

        stos(int pojemnosc);
        stos();
        stos(initializer_list<string> args);
        stos(stos &s);
        stos(const stos &);
        stos(stos &&s);
        stos& operator= (const stos &s);
        stos& operator= (stos &&s);

        ~stos();


        void wloz(string x);
        string sciagnij();
        string sprawdz();
        int rozmiar();
        void wypisz_stos();


};


#endif
