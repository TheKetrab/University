#include <iostream>
#include "datagodz.hpp"

#ifndef DEF_WYD
#define DEF_WYD

class Wydarzenie {

public:
    DataGodz dg;
    std::string nazwa;

    Wydarzenie(DataGodz dg, std::string nazwa);
    Wydarzenie(const Wydarzenie &w);
    Wydarzenie(Wydarzenie &w);
    Wydarzenie& operator= (const Wydarzenie &w);

    friend bool operator<(const Wydarzenie& w1, const Wydarzenie& w2);
    friend std::ostream & operator << (std::ostream &wy, const Wydarzenie &w);
};


#endif
