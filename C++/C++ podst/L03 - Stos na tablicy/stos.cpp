#include <iostream>
#include <initializer_list>
#include <string>
#include "stos.hpp"

using namespace std;


stos::stos(int pojemnosc) {
    cerr<<"konstruktor - rozmiar"<<endl;
    if(pojemnosc<0) throw invalid_argument("niedodatni rozmiar stosu!");

    this->pojemnosc = pojemnosc;
    elementy = new string[pojemnosc];
}

stos::stos() : stos(1) { }

stos::stos(initializer_list<string> args) {

    cerr<<"konstruktor - lista inicjalizujaca"<<endl;

    pojemnosc = args.size();
    elementy = new string[args.size()];

    for(int i=0; i!=(int)args.size(); ++i) {
        elementy[i]=args.begin()[i];
        ile++;
    }

}

stos::stos(const stos &s) {

    cerr<<"konstruktor kopiujacy"<<endl;
    pojemnosc = s.pojemnosc;
    ile = s.ile;
    elementy = new string[pojemnosc];
    for (int i = 0; i < pojemnosc; i++) elementy[i] = s.elementy[i];
}

stos::stos(stos &s) {

    cerr<<"konstruktor kopiujacy"<<endl;
    pojemnosc = s.pojemnosc;
    ile = s.ile;
    elementy = new string[pojemnosc];
    for (int i = 0; i < pojemnosc; i++) elementy[i] = s.elementy[i];
}

stos::stos(stos &&s) {

    cerr<<"konstruktor przenoszacy"<<endl;
    pojemnosc = s.pojemnosc;
    ile = s.ile;
    elementy = s.elementy;
    //czyszczenie
    s.elementy = nullptr;
}

stos& stos::operator= (const stos &s) {

    cerr<<"przypisanie kopiujace"<<endl;

    // jesli a = a
    if(this == &s) return *this;

    if(pojemnosc>=s.pojemnosc) {
        // przypisanie
        for(int i=0; i<s.ile; i++)
            elementy[i] = s.elementy[i];

        // usuwanie pozostalosci po poprzedniku
        for(int i=s.ile; i<ile; i++)
            elementy[i] = "";

        pojemnosc = s.pojemnosc;
        ile = s.ile;
    }

    else {
        delete[] elementy;
        elementy = new string[s.pojemnosc];
        pojemnosc = s.pojemnosc;
        ile = s.ile;

        // przypisanie
        for(int i=0; i<s.pojemnosc; i++)
            elementy[i] = s.elementy[i];

    }

    return *this;
}

stos& stos::operator= (stos &&s) {

    cerr<<"przypisanie przenoszace"<<endl;
    swap(pojemnosc, s.pojemnosc);
    swap(ile, s.ile);
    swap(elementy, s.elementy);

    // jesli a = a
    /*if(this == &s) return *this;

    this->ile = ile;
    this->pojemnosc = pojemnosc;
    this->elementy = elementy;
    delete[] elementy;
*/
    /*if(pojemnosc>=s.pojemnosc) {
        // przypisanie
        for(int i=0; i<pojemnosc && i<s.pojemnosc; i++)
            elementy[i] = s.elementy[i];

        // usuwanie pozostalosci po poprzedniku
        for(int i=s.ile; i<ile; i++)
            elementy[i] = "";

        pojemnosc = s.pojemnosc;
        ile = s.ile;
    }

    else {
        elementy = new string[s.pojemnosc];
        pojemnosc = s.pojemnosc;
        ile = s.ile;

        // przypisanie
        for(int i=0; i<s.pojemnosc; i++)
            elementy[i] = s.elementy[i];

    }*/

    return *this;
}




stos::~stos() {
    delete[] elementy;
}


bool stos::pelny() {
    if(ile==pojemnosc) return true;
    return false;
}

bool stos::pusty() {
    if(ile==0) return true;
    return false;
}

void stos::wloz(string str) {
    if(pelny()==true) {
        throw invalid_argument("probujesz wlozyc element na pelny stos!");
        return;
    }

    elementy[ile] = str;
    ile++;
}

string stos::sciagnij() {
    if(pusty()==true) {
        throw invalid_argument("probujesz sciagnac element z pustego stosu!");
        return "";
    }

    string temp=elementy[ile-1];
    elementy[ile-1] = "";
    ile--;
    return temp;
}

string stos::sprawdz() {
    if(pusty()==true) {
        return "";
    }

    return elementy[ile-1];
}

int stos::rozmiar() {
    return pojemnosc;
}

void stos::wypisz_stos() {

    cout<<" | "<<endl;

    for(int i=ile-1; i>=0; i--) {
        cout<<" | "<<elementy[i]<<endl;
    }

    cout<<"   ----- -----"<<endl;
}
