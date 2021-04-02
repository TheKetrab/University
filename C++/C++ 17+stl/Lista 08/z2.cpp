// cpp stl Bartlomiej Grochowski 300951
// zadanie 2 - Zeta Riemanna i Eta Dirichleta + wykres w punkcie krytycznym
//   UWAGA: wymaga zainstalowanego programu: 'gnuplot'

#include <iostream>
#include <complex>
#include <fstream>

using namespace std;
using comd = complex<double>;

const string OUTPUT_DATA = "data.csv";
const string PLOT_SCRIPT = "dzeta.gnuplot";

#define error { \
    cerr << "Run program with: ./z2 num_of_iters(uint) begin_real(double) end_real(double) num_of_points(uint)" << endl; \
    cerr << "Eg. ./z2 1000 -50 50 10000" << endl; \
    return EXIT_FAILURE; \
}

string gnuplot_script =
    "set grid\nset yzeroaxis\nset xzeroaxis\n"
    "set title \"c = Dzeta( 0.5 + t*i )\"\n"
    "set xlabel \"t\"\n"
    "plot \"" + OUTPUT_DATA + "\" using 1:2 with lines title \"Real(c)\", \\\n"
    "     \"" + OUTPUT_DATA + "\" using 1:3 with lines title \"Imag(c)\"";

/*
// tylko dla R(z) > 1
comd dzeta_Rgr1(comd z, uint64_t n) {

    comd sum = 0.0;
    for (uint64_t i = n; i >= 1; i--)
        sum += 1.0 / pow(i, z);

    return sum;
}
*/

// Eta & Zeta dependence:
// https://www.youtube.com/watch?v=9-fZzwvYjdc
// N(s) = (1 - 2^(1-s)) * Z(s)
// N(s) = SUM(n:1,INF)[ (-1)^(n-1) / n^s ]

comd dirichlet_eta(comd s, uint64_t N) {

    comd sum = 0.0;
    for (uint64_t n = N; n>=1; n--)
        sum += ((n%2) ? 1.0 : -1.0) / pow(n,s);

    return sum;
}

comd riemann_zeta(comd s, uint64_t N) {

    comd coef = (1.0 - pow(2,1.0-s));
    return dirichlet_eta(s,N) / coef;
}

int main(int argc, const char* argv[]) {

    if (argc != 5) error;

    try {
        
        ofstream script(PLOT_SCRIPT);
        script << gnuplot_script;
        script.close();

        uint64_t N = stoull(argv[1]);
        double a = stod(argv[2]);
        double b = stod(argv[3]);
        uint64_t K = stoull(argv[4]);

        if (b < a) swap(a,b);

        double step = (b-a) / (double)K;

        ofstream file(OUTPUT_DATA);
        for (double i = a; i < b; i += step) {
            comd c(0.5,i); comd zc = riemann_zeta(c,N);
            file << i << " " << zc.real() << " " << zc.imag() << endl;
        }

        file.close();

        string cmd = "gnuplot -p " + PLOT_SCRIPT;
        system(cmd.c_str());
        
    }

    catch (exception &e) {
        error;
    }

    return EXIT_SUCCESS;
}

