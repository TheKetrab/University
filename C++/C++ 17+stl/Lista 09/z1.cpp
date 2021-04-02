// cpp stl Bartlomiej Grochowski 300951
// zadanie 1 - regex czasu

#include <iostream>
#include <regex>

using namespace std;

#define SUCCESS { cerr << "-- SUCCESS --" << endl; }
#define FAILURE { cerr << "-- FAILURE --" << endl; }

// VALID TIME
regex time_regex (
    "^([0-1][0-9]|2[0-3])" // hh
    ":([0-5][0-9])"        // mm
    "(:([0-5]?[0-9]?))?$"  // s or ss (optional)    
);

int main(int argc, const char* argv[]) {

    try {

        if (argc != 2) throw exception();
        string input = argv[1];

        smatch m;        
        if (regex_match(input,m,time_regex)) {
            
            SUCCESS

            cout << "H: " << m[1] << endl;
            cout << "M: " << m[2] << endl;
            if (m[4].matched)
                cout << "S: " << m[4] << endl;

        } else FAILURE

    } catch (const exception& e) {
        FAILURE
    }

    return 0;
}
