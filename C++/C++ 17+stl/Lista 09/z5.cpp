// cpp stl Bartlomiej Grochowski 300951
// zadanie 5 - Ctrl+F -> wybierz wszystkie linki

#include <iostream>
#include <regex>
#include <fstream>

using namespace std;

#define error(msg) \
    { cerr << msg << endl; return EXIT_FAILURE; }

// (?: ... ) - non-capturing group
// *? - lazy, bierze tak mało jak się tylko da

string sreg = R"(<a\s+(?:[^>]*?\s+)?href=(["'])(.*?)(["']))";
regex ahref_regex( sreg );

int main(int argc, const char* argv[]) {

    if (argc != 2)
        error("run program with: ./z5 PATH");

    ifstream f(argv[1]);
    if (!f.is_open())
        error("unable to open file");

    stringstream ss;
    ss << f.rdbuf();
    f.close();
    string str = ss.str();

    for (auto it = sregex_iterator(str.cbegin(), str.cend(), ahref_regex);
        it != sregex_iterator(); it++) cout << it->str(2) << endl;

    return EXIT_SUCCESS;
}