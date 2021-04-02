// cpp stl Bartlomiej Grochowski 300951
// zadanie 2 - funktor do składania funkcji

#include <iostream>
#include <functional>

using namespace std;

// g - bierze X, zwraca G
// h - bierze X, zwraca H
// F - bierze G,H zwraca F
template <typename X, typename F, typename G, typename H>
struct compose_f_gx_hy {

    function<F(G,H)> f;
    function<G(X)> g;
    function<H(X)> h;

    compose_f_gx_hy(function<F(G,H)> f, function<G(X)> g, function<H(X)> h)
        : f(f), g(g), h(h) {}

    F operator()(X x) {
        return f( g(x) , h(x) );
    }
};

int main() {

    // G: int -> int ............. x^2
    // H: int -> string .......... to_string
    // F: int, string -> string .. to_string(a1) + a2 = res

    // -------------- x --- Fret -- Gret -- Hret --
    compose_f_gx_hy< int , string , int , string > composed(
        [](int a1, string a2) {                  // F
            return to_string(a1) + 
              " + " + a2 + " = " +
              to_string(a1 + atoi(a2.c_str()));
        },        
        [](int x) { return x*x; },               // G
        [](int x) { return to_string(x); }       // H
    );

    cout << composed(3) << endl;
    cout << composed(5) << endl;
    cout << composed(10) << endl;
    cout << composed(13) << endl;

    return 0;
}