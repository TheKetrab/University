// cpp stl Bartlomiej Grochowski 300951
// zadanie 5 - filesystem: rec_dir_iterator

#include <filesystem>
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;
namespace fs = std::filesystem;

#define error(msg) { \
    cerr << msg << endl; return EXIT_FAILURE; \
}

int main(int argc, char* argv[]) {

    if (argc < 2)
        error("Run with: ./z5 DIR_PATH (eg. \"./z5 ..\")")

    fs::path p(argv[1]);

    if (!fs::exists(p))
        error(string(argv[1]) + " does not exists.")

    else if (!fs::is_directory(p))
        error(string(argv[1]) + " is not a directory.")

    // ----- COUNT -----
    uint64_t size = 0;
    for (auto& entry : fs::recursive_directory_iterator(p))
        if (fs::is_regular_file(entry.path()))
            size += fs::file_size(entry.path());

    // ewentualnie:
    // #include <numeric>
    // beg = recursive_directory_iterator(p)
    // end = recursive_directory_iterator{};
    // f = [](uint64_t acc, const fs::directory_entry &e)
    //  { return acc + directory_size(entry.path()); };
    // size = accumulate(beg,end,0,f)

    double kb = (double)size / 1024.0;
    double mb = kb / 1024.0;
    double gb = mb / 1024.0;
    cout << setprecision(2) << fixed;

    cout << fs::canonical(p) << " -----> " << size << " B";
    if (gb > 1) cout << " = " << gb << " GB";
    else if (mb > 1) cout << " = " << mb << " MB";
    else if (kb > 1) cout << " = " << gb << " KB";

    cout << endl;

    return EXIT_SUCCESS;
}