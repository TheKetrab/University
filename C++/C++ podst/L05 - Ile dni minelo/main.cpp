#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

#include "data.hpp"
#include "datagodz.hpp"
#include "wyd.hpp"

using namespace std;

int main()
{


    // TEST 1
    srand (time(NULL));
    vector<Wydarzenie> test;

    for(int i=0; i<100; i++) {
        int r = rand() % 30 + 2000;
        int m = rand() % 12 + 1;
        int d = rand() % 28 + 1; // tak, zeby nie zastanawiac sie dokladnie...
        int go = rand() % 24;
        int mi = rand() % 60;
        int se = rand() % 60;

        Wydarzenie w(DataGodz(d,m,r,go,mi,se),to_string(i));
        test.push_back( w );
    }

    sort(test.begin(), test.end());
    vector<Wydarzenie>::iterator it = test.begin();
    for(; it != test.end(); it++ )
        cout <<* it<<endl;


/*
    // TEST 2
    cout<<"DATA:"<<endl;
    Data a(31,12,1401);
    a++;
    a.wypisz_date();

    Data b(27,3,2018);
    b+=39;
    b.wypisz_date();

    Data aktualna;
    cout<<"Roznica b-aktualna: "<<(b-aktualna)<<endl;


    cout<<"DATAGODZ:"<<endl;
    DataGodz c(13,7,2008,13,51,0);
    cout<<c<<endl;
    c++;
    cout<<c<<endl;
    c-=2;
    cout<<c<<endl;
    DataGodz d;
    DataGodz e;
    cout<<"e==d? : "<<(d==e)<<endl;
    cout<<"c==e? : "<<(c==e)<<endl;
    cout<<"c<d? : "<<(c<d)<<endl;

    c+=1000000;

*/

    return 0;
}
