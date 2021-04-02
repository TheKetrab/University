#include <iostream>
#include "data.hpp"


#ifndef DEF_DATAGODZ
#define DEF_DATAGODZ

class DataGodz final : public Data {

protected:
    int godzina;
    int minuta;
    int sekunda;

public:
    DataGodz(int d, int m, int r, int go, int mi, int se);
    DataGodz(int go, int mi, int se); // parametryzowany?
    DataGodz();
    ~DataGodz();

    DataGodz(const DataGodz &dg);
    DataGodz(DataGodz &dg);
    DataGodz& operator= (const DataGodz &dg);

protected:
    uint64_t ilesekunduplynelo() const; // od daty 01.01.0000r

public:
    int get_godzina() const;
    int get_minuta() const;
    int get_sekunda() const;

    // -> dospecyfikowalem sobie:
    // operacje nie moga zmieniac dnia!
    DataGodz & operator++ ();
    DataGodz operator++ (int);
    DataGodz & operator-- ();
    DataGodz operator-- (int);
    DataGodz & operator+= (int s);
    DataGodz & operator-= (int s);
    // ----- ----- ----- ----- -----


protected:
    uint64_t ilesekund();

public:
    friend std::ostream & operator << (std::ostream &wy, const DataGodz &dg);
    friend bool operator<(const DataGodz& dg1, const DataGodz& dg2);
    friend bool operator==(const DataGodz& dg1, const DataGodz& dg2);
    friend int64_t operator- (const DataGodz &dg1, const DataGodz &dg2);

};



#endif
