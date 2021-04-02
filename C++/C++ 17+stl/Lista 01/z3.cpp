// cpp stl Bartlomiej Grochowski 300951
// zadanie 3 - using zamiast typedef, auto, foreach

#include <iostream> 
#include <set>
using namespace std;

int main() {

    using myset = std::set<string>; // alias (niegdys typedef)

    myset S { "abc", "xyz", "qux", "uniwersytet", "bgr" };

    for (auto s : S)
        cout << s << endl;

    return 0;

}