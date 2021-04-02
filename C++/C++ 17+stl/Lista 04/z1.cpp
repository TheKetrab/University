// cpp stl Bartlomiej Grochowski 300951
// zadanie 1 - for_each i lambdy

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <iterator>
#include <string>

using namespace std;

vector<double> v = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
list<string> l = { "str1", "abc", "xyz", "lista 4", "funktory" };
set<int> s = { -1, -15, 5, 344, 555, 0, 128, -50, 11 };

// wypisz wartości pomiędzy A i B
template <typename Container, typename T>
void print_if_between(Container &c, T a, T b) {

    for_each(c.begin(),c.end(),[a,b](T v) {
        if (v > a && v < b)
            cout << v << " ";
    });
}

void A() {

    print_if_between(v,2.80,6.30);
    cout << endl;

    print_if_between(l,string("a"),string("to jest to"));
    cout << endl;

    print_if_between(s,-5,10);
    cout << endl;
}

// wypisz od pozycji p co k'tą
template<typename Container>
void print_kth_num_from_p(const Container& c, int k, int p) {

    auto iterator = c.begin();
    int cnt = 0;

    for_each(c.begin(),c.end(), [&](auto v) {
        if (distance(c.begin(),iterator) < p) 
            iterator++;
        else {
            if (cnt % k == 0) cout << v << " ";
            cnt++;
        }
    });
}

void B() {

    print_kth_num_from_p(v,2,3); // wypisz co drugą, zaczynając od trzeciej
    cout << endl;

    print_kth_num_from_p(l,3,0); // wypisz co trzecią, zaczynając od początku
    cout << endl;

    print_kth_num_from_p(s,2,10); // wypisz co drugą, zaczynając od dziesiątej
    cout << endl;

}

// wyznacz średnią arytmetyczną
template<typename Container>
double avg(const Container& c) {

    double sum = 0;
    int n = c.size();

    for_each(c.begin(), c.end(), [&sum](auto v) {
        sum += v;
    });

    return sum / (double)n;
}

void C() {

    cout << "Avg of vector<double> --> " << avg(v) << endl; 
    cout << "Avg of set<int> --------> " << avg(s) << endl; 
}

// min max
template <typename Container>
auto get_min_max(const Container &c) {

    auto min = c.begin(), max = c.begin(), iterator = c.begin();

    for_each(c.begin(), c.end(), [&](auto& v) {
        if (v < *min) min = iterator;
        if (v > *max) max = iterator;
        iterator++;
    });

    return pair { min, max };
};

void D() {

    auto minmax_v = get_min_max(v);
    auto minmax_l = get_min_max(l);
    auto minmax_s = get_min_max(s);

    cout << "Vector => min is " << *(minmax_v.first) << " max is " << *(minmax_v.second) << endl; 
    cout << "List   => min is " << *(minmax_l.first) << " max is " << *(minmax_l.second) << endl; 
    cout << "Set    => min is " << *(minmax_s.first) << " max is " << *(minmax_s.second) << endl; 
}

// sum / concat
template <typename Container>
auto sum(const Container &c) {

    auto sum = *(c.begin());
    bool fs = false; // first skipped

    for_each(c.begin(), c.end(), [&](auto& v) {
        if (!fs) fs = true;
        else sum += v;        
    });

    return sum;
};

void E() {

    cout << "Vector => sum is ..... " << sum(v) << endl;
    cout << "List   => concat is .. " << sum(l) << endl;
    cout << "Set    => sum is ..... " << sum(s) << endl;
}


int main() {
    A();
    B();
    C();
    D();
    E();
    return 0;
}
