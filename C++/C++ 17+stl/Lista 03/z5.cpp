// cpp stl Bartlomiej Grochowski 300951
// zadanie 5 - chrono

#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>

using namespace std;

// global random
random_device rd;
mt19937 gen(rd());

// declarations
class matrix;
void test(int n, int k);
double rand_double();
ostream& operator<<(ostream& os, const matrix &m);

int main() {

    test(10,100000);
    test(100,1000);
    test(1000,1);

    return 0;
}

class matrix {

    int n;
    double* a;

public:

    matrix(int n) {
        this->n = n;
        a = new double[n*n];
        for (int i=0; i<n*n; i++)
            a[i] = rand_double();
    }

    ~matrix() {
        delete[] a;
    }

    void square() {

        double *res = new double[n*n];

        for (int i=0; i<n; i++)
            for (int j=0; j<n; j++) {
                res[n*i + j] = 0;
                for (int k=0; k<n; k++)
                    res[n*i + j] += a[n*i + k] * a[n*k + j];
            }

        delete[] a;
        this->a = res;
    }

    double get(int i, int j) const {

        if (i < 0 || i >= n || j < 0 || j >= n)
            return 0;

        return a[n*i + j];
    }

    friend ostream& operator<<(ostream&, const matrix &m);

};

ostream& operator<<(ostream& os, const matrix &m) {

    for (int i=0; i<m.n; i++) {
        for (int j = 0; j < m.n; j++)
            os << " " << m.get(i,j) << ' ';
        os << endl;
    }

    return os;
}

void test(int n, int k) {

    // matrix n x n, k iteracji
    auto start = chrono::high_resolution_clock::now();

    for (int i=0; i<k; i++) {
        matrix m(n);
        m.square();
    }

    auto stop = chrono::high_resolution_clock::now();

    chrono::duration<double> t = (stop-start) / k;
    cout << "m" << n << " square time: " << t.count() << endl;

}

double rand_double() {
    uniform_real_distribution<double> dis(0.5,2.0);
    return dis(gen);
}
