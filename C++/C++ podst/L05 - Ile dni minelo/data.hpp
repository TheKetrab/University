#include <iostream>

#ifndef DEF_DATA
#define DEF_DATA

class Data {

protected:
    int dzien;
    int miesiac;
    int rok;

public:
    Data(); // aktualny czas
    Data(int d, int m, int r);
    ~Data();
    Data(const Data &d) = default;
    Data& operator= (Data &d) = default;

private:
    static bool poprawna(int d, int m, int r);

protected:
    bool ostatnidzienroku();
    bool ostatnidzienmiesiaca();
    bool pierwszydzienroku();
    bool pierwszydzienmiesiaca();

protected:
    int iledniuplynelo() const; // od daty 01.01.0000r
    int iledniuplynelo2() const;
    Data & jakadata(int d); // operacja odwrotna
    //wydobywa miesiac i dzien na podstawie pozostalych dni
    void md (int &m, int &d, int dni, bool przest);

public:
    int get_dzien();
    int get_miesiac();
    int get_rok();

    static bool przestepny(int rok);
    static int dniwmiesiacach[2][13];
    static int dniodpoczroku[2][13];

    // operatory
    int operator- (const Data &d);
    Data & operator++ ();
    Data operator++ (int);
    Data & operator-- ();
    Data operator-- (int);
    Data & operator+= (int d);
    Data & operator-= (int d);

    void wypisz_date();
};



#endif
