#ifndef STRUMIENIE
#define STRUMIENIE

using namespace std;


namespace strumienie {

/// funkcje testowe
void func_zad_1();
void func_zad_2();
void func_zad_3(string loc);
void func_zad_5(string input,string output);


/// manipulatory
istream& clearline(istream &in);
inline ostream& comma (ostream &os);
inline ostream& colon (ostream &os);

class ignore {
    int ile;
public:
    ignore (int ile);
    friend istream& operator >> (istream &is, const ignore &nap);
};

class inde {
    int x;
    int w;
public:
    inde (int x, int w) throw (invalid_argument);
    friend ostream& operator<< (ostream &os, const inde &ind);
};


/// do sortowania par
bool porownanie (const pair<int,string> &p1, const pair<int,string> &p2);


/// wrappery
class wejscie {
public:
    ifstream p;
    wejscie(string loc) throw (ios_base::failure);
    ~wejscie();
    int czytaj_bajt() throw (ios_base::failure);
    friend wejscie& operator>> (wejscie& we, int &c);

};

class wyjscie {
public:
    ofstream p;
    wyjscie(string loc) throw (ios_base::failure);
    ~wyjscie();
    void write(int c);
    friend wyjscie& operator<< (wyjscie& wy, int &c);

};

}

#endif
