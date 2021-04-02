// cpp stl Bartlomiej Grochowski 300951
// zadanie 4 - regex liczby zespolonej

#include <iostream>
#include <regex>

using namespace std;

#define SUCCESS { cerr << "-- SUCCESS --" << endl; }
#define FAILURE { cerr << "-- FAILURE --" << endl; }

// VALID COMPLEX
regex complex_regex (
    "^"
    "\\s*\\(\\s*"
    "[0-9]+(\\.[0-9]+)?"
    "\\s*\\+\\s*"
    "[0-9]+(\\.[0-9]+)?"
    "[Ii]"
    "\\s*\\)\\s*"
    "$"
);

int main(int argc, const char* argv[]) {

    try {

        if (argc != 2) throw exception();
        string input = argv[1];

        if (regex_match(input,complex_regex)) SUCCESS
        else                                  FAILURE

    } catch (const exception& e) {
         cerr << "Run: ./z4 \"(3+2.5i)\"" << endl;
    }

    return 0;
}
