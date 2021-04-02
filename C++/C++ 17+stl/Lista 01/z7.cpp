// cpp stl Bartlomiej Grochowski 300951
// zadanie 7 - lokalna zmienna w switch-case

#include <iostream> 
using namespace std;

void print_date(int day, int month, int year) {

    cout << day << " "; 
    
    switch (string str; month) {

        case 1: str = "stycznia";      cout << str; break;
        case 2: str = "lutego";        cout << str; break;
        case 3: str = "marca";         cout << str; break;
        case 4: str = "kwietnia";      cout << str; break;
        case 5: str = "maja";          cout << str; break;
        case 6: str = "czerwca";       cout << str; break;
        case 7: str = "lipca";         cout << str; break;
        case 8: str = "siepnia";       cout << str; break;
        case 9: str = "września";      cout << str; break;
        case 10: str = "października"; cout << str; break;
        case 11: str = "listopada";    cout << str; break;
        case 12: str = "grudnia";      cout << str; break;
    }

    cout << " " << year << endl;
}

int main() {

    print_date(10,10,2018);
    return 0;
}