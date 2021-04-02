#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstring>
#include <streambuf>
#include <cassert>
#include "strumienie.hpp"
using namespace std;


namespace strumienie {

void func_zad_1() {

    cout<<"FUNC ZAD 1"<<endl;
    cout<<"Podaj x, zostanie zignorowane 5 znakow. ";
    string x;
    cin>>ignore(5)>>x;
    cout<<"x: ;"<<x<<";"<<endl;

}

void func_zad_2() {

    /// TEST
    cout<<"FUNC ZAD 2"<<endl;
    cout<<inde(5,5)<<colon<<"aaa"<<comma<<endl;
    cout<<inde(0,5)<<colon<<"bbb"<<comma<<endl;
    cout<<inde(114,5)<<colon<<"ccc"<<comma<<endl;
    cout<<inde(1115432343,5)<<colon<<"ddd"<<comma<<endl;

}

void func_zad_3(string loc) {

    cout<<"FUNC ZAD 3"<<endl;

    vector<pair<int,string>> vec;

    fstream p;
    p.open( loc, std::ios::in | std::ios::out );

    if( p.good() == true )
    {
        int i=0;
        while(p.eof()==false) {
            string temp = "";
            getline( p, temp );
            vec.push_back({i+1,temp});
            i++;
        }

        vec.pop_back(); // bo getline pobiera pustego stringa na koniec

        p.close();
    }

    std::sort(vec.begin(), vec.end(), porownanie);

    for(int i=0; i<(int)vec.size(); i++) {
        cout<<inde(vec[i].first,3)<<colon<<vec[i].second<<endl;
    }


}

void func_zad_5(string input,string output) {

    cout<<"FUNC ZAD 5"<<endl;

    wejscie i(input);
    wyjscie o(output);

    while(i.p.peek()!=EOF) {
        int bajt1;
        int bajt2;

        //bajt1 = i.czytaj_bajt();
        i>>bajt1;

        if(i.p.good() && i.p.peek()!=EOF) {
            //bajt2 = i.czytaj_bajt();
            //o.write(bajt2);
            //o.write(bajt1);
            i>>bajt2;
            o<<bajt2;
            o<<bajt1;
        }

        else {
            //o.write(bajt1);
            o<<bajt1;
            break;
        }

    }


}

/// ZADANIE 1
istream& clearline(istream &in) {

    char c;
    while(true) {
        c = in.get();
        if(c==EOF || c=='\n' || c=='\0') break;
    }
    return in;
}

/// ignore
istream& operator >> (istream &is, const ignore &nap) {

    char c;
    for(int i=0; i<nap.ile; i++) {
        c = is.get();
        if(c==EOF || c=='\n' || c=='\0')
            break;
    }

    return is;
}

ignore::ignore (int ile) : ile(ile) {}




/// ZADANIE 2
inline ostream& comma (ostream &os) {
    return os << ", ";
}
inline ostream& colon (ostream &os) {
    return os << ": ";
}

ostream& operator<< (ostream &os, const inde &ind) {

    int ilosc_cyfr;
    if(ind.x == 0) ilosc_cyfr = 1;
    else ilosc_cyfr = floor( log10( ind.x ) ) + 1;


    int tab[ilosc_cyfr];
    int liczba = ind.x;


    for(int i=ilosc_cyfr-1; i>=0; i--) {
        tab[i] = liczba%10;
        liczba /= 10;
    }

    // dwie sytuacje
    if(ilosc_cyfr >= ind.w) {
        os << '[';
        for(int i=0; i<ilosc_cyfr; i++)
            os << tab[i];
        os << ']';
    }

    else {
        os << '[';
        for(int i=0; i<ind.w-ilosc_cyfr; i++)
            os<<0;
        for(int i=0; i<ilosc_cyfr; i++)
            os<<tab[i];
        os << ']';
    }

    return os;
}

inde::inde (int x, int w)
throw (invalid_argument) : x(x), w(w) {
    if(w<0) throw invalid_argument("W < 0 !!!");
}

bool porownanie (const pair<int,string> &p1, const pair<int,string> &p2) {
    return p1.second < p2.second;
}


/// ZADANIE 4

/// wejscie
wejscie::wejscie(string loc) throw (ios_base::failure) {

    p.open( loc , std::ios::in | ios::binary );
    p.exceptions(ios::failbit | ios::badbit);

    if(p.fail()) // czyli flaga failbit lub badbit
        throw ios_base::failure("Opening file failed");
}

wejscie::~wejscie() {
    p.close();
}

int wejscie::czytaj_bajt() throw (ios_base::failure) {
    int c;

    if(p.fail()) throw ios_base::failure("Bad/Fail bit");

    c =  p.get();
    return c;
}

wejscie& operator>> (wejscie& we, int &c) {

    c = we.czytaj_bajt();
    return we;
}

/// wyjscie
wyjscie::wyjscie(string loc) throw (ios_base::failure) {

    p.open( loc , std::ios::out | ios::binary );
    p.exceptions(ios::failbit | ios::badbit);

    if(p.fail()) // czyli flaga failbit lub badbit
        throw ios_base::failure("Opening file failed");
}

wyjscie::~wyjscie() {
    p.close();
}

void wyjscie::write(int c) {
    p.put(c);
}

wyjscie& operator<< (wyjscie& wy, int &c) {

    wy.p.put(c);
    return wy;
}

}
