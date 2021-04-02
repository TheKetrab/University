// cpp stl Bartlomiej Grochowski 300951
// zadanie 3 - filesystem: fileinfo

#include <filesystem>
#include <iostream>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

    for (int i=1; i<argc; i++) {

        fs::path p(argv[i]);        
        if (fs::exists(p)) {

            cout << "root_name()       = " << p.root_name() << endl;
		    cout << "root_path()       = " << p.root_path() << endl;
		    cout << "relative_path()   = " << p.relative_path() << endl;
		    cout << "parent_path()     = " << p.parent_path() << endl;
		    cout << "filename()        = " << p.filename() << endl;
		    cout << "stem()            = " << p.stem() << endl;
		    cout << "extension()       = " << p.extension() << endl;
            
            cout << "-----" << endl;
            cout << "filesize()        = " << fs::file_size(p) << " bytes " << endl;
            cout << "canonical()       = " << fs::canonical(p) << endl;

            auto time = fs::last_write_time(p);

            // TAKA KONWERSJA NIE DZIAŁA: g++-10 -std=c++20
            //std::time_t ctime = decltype(ftime)::clock::to_time_t(ftime);

            time_t ctime = chrono::system_clock::to_time_t(
                chrono::time_point_cast<chrono::system_clock::duration>(
                    time - fs::file_time_type::clock::now() + chrono::system_clock::now()));

            cout << "last_write_time() = " << asctime(localtime(&ctime)) << endl;

            cout << " ----- ----- ----- " << endl;

        } else cout << "'" << argv[i] << "' doesn't exist." << endl; 

    }

    return 0;

}
