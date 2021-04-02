#include <iostream>
#include "bst.hpp"
#include "bst.cpp"

using namespace std;
using namespace struktury;

void main_loop();

int main()
{


    /// ALFABETYCZNIE
    const char* n1 = "aaa";
    const char* n2 = "abcadlo";
    const char* n3 = "piec";
    const char* n4 = "spadlo";
    const char* n5 = "a";
    bst<const char*> *alfabetycznie = new bst<const char*>({n1,n2,n3,n4,n5});
    cout<<"ALFABETYCZNIE: "<<*alfabetycznie<<endl;
    /// ----- ----- -----


    /// NORMALNE
    bst<int> *test = new bst<int>({1,2,33,23,4,11,5,66,7,0});
    cout<<"NORMALNIE: "<<*test<<endl;


    /// REVERSE
    bst<int,rev<int>> *tr = new bst<int,rev<int>>(5);
    tr->wstaw(3);
    tr->wstaw(4);
    tr->wstaw(1);
    tr->wstaw(8);
    tr->wstaw(9);
    tr->wstaw(7);
    tr->usun(5);
    tr->wypisz(); cout<<" <-- drzewo"<<endl;
    cout<<"REVERSE: "<<*tr<<std::endl;

    delete alfabetycznie;
    delete test;
    delete tr;

    main_loop();

    return 0;
}


void main_loop() {

    bst<int> *tree = new bst<int>;

    int wybor;
    int liczba;
    while(true) {

        cout<<"----- ----- ----- ----- -----"<<endl;
        cout<<"DRZEWO: "; tree->wypisz(); cout<<endl;
        cout<<"1. wstaw"<<endl;
        cout<<"2. usun"<<endl;
        cout<<"3. wyszukaj"<<endl;
        cout<<"def. wyjscie"<<endl;

        cin>>wybor;
        switch(wybor) {

        case 1:
            cout<<"Podaj liczbe do wstawienia: ";
            cin>>liczba;
            tree->wstaw(liczba);
            break;
        case 2:
            cout<<"Podaj liczbe do usuniecia: ";
            cin>>liczba;
            tree->usun(liczba);
            break;
        case 3:
            cout<<"Podaj liczbe do znalezienia: ";
            cin>>liczba;
            (tree->wyszukaj(liczba)).wypisz(); cout<<endl;
            break;
        default:
            delete tree;
            return;


        }
    }

}


