// cpp stl Bartlomiej Grochowski 300951
// zadanie 1 - metaprogramowanie: liczby Lucasa

#include <iostream>

using namespace std;

template <int N>
struct lucas {
    static const int val = lucas<N-1>::val + lucas<N-2>::val;
    constexpr int operator()() const {
        return lucas<N>::val;
    }
};

template <>
struct lucas<0> {
    static const int val = 2;
    constexpr int operator()() const {
        return lucas<0>::val;
    }
};

template <>
struct lucas<1> {
    static const int val = 1;
    constexpr int operator()() const {
        return lucas<1>::val;
    }
};

constexpr int f_lucas(int n) {
    int l0 = 2, l1 = 1;
    for (int i = 0; i < n; i++) {
        int temp = l0;
        l0 = l1;
        l1 = temp + l1;
    } return l0;
}


#define PRINT_LUCAS(I) { \
    cout << "Lucas[" << I << "]\t= " << (lucas<I>()()) \
        << "\t(" << f_lucas(I) << ")" << endl; \
}

int main() {

    PRINT_LUCAS(0);
    PRINT_LUCAS(1);
    PRINT_LUCAS(2);
    PRINT_LUCAS(3);
    PRINT_LUCAS(4);
    PRINT_LUCAS(5);
    PRINT_LUCAS(6);
    PRINT_LUCAS(7);
    PRINT_LUCAS(8);
    PRINT_LUCAS(9);
    PRINT_LUCAS(10);

    return 0;
}