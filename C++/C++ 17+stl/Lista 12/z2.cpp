// cpp stl Bartlomiej Grochowski 300951
// zadanie 2 - metaprogramowanie: n po k

#include <iostream>
#define n 42
#define k 17
using namespace std;

// (n-1)_(k-1) * n/k 
//   = n/k * [(n-1)! / (k-1)!(n-1-k+1)] = 
//   = n! / k!(n-k)!
//   = n_k
//
// n_k = (n-1)_(k-1) * n/k

template <uint64_t N, uint64_t K>
struct binomial {
    static_assert(N > K,"N must be greater than K");
    constexpr uint64_t operator()() {
        return (binomial<N-1,K-1>()() * N) / K;
    }
};

template <>
struct binomial<0,0> {
    constexpr uint64_t operator()() const { return 0; }
};

template <uint64_t N>
struct binomial<N,0> {
    constexpr uint64_t operator()() const { return 1; }
};

template <uint64_t N>
struct binomial<N, N> {
    constexpr uint64_t operator()() const { return 1; }
};

int main() {

    constexpr uint64_t coefficient = binomial<n,k>()();
    cout << coefficient << endl;

}