// cpp stl Bartlomiej Grochowski 300951
// zadanie 1 - dokładne obliczenia Gammy
//   zalecana dokładność: 10000000 (N = 10 milionów iteracji)


#include <iostream>
#include <complex>

using namespace std;

const long double em = 0.5772156649;
using comld = complex<long double>;

#define error { \
    cerr << "Run program with: ./z1 N(uint) REAL(double) IMAG(double)" << endl; \
    cerr << "Eg: ./z1 1000000 4 3" << endl; \
    return EXIT_FAILURE; \
}

comld gamma(comld z, uint64_t n) {
    
    comld product = 1.0l;
    for (long double i=n; i>=1; i--)
        product *= pow(1.0l + 1.0l / i , z) / (1.0l + z/i);

    return product / z;
}

comld gamma_inv(comld z, uint64_t n) {

    comld product = 1.0l;
    for (long double i=n; i>=1; i--)
        product *= (1.0l + z/i) * exp(-z/i);

    return z * exp(em*z) * product;
} 

int main(int argc, const char* argv[]) {

    if (argc != 4) error;

    try {

        uint64_t n = stoull(argv[1]);
        long double real = stold(argv[2]);
        long double imag = stold(argv[3]);

        comld c(real,imag);
        cout << "    c = " << c << endl;
        cout << "A:  Γ [ " << c << " ]   =   " << gamma(c,n) << endl;
        cout << "B: 1/Γ[ " << c << " ]   =   " << gamma_inv(c,n) << endl;

    }

    catch (exception &e) {
        error;
    }

    return EXIT_SUCCESS;
}