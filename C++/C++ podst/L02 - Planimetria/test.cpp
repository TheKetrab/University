#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "test.hpp"
#include "pwp.hpp"

punkt *tab_punktow[25];
wektor *tab_wektorow[25];
prosta *tab_prostych[25];
int ilosc_punktow=0;
int ilosc_wektorow=0;
int ilosc_prostych=0;

using namespace std;

void wybor_glowny() {
    cout<<" --- --- --- --- --- "<<endl;
    cout<<"Co chcesz zrobic?"<<endl;
    cout<<"1. Dodac obiekt."<<endl<<"2. Wywolac funkcje."<<endl<<"3. Wypisac baze danych."<<endl<<"4. Pobrac baze danych."<<endl<<"5. Zakonczyc program."<<endl;
    int wybor; cin>>wybor;

    switch(wybor) {

    wypisz_dane_punktu(0);
    case 1:
        wybor_dodawanie_obiektu();
        break;
    case 2:
        wybor_wywolanie_funkcji();
        break;
    case 3:
        wypisz_baze_danych();
        wybor_glowny();
        break;
    case 4:
        //odczytaj_baze_danych_z_pliku();
        break;
    default:
        exit(0);
    }
}

void wybor_dodawanie_obiektu() {
    cout<<" --- --- --- --- --- "<<endl;
    cout<<"Jaki obiekt chcesz dodac?"<<endl;
    cout<<"1. Punkt (x,y)."<<endl<<"2. Punkt (punkt, wektor)."<<endl;
    cout<<"3. Wektor (dx,dy)."<<endl;
    cout<<"4. Prosta (punkt, punkt)."<<endl<<"5. Prosta (wektor)."<<endl<<"6. Prosta (a,b,c)."<<endl<<"7. Prosta (prosta, wektor)."<<endl;
    int wybor; cin>>wybor;

    wypisz_baze_danych();
    cout<<"Podaj odpowiednie parametry."<<endl;

    switch(wybor) {

        case 1:
        {
            double x,y; cin>>x>>y;
            punkt p(x,y);
            tab_punktow[ilosc_punktow]=&p;
            ilosc_punktow++;
            break;
        }

        case 2:
        {
            int id_p; int id_v; cin>>id_p>>id_v;
            punkt p(*tab_punktow[id_p],*tab_wektorow[id_v]);
            tab_punktow[ilosc_punktow]=&p;
            ilosc_punktow++;
            break;
        }

        case 3:
        {
            double dx,dy; cin>>dx>>dy;
            wektor v(dx,dy);
            tab_wektorow[ilosc_wektorow]=&v;
            ilosc_wektorow++;
            break;
        }

        case 4:
        {
            int id_p1, id_p2; cin>>id_p1>>id_p2;
            prosta l(*tab_punktow[id_p1],*tab_punktow[id_p2]);
            tab_prostych[ilosc_prostych]=&l;
            ilosc_prostych++;
            break;
        }

        case 5:
        {
            int id_v; cin>>id_v;
            prosta l(*tab_wektorow[id_v]);
            tab_prostych[ilosc_prostych]=&l;
            ilosc_prostych++;
            break;
        }

        case 6:
        {
            double a,b,c; cin>>a>>b>>c;
            prosta l(a,b,c);
            tab_prostych[ilosc_prostych]=&l;
            ilosc_prostych++;
            break;
        }

        case 7:
        {
            int id_l,id_v; cin>>id_l>>id_v;
            prosta l(*tab_prostych[id_l],*tab_wektorow[id_v]);
            tab_prostych[ilosc_prostych]=&l;
            ilosc_prostych++;
            break;
        }


    }

    cout<<"Twoja baza danych zostala uaktualniona!"<<endl;
    wybor_glowny();
}

void wypisz_baze_danych()
{
    cout<<" --- --- --- --- --- "<<endl;
    cout<<"Baza danych:"<<endl;
    for(int i=0; tab_punktow[i]!=nullptr && i<25; i++) {
        cout<<"Pkt "<<i<<" : "; wypisz_dane_punktu(i); }

    for(int i=0; tab_wektorow[i]!=nullptr && i<25; i++) {
        cout<<"Wek "<<i<<" : "; wypisz_dane_wektora(i); }

    for(int i=0; tab_prostych[i]!=nullptr && i<25; i++) {
        cout<<"Pro "<<i<<" : "; wypisz_dane_prostej(*tab_prostych[i]); }

}

void wypisz_dane_punktu(int id)
{
    cout<<"( "<<tab_punktow[id]->x<<" , "<<tab_punktow[id]->y<<" )"<<endl;
}

void wypisz_dane_wektora(int id)
{
    cout<<"[ "<<tab_wektorow[id]->dx<<" , "<<tab_wektorow[id]->dy<<" ]"<<endl;
}


void wybor_wywolanie_funkcji() {
    cout<<" --- --- --- --- --- "<<endl;
    cout<<"Jaka funkcje chcesz wywolac?"<<endl;
    cout<<"1. czy_wektor_prostopadly(prosta, wektor)"<<endl;
    cout<<"2. czy_wektor_rownlolegly(prosta, wektor)"<<endl;
    cout<<"3. czy_punkt_nalezy(punkt, prosta)"<<endl;
    cout<<"4. czy_proste_prostopadle(prosta, prosta)"<<endl;
    cout<<"5. czy_proste_rownolegle(prosta, prosta)"<<endl;
    cout<<"6. przeciecie_sie_prostych(prosta, prosta)"<<endl;
    cout<<"7. odl_pkt_od_prostej(punkt, prosta)"<<endl;
    int wybor; cin>>wybor;

    wypisz_baze_danych();
    cout<<"Podaj odpowiednie parametry."<<endl;

    switch(wybor) {

        case 1:
        {
            int id_l, id_v; cin>>id_l>>id_v;
            if(tab_prostych[id_l]->czy_wektor_prostopadly(*tab_wektorow[id_v])) cout<<"TAK"<<endl;
            else cout<<"NIE"<<endl;
            break;
        }

        case 2:
        {
            int id_l, id_v; cin>>id_l>>id_v;
            if(tab_prostych[id_l]->czy_wektor_rownolegly(*tab_wektorow[id_v])) cout<<"TAK"<<endl;
            else cout<<"NIE"<<endl;
            break;
        }


        case 3:
        {
            int id_p, id_l; cin>>id_p>>id_l;
            if(tab_prostych[id_l]->czy_punkt_nalezy(*tab_punktow[id_p])) cout<<"TAK"<<endl;
            else cout<<"NIE"<<endl;
            break;
        }

        case 4:
        {
            int id_l, id_m; cin>>id_l>>id_m;
            if(czy_proste_prostopadle(*tab_prostych[id_l],*tab_prostych[id_m])) cout<<"TAK"<<endl;
            else cout<<"NIE"<<endl;
            break;
        }

        case 5:
        {
            int id_l, id_m; cin>>id_l>>id_m;
            if(czy_proste_rownolegle(*tab_prostych[id_l],*tab_prostych[id_m])) cout<<"TAK"<<endl;
            else cout<<"NIE"<<endl;
            break;
        }

        case 6:
        {
            int id_l, id_m; cin>>id_l>>id_m;
            if(czy_proste_rownolegle(*tab_prostych[id_l],*tab_prostych[id_m]))
                cout<<"Taki punkt nie istnieje."<<endl;
            else {
                punkt p((przeciecie_sie_prostych(*tab_prostych[id_l],*tab_prostych[id_m])).x,(przeciecie_sie_prostych(*tab_prostych[id_l],*tab_prostych[id_m])).y);
                tab_punktow[ilosc_punktow]=&p;
                ilosc_punktow++;
                cout<<"( "<<p.x<<" , "<<p.y<<" )"<<endl;
            }
            break;
        }

        case 7:
        {
            int id_p, id_l; cin>>id_p>>id_l;
            cout<<tab_prostych[id_l]->odl_pkt_od_prostej(*tab_punktow[id_p])<<endl;
            break;
        }


    }

    wybor_glowny();
}
/*
void odczytaj_baze_danych_z_pliku()
{
    fstream plik;
    plik.open("baza_danych", std::ios::in);
    if(plik.good() == true) {
        cout << "Uzyskano dostep do pliku!" << std::endl;

        int dane=0;
        string napis;
        while(!plik.eof())
        {
            getline(plik, napis);
            if(napis=="PUNKT") { cout<<"napis: "<<napis<<endl; dane=1; }
            else if(napis=="WEKTOR") { cout<<"napis: "<<napis<<endl; dane=2; }
            else if(napis=="PROSTA") { cout<<"napis: "<<napis<<endl; dane=3; }
            else if(napis=="") { cout<<"pusty!"<<endl; }
            else { cout<<"dodaje! napis: "<<napis<<endl; dodaj_do_danych(napis,dane); }
        }


    } else cout<<"Dostep do pliku zostal zabroniony!"<<endl;
    plik.close();


    wybor_glowny();

}

void dodaj_do_danych(string s, int rodzaj)
{
    double liczba=0;
    int ile_m_po_przecinku=0;

    switch(rodzaj) {

    case 1:
        double x,y;

        int wsk=0;
        bool przecinek=false;

        for(wsk;s[wsk]!=' ';wsk++) {

            if(s[wsk]=='.') przecinek=true;
            else {
                liczba*=10.0;
                liczba+=s[wsk]-'0';
                if(przecinek) ile_m_po_przecinku++;
            }
        }

        liczba/=pow(10,ile_m_po_przecinku);
        x=liczba;

        wsk++;
        przecinek=false;
        liczba=0;

        for(wsk;s[wsk]!='\0';wsk++) {

            if(s[wsk]=='.') przecinek=true;
            else {
                liczba*=10;
                liczba+=s[wsk]-'0';
                if(przecinek) ile_m_po_przecinku++;
            }
        }

        liczba/=pow(10,ile_m_po_przecinku);
        y=liczba;

        punkt p(x,y);
        tab_punktow[ilosc_punktow]=&p;
        ilosc_punktow++;



        break;


    }


    wypisz_baze_danych();


}
*/
