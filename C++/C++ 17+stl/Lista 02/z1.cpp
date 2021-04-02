// cpp stl Bartlomiej Grochowski 300951
// zadanie 1 - unique ptr na tablicy

#include <iostream>
#include <memory>
#include <functional>
#include <ctime>

using namespace std;
#define N 5 // size of array

class counter {

    uint64_t val;

public:
    counter() : val(1) {}
    virtual ~counter() {
        cerr << "counter: " << val << endl;
    }

    counter& operator+=(uint64_t k) {
        this->val += k;
        return *this;
    }

};

using F = function<void(counter[])>;

void invoke_m(unique_ptr<counter[],F> ptr, int m) {

    if (m == 0) return;

    int k = -m + N; // 10 -> 0, ... , 1 -> 9

    // 10 losowych elementow zwiekszone o k
    for (int i=0; i<10; i++) {

        int idx = rand() % N;
        ptr.get()[idx] += k;

    }

    // zeby przekazac przez wartosc, trzeba uzyc funkcji move,
    // bo funkcja tworzy kopie wskaznika, a on jest unique,
    // wiec trzeba explicite przeniesc wskaznik do innego obiektu
    invoke_m(move(ptr),m-1);

}

int main() {

    srand(time(nullptr));

    unique_ptr<counter[],F> ptr(
        new counter[N], [](counter *p) { delete[] p; });

    invoke_m(move(ptr),N);
    
    if (ptr.get() == nullptr) {
        cout << "Ptr jest juz niedostepny - stracil referencje do obiektu." << endl;
    }

    return 0;
}