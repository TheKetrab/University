// cpp stl Bartlomiej Grochowski 300951
// zadanie 3 - metaprogramowanie: gcd

#include <iostream>

#define a 558
#define b 10266

using namespace std;

template <int A, int B>
struct gcd {
    constexpr int operator()() const {
        if constexpr (A >= B)
            return gcd<B, A%B>()();
        else
            return gcd<B, A>()();
    };
};

template <int A>
struct gcd<A, 0> {
    constexpr int operator()() const { return A; };
};

int main() {
    constexpr int res = gcd<a,b>()();
    cout << res << endl;
}