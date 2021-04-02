// cpp stl Bartlomiej Grochowski 300951
// zadanie 3 - ratio template do liczb harmonicznych

#include <iostream>
#include <ratio>
#define N 46 // N = 47 juz mamy blad przy kompilacji

using namespace std;

template<int n>
struct harm : ratio_add< harm<n-1>, ratio<1,n> > {};

template<>
struct harm<1> : ratio<1,1> {};

#define PRINT_HARM(I) { \
    cout << "H" << I << " = " << harm<I>::num << "/" << harm<I>::den << endl; \
}

int main() {

    // Hn = 1 + 1/2 + 1/3 + ... + 1/n
    // ratio dziala na statycznej kompilacji, na template'ach
    // print harm to makro, zeby dzialalo na statycznej kompilacji

    PRINT_HARM(1);
    PRINT_HARM(2);
    PRINT_HARM(3);
    PRINT_HARM(4);
    PRINT_HARM(5);
    PRINT_HARM(6);
    PRINT_HARM(7);
    PRINT_HARM(8);
    PRINT_HARM(9);
    PRINT_HARM(10);
    PRINT_HARM(N);

    return 0;
}

