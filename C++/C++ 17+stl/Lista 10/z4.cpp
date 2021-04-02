// cpp stl Bartlomiej Grochowski 300951
// zadanie 4 - filesystem: dir_iterator

#include <filesystem>
#include <iostream>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

#define error(msg) { \
    cerr << msg << endl; return EXIT_FAILURE; \
}

int main(int argc, char* argv[]) {

    if (argc < 2)
        error("Run with: ./z4 DIR_PATH (eg. \"./z4 ..\")")

    fs::path p(argv[1]);

    if (!fs::exists(p))
        error(string(argv[1]) + " does not exists.")

    else if (!fs::is_directory(p))
        error(string(argv[1]) + " is not a directory.")

    else // OK
        for (auto& entry : fs::directory_iterator(p))
            cout << entry.path() << endl;

    return EXIT_SUCCESS;
}