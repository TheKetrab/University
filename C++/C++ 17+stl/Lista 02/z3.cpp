// cpp stl Bartlomiej Grochowski 300951
// zadanie 3 - weak ptr

#include <iostream>
#include <memory>

using namespace std;

void test(weak_ptr<string> &wp) {

    cout << "wp use_count: " << wp.use_count() << " --> ";
    shared_ptr<string> sp = wp.lock();

    if (sp != nullptr) {
        cout << *sp << endl;
    } else {
        cout << "weak pointer expired" << endl;
    }

}

int main() {

    weak_ptr<string> wp;
    {
        auto sp = std::make_shared<string>("cpp_stl");
    	wp = sp; test(wp);
    }
 
    // tutaj shared ptr juz nie zyje, wiec wp rowniez
    test(wp);

    return 0;
}