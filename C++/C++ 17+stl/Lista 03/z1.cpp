// cpp stl Bartlomiej Grochowski 300951
// zadanie 1 - limity

#include <iostream>
#include <limits>
#include <cmath>

using namespace std;

int main() {

    auto max = numeric_limits<long long int>::max();
    auto min = numeric_limits<long long int>::min();
    cout << "Max: " << max << endl;
    cout << "Min: " << min << endl;
    cout << "Bits: " << sizeof(long long int) * 8 << endl;
    cout << "Num of digits: " << floor(log10(max)) + 1 << endl;

    return 0;
}
