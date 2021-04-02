// cpp stl Bartlomiej Grochowski 300951
// zadanie 2 - info o zmiennoprzecinkowych

#include <iostream>
#include <limits>
#include <cmath>

using namespace std;

int main() {

    cout << "Closest to 0:" << endl;
    cout << "float ...... -> " << numeric_limits<float>::min() << endl;
    cout << "double ..... -> " << numeric_limits<double>::min() << endl;
    cout << endl;

    cout << "Max:" << endl;
    cout << "float ...... -> " << numeric_limits<float>::max() << endl;
    cout << "double ..... -> " << numeric_limits<double>::max() << endl;
    cout << endl;

    cout << "Diff between 1 and another:" << endl;
    cout << "float ...... -> " << numeric_limits<float>::epsilon() << endl;
    cout << "double ..... -> " << numeric_limits<double>::epsilon() << endl;

    return 0;
}
