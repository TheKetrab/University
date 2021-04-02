// cpp stl Bartlomiej Grochowski 300951
// zadanie 4 - analiza slownikowa

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

#define error(msg) \
    { cerr << msg << endl; return EXIT_FAILURE; }

map<char,float> analyze(const string &str) {

    int letters_total = 0;
    map<char,float> counts;

    for (char c : str)
        if (c >= 'a' && c <= 'z') { // a-z
            letters_total++;
            if (counts.find(c) != counts.end())
                counts[c]++; else counts[c] = 1;
        }

    for_each(counts.begin(), counts.end(),
        [letters_total](pair<const char, float> &p)
            { p.second /= letters_total; });

    return counts;
}

int main(int argc, char* argv[]) {

    if (argc != 2)
        error("run program with: ./z4 PATH");

    ifstream f(argv[1]);
    if (!f.is_open())
        error("unable to open file");

    stringstream ss; ss << f.rdbuf(); string str = ss.str();
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    auto res = analyze(str);
    vector<pair<char,float>> v1, v2;
    for (auto p : res) {
        v1.push_back(p);
        v2.push_back(p);
    }

    sort(v1.begin(),v1.end(), [](auto p1, auto p2)
        { return p1.first < p2.first; }); // alfabetycznie
    sort(v2.begin(),v2.end(), [](auto p1, auto p2)
        { return p1.second > p2.second; }); // czestotliwosc

    // PRINT
    cout.precision(2);

    cout << "CZĘSTOTLIWOŚĆ:" << endl;
    for (int i=0; i < (int)v2.size(); i++) {
        cout << i+1 << ".\t" << v2[i].first << " ";
        cout << v2[i].second * 100 << "%" << endl;
    }

    cout << "ALFABETYCZNIE:" << endl;
    for (int i=0; i < (int)v1.size(); i++) {
        cout << i+1 << ".\t" << v1[i].first << " ";
        cout << v1[i].second * 100 << "%" << endl;
    }

    return EXIT_SUCCESS;
}