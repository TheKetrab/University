// cpp stl Bartlomiej Grochowski 300951
// zadanie 3 - regex nazwy miasta

#include <iostream>
#include <regex>

using namespace std;

#define SUCCESS { cerr << "-- SUCCESS --" << endl; }
#define FAILURE { cerr << "-- FAILURE --" << endl; }

// UWAGA: nazwę ze spacjami w konsoli podajemy z cudzysłowie

// nazwy składają się ze słów, każde zaczyna się dużą literą
// max 2 słowa mają w sobie łącznik
#define WORD "([A-Z]|Ż|Ź|Ć|Ą|Ś|Ę|Ł|Ó|Ń)([a-z]|ż|ź|ć|ń|ó|ł|ę|ą|ś)*"
#define HYPHEN_WORD WORD "-" WORD

// VALID CITY
// 1. pierwsze słowo jest zwykłe: potem szukaj SPACJA+SŁOWO i max 2 z myślnikiem
// 2. pierwsze słowo jest z myślnikiem: potem szukaj SPACJA+SŁOWO i max 1 z myślnikiem
regex city_regex (
    "^("
        "(" WORD
            "(\\s+" WORD ")*"
            "(\\s+" HYPHEN_WORD ")?"
            "(\\s+" WORD ")*"
            "(\\s+" HYPHEN_WORD ")?"
            "(\\s+" WORD ")*"
        ")"
    "|"
        "(" HYPHEN_WORD
            "(\\s+" WORD ")*"
            "(\\s+" HYPHEN_WORD ")?"
            "(\\s+" WORD ")*"
        ")"
    ")$"
);

int main(int argc, const char* argv[]) {

    try {

        if (argc != 2) throw exception();
        string input = argv[1];
        cout << "Input string: '" << input << "'" << endl;

        if (regex_match(input,city_regex)) SUCCESS
        else                               FAILURE

    } catch (const exception& e) {
        cerr << "Run: ./z3 \" Ala Ma Ko-Ta\"" << endl;
    }

    return 0;
}
