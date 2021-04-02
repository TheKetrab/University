#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <stdexcept>

// Bartlomiej Grochowski
// nr 300 951


using namespace std;

int64_t stworz_int64(string liczba);
vector<int64_t> rozklad(int64_t n);
void wypisz_czynniki(vector<int64_t> tab, int64_t n);
bool czy_pierwsza(int64_t n);
bool czy_cyfra(char c);

int main(int argc, char *argv[])
{

    if(argc < 2) {
        cerr<<"Wywolaj program ponownie w nastepujacy sposob: "<<endl<<"Li1 liczba_1 liczba_2 ... liczba_n"<<endl<<"gdzie argumenty sa liczbami calkowitymi z przedzialu:"<<endl<<"[ "<<LLONG_MIN<<" , "<<LLONG_MAX<<" ]"<<endl;
        return 1;
    }

    string argument;
    int64_t liczba;
    vector<int64_t> tab;

    for(int i=1; i<argc; i++) {

        argument = argv[i];
        liczba = stworz_int64(argument);
        tab = rozklad(liczba);

        wypisz_czynniki(tab,liczba);

    }

    return 0;
}

int64_t stworz_int64(string liczba)
{
    // ZAKRES: -9223372036854775808  -  9223372036854775807

    int64_t wynik=0;
    bool ujemna=false;

    string niepoprawne = "Argumenty to nie liczby...";

    try  {

        // jesli zaczyna sie zerem
        if(liczba[0]=='0' && liczba[1]=='\0') return 0;
        else if(liczba[0]=='0' || (liczba[0]=='-' && liczba[1]=='0')) throw invalid_argument("liczba nie moze zaczynac sie od cyfry 0.");

        // zbadaj czy pierwszy znak stringa to '-'
        if(liczba[0]=='-') { ujemna=true; wynik=-0; }
        else if(czy_cyfra(liczba[0])) wynik+=(liczba[0]-'0');
        else throw invalid_argument("niepozadany znak - to nie jest liczba.");

        // Horner
        for(int i=1; liczba[i]!='\0'; i++) {
            if(!czy_cyfra(liczba[i])) invalid_argument("niepozadany znak - to nie jest liczba.");
            wynik*=10;
            if(ujemna==true) wynik-=(liczba[i]-'0');
            else wynik+=(liczba[i]-'0');

            // czy przekroczono zakres?
            if(ujemna==false && wynik<0) throw invalid_argument("przekroczenie zakresu.");
            else if(ujemna==true && wynik>0) throw invalid_argument("przekroczenie zakresu.");
        }

    }

    catch(const invalid_argument& ia) {
        cerr<<"Invalid argument: "<<ia.what()<<endl;
        exit(1);
    }


    return wynik;

}


vector<int64_t> rozklad(int64_t n)
{
    vector<int64_t> wynik;

    // jesli jest postaci -1 / 0 / 1
    if(n==-1) { wynik.push_back(-1); return wynik; }
    if(n==0) { wynik.push_back(0); return wynik; }
    if(n==1) { wynik.push_back(1); return wynik; }


    // jesli ujemna
    if(n<0) {
        wynik.push_back(-1);

        // znajduje dzielnik i dzieli przez niego
        // dopiero teraz przechodzi na dodatnie,
        // zeby miec pewnosc, ze zakres jest ok.

        bool znaleziono_dzielnik=false;

        for(int64_t i=-2; i>=-(int64_t)sqrt(-n); i--)
            if(n%i == 0) {
                wynik.push_back(-i);
                n/=i;
                znaleziono_dzielnik=true;
                break;
            }

        if(znaleziono_dzielnik==false) { // czyli jest pierwsza, ale ujemna
            wynik.push_back(-n);
            return wynik;
        }

    }


    // rozklad na czynniki
    while(!czy_pierwsza(n)) {

        // jesli nie jest pierwsza to istnieje cos przez co sie dzieli
        for(int64_t i=2; i<=(int64_t)sqrt(n); i++)
            if(n%i == 0) {
                wynik.push_back(i);
                n/=i;
                break;
            }


    }

    wynik.push_back(n);

    return wynik;

}

void wypisywanie(int64_t i)
{
    cout<<" * "<<i;
}

void wypisz_czynniki(vector<int64_t> tab, int64_t n)
{
    cout<<n<<" = ";
    cout<<tab[0];

    for_each(tab.begin()+1, tab.end(), wypisywanie);
    cout<<endl;

}


bool czy_pierwsza(int64_t n)
{
    //if(n==9223372036854775783) return true;

    for(int64_t k=2; k<=(int64_t)sqrt(n); k++)
        if(n%k==0) return false;

    return true;
}


bool czy_cyfra(char c)
{
    if(c>=48 && c<=57) return true;
    return false;
}
