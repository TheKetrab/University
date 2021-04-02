#include <iostream>
#include <vector>
#include <exception>
#include <string>
#include <cmath>
#include "macierz.hpp"

namespace obliczenia {
// ---




/// ===== MACIERZ =====

/// ----- ----- -----
/// konstruktory

Macierz::Macierz(const Macierz &m, int w, int k) {

    this->n=m.n-1;
    this->m=m.m-1;

    this->t = new double *[m.m-1];

    for(int i=0; i<w-1; i++)
        t[i] = wektor_bez_kolumny(m,i,k);

    for(int i=w-1; i<this->m; i++)
        t[i] = wektor_bez_kolumny(m,i+1,k);
}

Macierz::Macierz(int n) {

    this->n = n;
    this->m = n;
    t = new double *[n];
    for(int i=0; i<n; i++)
        t[i] = wektor_jedno_jedynkowy(n,i);
}

Macierz::Macierz(int m, int n) {

    this->n = n;
    this->m = m;
    t = new double *[m];
    for(int i=0; i<m; i++)
        t[i] = wektor_zerowy(n);
}

Macierz::Macierz(std::string s) throw (ZleWejscieMacierzy) {

    std::vector<std::string> vec = Macierz::podziel_tekst(s);

    if(vec[0]!="(" || vec[3]!=")(" || vec[(int)vec.size()-1]!=")")
        throw ZleWejscieMacierzy(s,0);


    n = stoi(vec[1]);
    m = stoi(vec[2]);

    if((int)vec.size() != (n*m + 5))
        throw ZleWejscieMacierzy(s,1);

    // zadeklarowanie tablicy
    t = new double *[m];
    for(int i=0; i<m; i++)
        t[i] = new double [n];

    // inicjacja tablicy
    int k=4;
    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++)
            t[i][j]=stod(vec[k++]);
}

Macierz::Macierz(const Macierz &m) {

    this->n = m.n;
    this->m = m.m;

    this->t = new double *[this->m];
    for(int i=0; i<this->m; i++)
        t[i] = new double [n];

    for(int i=0; i<this->m; i++)
        for(int j=0; j<this->n; j++)
            this->t[i][j]=m.t[i][j];
}

Macierz::Macierz(Macierz &&m) {

    n = m.n;
    m = m.m;
    t = m.t;
    //czyszczenie
    for(int i=0; i<m.m; i++)
         m.t[i] = nullptr;
    m.t = nullptr;


}

Macierz& Macierz::operator=(const Macierz &m) {

    // przypisz
    this->n=m.n;
    this->m=m.m;
    this->t=m.t;

    return *this;
}

Macierz& Macierz::operator=(Macierz &&m) {

    // wyczysc poprzednika
    for(int i=0; i<this->m; i++)
        delete[] t[i];
    delete[] t;
    n=0, m=0;

    // przypisz
    n=m.n;
    m=m.m;
    t=m.t;

    // reset
    m.n=0; m.m=0;
    m.t=nullptr;

    return *this;
}

Macierz::~Macierz() {

    for(int i=0; i<m; i++)
         delete[] t[i];

    delete[] t;
    n=0, m=0;

}


/// ----- ----- -----
/// metody statyczne

double* Macierz::wektor_kolumna_do_wiersza(const Macierz &m, int k) {

    double *tab = new double [m.m];
    for(int i=0; i<m.m; i++)
        tab[i] = m.t[i][k-1];

    return tab;

}

double* Macierz::wektor_jedno_jedynkowy(int n, int poz_1) {

    double *tab = new double [n];
    int i=0;
    for(;i<poz_1;i++) tab[i]=0;
    tab[i]=1; i++;
    for(;i<n;i++) tab[i]=0;

    return tab;
}

double* Macierz::wektor_zerowy(int n) {

    double *tab = new double [n];
    for(int i=0; i<n; i++)
        tab[i]=0;

    return tab;
}

double* Macierz::wektor_bez_kolumny(const Macierz &m, int w_abstrakt, int k) {

    double *tab = new double [m.n-1];

    // przepisz do k-tej kolumny
    for(int i=0; i<k-1; i++)
        tab[i]=m.t[w_abstrakt][i];

    // przepisz po k-tej kolumnie
    for(int i=k-1; i<m.n-1; i++)
        tab[i]=m.t[w_abstrakt][i+1];

    return tab;
}

double Macierz::iloczyn_pola(const Macierz &a, const Macierz &b, int x, int y) {

    double wynik=0.0;
    double iloczyn=0.0;

    for(int i=0; i<a.m; i++) {
        //std::cout<<"at[x][i], b.t[i][y] "<<a.t[x][i]<<" "<<b.t[i][y]<<std::endl;
        iloczyn = a.t[x][i] * b.t[i][y];
        //std::cout<<"ILOCZYN-----------------> "<<iloczyn<<std::endl;
        wynik = wynik + iloczyn;

        //std::cout<<"WYNIK: "<<wynik<<std::endl;
}

    //std::cout<<"WYNIK PO PETLI: "<<wynik<<std::endl;

    return wynik;
}

std::vector<std::string> obliczenia::Macierz::podziel_tekst(std::string s) {

    std::vector<std::string> wynik;

    /*
    // podzial stringa - korzystam z kodu ze strony:
    // https://stackoverflow.com/questions/14265581/ \
    // parse-split-a-string-in-c-using-string-delimiter-standard-c
    */

    std::string delimiter = "_";

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        wynik.push_back(token);
        s.erase(0, pos + delimiter.length());
    }

    wynik.push_back(s);

    return wynik;

}


/// ----- ----- -----
/// inne metody

void Macierz::szukaj_wiersza_do_zmiany(int w1, int &w2, int k, bool &success) const {
// w1 - od kad, w2 - zwroc, k - wg ktorej kolumny, sukces t/f?

    // szukamy tylko od w1 na dol, bo wyzsze sa okej
    for(int i=w1-1; i<m; i++)
        if(t[i][k-1]!=0) {
            w2=i+1;
            success=true;
            return;
        }

    success=false;
    return;

}

void Macierz::przestaw_wiersze(int w1, int w2) {
    std::swap(t[w1-1],t[w2-1]);
}

void Macierz::przestaw_kolumny(int k1, int k2) {

    for(int i=0; i<m; i++)
        std::swap(t[i][k1-1],t[i][k2-1]);
}

void Macierz::skaluj_wiersz(int w, double x) {

    if(x==0) throw std::invalid_argument("zle!");

    for(int i=0; i<n; i++)
        t[w-1][i]*=x;
}

void Macierz::skaluj_kolumne(int k, double x) {

    if(x==0) throw std::invalid_argument("zle!");

    for(int i=0; i<n; i++)
        t[i][k-1]*=x;
}

void Macierz::dodaj_do_wiersza_przeskalowany(int w1, int w2, double x) {

    for(int i=0; i<n; i++)
        t[w1-1][i] += x*t[w2-1][i];

}

void Macierz::dodaj_do_kolumny_przeskalowana(int k1, int k2, double x) {

    for(int i=0; i<n; i++)
        t[i][k1-1] += x*t[i][k2-1];

}

double Macierz::det() const {

    //LaPlas

    if(n==1 && m==1)
        return t[0][0];

    double wynik = 0;

    for(int i=0; i<n; i++)
        wynik+= pow(-1.0,1 + i+1) * t[0][i] * Macierz(*this,1,i+1).det();

    return wynik;

}

Macierz Macierz::odwrotna() const throw (DetZero) {

    // tworzy kopie
    Macierz m(this->n);
    for(int i=0; i<this->m; i++)
        for(int j=0; j<this->n; j++)
            m.t[i][j] = t[i][j];

    Macierz baza(m.n);

    double wsp;

    /* SPROWADZENIE DO JEDYNEK NA PRZEKATNEJ */

    for(int i=0; i<m.n; i++) {

        // jesli zero, to musimy znalezc wiersz do podmienienia
        if(m.t[i][i]==0) {

            int x=0;
            bool s=false;

            m.szukaj_wiersza_do_zmiany(i+1,x,i+1,s);

            if(s==false) throw DetZero();

            m.przestaw_wiersze(i+1,x);
            baza.przestaw_wiersze(i+1,x);

        }

        wsp = 1.0 / m.t[i][i]; // chcemy, zeby tutaj byla jedynka
        m.skaluj_wiersz(i+1,wsp);
        baza.skaluj_wiersz(i+1,wsp);

        for(int j=1+i; j<m.m; j++) {

            wsp = -1.0 * (m.t[j][i] / m.t[i][i]);
            m.dodaj_do_wiersza_przeskalowany(j+1,i+1,wsp);
            baza.dodaj_do_wiersza_przeskalowany(j+1,i+1,wsp);

        }

    }

    /* SPROWADZENIE DO MACIERZY IDENTYCZNOSCIOWEJ */

    for(int i=m.n-1; i>=0; i--) {

        for(int j=i-1; j>=0; j--) {

            wsp = -1.0 * (m.t[j][i] / m.t[i][i]);
            m.dodaj_do_wiersza_przeskalowany(j+1,i+1,wsp);
            baza.dodaj_do_wiersza_przeskalowany(j+1,i+1,wsp);

        }


    }


    return baza;
}


/// ----- ----- -----
/// operatory

Macierz& Macierz::operator*= (double y) {

    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++)
            t[i][j]*=y;

    return *this;
}


Macierz& Macierz::operator+= (const Macierz &y) throw(RozmiaryNiePasuja) {

    if(m!=y.m || n!=y.n)
        throw RozmiaryNiePasuja(n,m,y.n,y.m,0);

    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++)
            t[i][j]+=y.t[i][j];

    return *this;
}

Macierz operator+ (const Macierz &x, const Macierz &y) throw (RozmiaryNiePasuja) {

    Macierz result = x;
    result += y;
    return result;

}


Macierz& Macierz::operator-= (const Macierz &y) throw(RozmiaryNiePasuja) {

    if(m!=y.m || n!=y.n)
        throw RozmiaryNiePasuja(n,m,y.n,y.m,0);

    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++)
            t[i][j]-=y.t[i][j];

    return *this;
}

Macierz operator- (const Macierz &x, const Macierz &y) throw (RozmiaryNiePasuja) {

    Macierz result = x;
    result -= y;
    return result;

}


Macierz& Macierz::operator*= (const Macierz &y) throw(RozmiaryNiePasuja) {

    Macierz kopia = *this;

    if(m!=y.n || n!=y.m)
        throw RozmiaryNiePasuja(n,m,y.n,y.m,1);

    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++)
            t[i][j]=iloczyn_pola(kopia,y,i,j);

    return *this;
}

Macierz operator* (const Macierz &x, const Macierz &y) throw (RozmiaryNiePasuja) {

    Macierz result = x;
    result *= y;
    return result;

}


Macierz operator! (const Macierz &x)
{

    Macierz result(x.n,x.m);

    //std::cout<<"result m: "<<result.m<<std::endl;

    for(int i=0; i<result.m; i++)
        result.t[i] = Macierz::wektor_kolumna_do_wiersza(x,i+1);


    return result;
}




/// ----- ----- -----
/// wejscie/wyjscie

void Macierz::wypisz() const {

    for(int i=0; i<m; i++) {
        for(int j=0; j<n; j++)
            std::cout<<t[i][j]<<" ";
        std::cout<<std::endl;
    }

}

std::istream & operator>> (std::istream &we, Macierz &x) throw (ZleWejscieMacierzy) {
    /// prawidlowy strumien: '(_n_m_)(_a[00]_a[01]_a[02]_..._a[nm]_)'

    std::string wejscie;
    we>>wejscie;

    // usuwanie tego co bylo w x
    for(int i=0; i<x.m; i++)
         delete[] x.t[i];
    delete[] x.t;
    x.n=0, x.m=0;
    // ---

    std::vector<std::string> vec = Macierz::podziel_tekst(wejscie);

    if(vec[0]!="(" || vec[3]!=")(" || vec[(int)vec.size()-1]!=")")
        throw ZleWejscieMacierzy(wejscie,0);


    // inicjowanie pustego
    x.n = stoi(vec[1]);
    x.m = stoi(vec[2]);
    x.t = new double *[x.m];
    for(int i=0; i<x.m; i++)
        x.t[i] = new double [x.n];
    // ---

    if((int)vec.size() != (x.n*x.m + 5))
        throw ZleWejscieMacierzy(wejscie,1);


    // wypelnianie
    int k=4;
    for(int i=0; i<x.m; i++)
        for(int j=0; j<x.n; j++)
            x.t[i][j]=stod(vec[k++]);

    return we;
}

std::ostream & operator<< (std::ostream &wy, const Macierz &x) {

    wy<<"(_"<<x.n<<"_"<<x.m<<"_)(";
    for(int i=0; i<x.m; i++)
        for(int j=0; j<x.n; j++)
            wy<<"_"<<std::to_string(x.t[i][j]);
    wy<<"_)";

    return wy;
}



/// wyjatki

BladMacierz::BladMacierz() {
    info = "Nieznany powod";
}

BladMacierz::BladMacierz(const BladMacierz &b) {
    info = b.info;
}

BladMacierz& BladMacierz::operator= (const BladMacierz &b) {
    info = b.info;
    return *this;
}

BladMacierz::~BladMacierz() {}

const char* BladMacierz::what() const throw() {
    return info.c_str();
}

RozmiaryNiePasuja::RozmiaryNiePasuja(int r1n, int r1m, int r2n, int r2m, int rodzaj) {
    this->r1n=r1n;
    this->r1m=r1m;
    this->r2n=r2n;
    this->r2m=r2m;
    this->rodzaj=rodzaj;
}

const char* RozmiaryNiePasuja::what() const throw() {

    std::cerr<<"r1n: "<<this->r1n
             <<"r1m: "<<this->r1m
             <<"r1m: "<<this->r2n
             <<"r2m: "<<this->r2m<<std::endl;

    switch(rodzaj) {

        case 0:
            return "+/-, wymagane: r1n==r2n && r1m==r2m";
            break;
        case 1:
            return "*, wymagane: r1n=r2m && r1m==r2n";
            break;
        default:
            return "";
    }

}


ZleWejscieMacierzy::ZleWejscieMacierzy(std::string w, int p) {
    wejscie = w;
    powod = p;
}

const char* ZleWejscieMacierzy::what() const throw() {
    std::cerr<<"wejscie: "<<wejscie<<std::endl;

    switch(powod) {
    case 0:
        return "Blad nawiasowania (schemat)";
        break;
    case 1:
        return "Blad argumentow (niepoprawna ilosc)";
    default:
        return "";
    }

}

const char* DetZero::what() const throw() {
    return "Det=0, uniemozliwia to wykonanie dzialania!";
}


// ---
}
