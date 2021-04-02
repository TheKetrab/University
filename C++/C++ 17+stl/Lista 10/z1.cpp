// cpp stl Bartlomiej Grochowski 300951
// zadanie 1 - istream_iterator

#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>
#include <iterator>
#include <cmath>
#include <sstream>
#include <algorithm>

using namespace std;

// wczytuje cały input do stringstreama
stringstream read_input() {

    stringstream str_stream;

    string line;
    while (getline(cin,line) && !line.empty())
        str_stream << " " << line;

    return str_stream;
}

int main() {

    stringstream input = read_input();

    // automatycznie dzieli po whitespace
    vector<double> v(istream_iterator<double>(input), {});

    for (auto x : v)
        cout << x << endl;

    int n = v.size();
    double avg = (n == 0) ? 0 
        : accumulate(v.begin(),v.end(),0.0) / n;

    double st_dev = (n == 0) ? 0
        : sqrt(accumulate(v.begin(),v.end(),0.0,
            [avg](double acc, double i) {
                return acc + (i-avg) * (i-avg);
            }) / n);

    cout << setprecision(3) << fixed;
    cout << "Average            = " << avg << endl;
    cout << "Standard deviation = " << st_dev << endl;

    return 0;
}