#include <iostream>
#include "stos.hpp"

#define MAX_AMO_S 20

using namespace std;


stos *baza_danych[MAX_AMO_S];
int ilosc_stosow=0;

void glowny_wybor();
void dodawanie_stosu_wybor();
void przenoszenie_stosu_wybor();
void wypisz_baze_danych();
void wywolaj_funkcje_wybor();
void dodawanie_s_ilist();

/* inne testy

    //Przypisanie kopiujace

    stos a = {"jeden","dwa","trzy","cztery"};
    stos b = {"alfa","beta","gamma"};

    a.wypisz_stos();
    b.wypisz_stos();
    a=b;
    a.sciagnij();
    a.wloz("nowy");
    a.wypisz_stos();
    b.wypisz_stos();

    // ----- ----- -----


    //Przypisanie przenoszace

    stos s1 = stos({"xxxxx"});
    s1 = stos({"x","a"});
    s1.wypisz_stos();
    cout<<"rozmiar: "<<s1.rozmiar()<<endl;

    // ----- ----- -----


    //Konstruktor kopiujacy

    stos a = stos(12);
    stos b = a;

    // ----- ----- -----


    //Sciaganie

    stos *c = new stos({"a","aa"});
    c->wypisz_stos();
    c->sciagnij();
    c->wypisz_stos();

    // ----- ----- -----


    //Konstruktor przenoszacy

    stos siema = {"jeden"};
    stos x(move(siema));
    x.wypisz_stos();

    // ----- ----- -----
*/


int main()
{

    try {
        glowny_wybor();
    }
    catch(const invalid_argument& ia) {
        cerr<<"Invalid argument: "<<ia.what()<<endl;
        exit(1);
    }

    return 0;
}

void glowny_wybor()
{
    cout<<"----- ----- -----"<<endl;
    cout<<"Co chcesz zrobic?"<<endl;
    cout<<"1. Dodac stos."<<endl;
    cout<<"2. Wywolac funkcje."<<endl;
    cout<<"3. Wypisz baze danych."<<endl;
    cout<<"4. Zakonczyc program."<<endl;

    int wybor; cin>>wybor;
    switch(wybor) {

    case 1:
        dodawanie_stosu_wybor();
        break;
    case 2:
        wywolaj_funkcje_wybor();
        break;
    case 3:
        wypisz_baze_danych();
        break;
    case 4:
        // wyczyszczenie miejsca w pamieci
        for(int i=0; i<MAX_AMO_S; i++)
            delete baza_danych[i];
        exit(0);
    }

    glowny_wybor();


}

void wypisz_baze_danych()
{
    cout<<"----- ----- -----"<<endl;
    cout<<"BAZA DANYCH :"<<endl;

    for(int i=0; i<ilosc_stosow; i++) {
        cout<<endl<<" -> STOS "<<i<<endl;
        baza_danych[i]->wypisz_stos();
    }

}

void dodawanie_stosu_wybor()
{
    cout<<"----- ----- -----"<<endl;
    cout<<"Jaki stos chcesz dodac?"<<endl;
    cout<<"1. stos()"<<endl;
    cout<<"2. stos(int pojemnosc)"<<endl;
    cout<<"3. stos(initialization list)"<<endl;
    cout<<"4. stos(obiekt) - przypisanie kopiujace"<<endl;

    int wybor; cin>>wybor;
    wypisz_baze_danych();
    cout<<"Podaj odpowiednie parametry."<<endl;

    switch(wybor) {

    case 1:
        baza_danych[ilosc_stosow] = new stos();
        ilosc_stosow++;
        break;
    case 2:
        int x; cin>>x;
        baza_danych[ilosc_stosow] = new stos(x);
        ilosc_stosow++;
        break;
    case 3:
        dodawanie_s_ilist();
        break;
    case 4:
        int id; cin>>id;
        baza_danych[ilosc_stosow] = new stos();
        *baza_danych[ilosc_stosow] = *baza_danych[id];
        ilosc_stosow++;
        break;
    }


}

void wywolaj_funkcje_wybor()
{
    cout<<"----- ----- -----"<<endl;
    cout<<"Jaka funkcje chcesz wywolac?"<<endl;
    cout<<"1. wloz(obiekt, string)"<<endl;
    cout<<"2. sciagnij(obiekt)"<<endl;
    cout<<"3. sprawdz(obiekt)"<<endl;
    cout<<"4. rozmiar(obiekt)"<<endl;

    int wybor; cin>>wybor;
    wypisz_baze_danych();
    cout<<"Podaj odpowiednie parametry."<<endl;

    switch(wybor) {

        case 1:
        {
            int id; string x; cin>>id>>x;
            baza_danych[id]->wloz(x);
            break;
        }
        case 2:
        {
            int id; cin>>id;
            cout<<baza_danych[id]->sciagnij()<<endl;
            break;
        }
        case 3:
        {
            int id; cin>>id;
            cout<<baza_danych[id]->sprawdz()<<endl;
            break;
        }
        case 4:
        {
            int id; cin>>id;
            cout<<baza_danych[id]->rozmiar()<<endl;
            break;
        }

    }

}

void dodawanie_s_ilist()
{
    cout<<"Podaj ilosc argumentow (1-5)."<<endl;
    int il; cin>>il;

    string s1, s2, s3, s4, s5;

    switch(il) {

    case 1:
        cin>>s1;
        baza_danych[ilosc_stosow] = new stos({s1});
        ilosc_stosow++;
        break;
    case 2:
        cin>>s1>>s2;
        baza_danych[ilosc_stosow] = new stos({s1,s2});
        ilosc_stosow++;
        break;
    case 3:
        cin>>s1>>s2>>s3;
        baza_danych[ilosc_stosow] = new stos({s1,s2,s3});
        ilosc_stosow++;
        break;
    case 4:
        cin>>s1>>s2>>s3>>s4;
        baza_danych[ilosc_stosow] = new stos({s1,s2,s3,s4});
        ilosc_stosow++;
        break;
    case 5:
        cin>>s1>>s2>>s3>>s4>>s5;
        baza_danych[ilosc_stosow] = new stos({s1,s2,s3,s4,s5});
        ilosc_stosow++;
        break;
    }

}
