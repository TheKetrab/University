
// cpp stl Bartlomiej Grochowski 300951
// zadanie 1 - trigraphs
// trzeba dolaczyc flagi do kompilatora: (przestarzale od cpp17)
// -trigraphs     : zeby moc ich uzywac
// -Wno-trigraphs : zeby nie bylo warningow

// 1. ??= # 
// 2. ??/ \ (backslash)
// 3. ??’ ^ 
// 4. ??( [ 
// 5. ??) ] 
// 6. ??! | 
// 7. ??< { 
// 8. ??> } 
// 9. ??- ~ 

#include <iostream>
using namespace std;

??=define TRIGRAPH "example of trigraph: ??!"

int main() {

    cout << TRIGRAPH << endl;
    return 0;
}