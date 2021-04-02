// cpp stl Bartlomiej Grochowski 300951
// zadanie 4 - enum class : type

#include <iostream> 
using namespace std;

enum class Name : uint16_t {
    Bartek, Ania, Alicja, Zuza, Alek
};

void print_message_to(string msg, Name receiver) {

    cout << msg << " ";
    switch(receiver) {
        case Name::Bartek: cout << "Bartek"; break;
        case Name::Ania:   cout << "Ania";   break;
        case Name::Alicja: cout << "Alicja"; break;
        case Name::Zuza:   cout << "Zuza";   break;
        case Name::Alek:   cout << "Alek";   break;
    }
    cout << endl;
}

int main() {

    print_message_to(string("stop it"),Name::Bartek);

    return 0;
}