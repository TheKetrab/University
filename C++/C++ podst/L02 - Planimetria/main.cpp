#include <iostream>

#include "pwp.hpp"
#include "test.hpp"


using namespace std;

// moje testy
// 1. pkt(-5,-2) , pkt(-2,-1) -> pro(-1,3,1)
//

int main()
{
    try {
        wybor_glowny();
    }
    catch(const invalid_argument& ia) {
        cerr<<"Invalid argument: "<<ia.what()<<endl;
        exit(1);
    }

    return 0;
}
