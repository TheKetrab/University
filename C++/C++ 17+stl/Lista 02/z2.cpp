// cpp stl Bartlomiej Grochowski 300951
// zadanie 2 - shared ptr

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <ctime>

using namespace std;
#define N 10 // ammount of ptrs

class line_writer {

    ofstream* file;

public:
    line_writer(const string& path) {
        file = new ofstream(path, ofstream::out);
        if (!file->is_open()) {
            throw ofstream::failure("Impossible to open file: " + path);
        } else {
            cerr << "Line writer created." << endl;
        }
    }
    ~line_writer() {
        file->close();
        delete file;
        cerr << "Line writer deleted." << endl;

    }
    void write(string& msg) {
        (*file) << msg;
    }

};

int main() {

    srand(time(nullptr));

    vector<shared_ptr<line_writer>> ptrs;
    {
        // w bloku, zeby zmienna p zostala usunieta
        // i obiekt p zostal usuniety i shared pointerow bylo N
        auto p = make_shared<line_writer>("file");
        for (int i=0; i<N; i++) ptrs.push_back(p);
    }

    while (ptrs.size() > 0) {

        cout << "Count: " << ptrs[0].use_count() << endl;

        int idx = rand() % ptrs.size();
        string msg = "Info from " + to_string(idx) + '\n';
        ptrs[idx]->write(msg);

        if (ptrs.size() == 1) {
            cout << "Bedzie usuwany ostatni wskaznik..." << endl;
        }

        ptrs.pop_back(); // delete last
        
    }

    cout << "W tym miejscu nie ma zadnego shared ptr." << endl;

    return 0;
}