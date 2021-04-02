// cpp stl Bartlomiej Grochowski 300951
// zadanie 2 - raw string

#include <iostream> 
using namespace std;

int main() {

    cout << R"(Instytut informatyki Uniwersytetu Wrocławskiego
Fryderyka Joliot-Curie 15
50-383 Wrocław)" << endl;
     
    cout << R"(C:\Program Files\)" << endl;

    // --> znaki miedzy '"' a '(' musza tez zamykac " )
    // w tym przypadku xyz
    cout << R"xyz(" () ) \ ~ " )" ! )xyz" << endl;

    return 0;
}
