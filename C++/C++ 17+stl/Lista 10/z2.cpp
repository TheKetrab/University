// cpp stl Bartlomiej Grochowski 300951
// zadanie 2 - ostream_iterator

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>

#define FILE "phi.txt"

using namespace std;

// algorytm Euklidesa ( O(log(a+b)) )
uint64_t gcd(uint64_t a, uint64_t b) {

    if (a < b) swap(a,b); // a>b
    for (int c; b>0; c=b, b=a%b, a=c);
    return a;
}

// funkcja Eulera (tocjent):
// liczba względnie pierwszych liczb z k od 0 do k-1
uint64_t totient(uint64_t k) {

    uint64_t res = 0;
    for (uint64_t i=0; i<k; i++)
        if (gcd(k,i) == 1) res++;

    return res;
}

int main() {

    uint64_t k; cin >> k;

    vector<uint64_t> totients(k);
    for (uint64_t i=0; i<k; i++)
        totients[i] = totient(i);

    ofstream file(FILE);
    copy(totients.begin(),totients.end(),
        ostream_iterator<uint64_t>(file, "; "));

   return 0;
}