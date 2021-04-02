// cpp stl Bartlomiej Grochowski 300951
// zadanie 2 - regex daty

#include <iostream>
#include <regex>

using namespace std;

#define SUCCESS { cerr << "-- SUCCESS --" << endl; }
#define FAILURE { cerr << "-- FAILURE --" << endl; }

// VALID DATE
regex date_regex (
  "^("
    // day-month-year normal
      "(" "(31)-(01|03|05|07|08|10|12)"                     // 31 days
        "|" "(29|30)-(01|03|04|05|06|07|08|09|10|11|12)" // 29 & 30 days except 29 Feb
        "|" "(0[1-9]|[1[0-9]|2[0-8])-(0[1-9]|1[012])"    // 1-28 days
      ")" "-(\\d{4,})"                                   // year

    "|" // OR

    // day-month-year leap 29 Feb since 1804 to 2010
    "(" "(29)-(02)-" // 29 Feb
       "(((18|19|20)(04|08|[2468][048]|[13579][26]))|2000)"// leap year
    ")"
  ")$"
);

int main(int argc, const char* argv[]) {

    try {

        if (argc != 2) throw exception();
        string input = argv[1];

        if (regex_match(input,date_regex)) SUCCESS
        else                               FAILURE

    } catch (const exception& e) {
        FAILURE
    }

    return 0;
}
