// cpp stl Bartlomiej Grochowski 300951
// zadanie 6 - rownanie kwadratowe, zmienne lokalne w if-else

#include <iostream> 
#include <cmath>
#include <complex>
using namespace std;

void delta(double a, double b, double c) {

    const double eps = 0.00001;

    // b^2 - 4ac
    if (double delta = b*b - 4*a*c ; delta < eps && delta > -eps) {
        cout << "miejsce zerowe: " << -b / (2.0 * a) << endl;
    } else if (delta > 0) {
        const double sdelta = sqrt(delta);
        cout << "miejsca zerowe:" << endl;
        cout << "  x1 = " << (-b - sdelta) / (2.0 * a) << endl;
        cout << "  x2 = " << (-b + sdelta) / (2.0 * a) << endl;
    } else {
        cout << "brak rozw w liczbach rzeczywistych" << endl;

        complex<double> imaginal = sqrt(-delta) * 1i / (2.0 * a);
        double real = -b / (2.0 * a);
        cout << "  x1 = " << real + imaginal << endl;
        cout << "  x2 = " << real - imaginal << endl;
    }

}

int main() {

    delta(5,3,32);
    return 0;
}