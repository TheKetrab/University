// cpp stl Bartlomiej Grochowski 300951
// zadanie 3 - funktor do wywoływania funkcji

#include <iostream>
#include <functional>

using namespace std;

template <class X, class F1, class F2>
class in_order {
private:
    function<F1(X&)> f1;
    function<F2(X&)> f2;

public:
    // konstruktor bierze dwie funkcje, które COŚ
    // zwracają, i które można zaaplikować do X (przez referencję)
    in_order(function<F1(X&)> f1, function<F2(X&)> f2) 
        : f1(f1), f2(f2) {}

    // operator aplikacji aplikuje x do f1 i f2,
    // ignoruje zwracane rezultaty i zwraca funkcję,
    // żeby można było wyniku 'in_order' użyc dalej
    function<X(X&)> operator()(X &x) {
        f1(x);
        f2(x);
        return [](X &x) { return x; };
    }

};

string f1(int &x) {
    cout << "f1, x = " << x << endl;
    x += 10;
    return to_string(x);
}

double f2(int &x) {
    cout << "f2, x = " << x-- << endl;
    return (double)x;
}

int f3(int &x) {
    cout << "f3, x = " << x << endl;
    return x * 100;
}

int main() {
    
    int x = 123;

    in_order<int,function<int(int&)>,int>(
        in_order<int,string,double>(&f1,&f2),&f3) (x);

    // in_order( in_order(&f1,&f2) , &f3 );

    return 0;
}
