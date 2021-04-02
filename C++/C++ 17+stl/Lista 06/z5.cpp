// cpp stl Bartlomiej Grochowski 300951
// zadanie 5 - permutacje

#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "run program with: ./z5 some_string" << endl;
        return 1;
    }

    string str = argv[1];
    sort(str.begin(),str.end());

    do {
        cout << str << endl;
    } while (next_permutation(str.begin(),str.end()));

    return 0;
}