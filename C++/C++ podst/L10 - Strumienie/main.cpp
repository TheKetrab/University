#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstring>
#include <streambuf>
#include <cassert>
#include "strumienie.hpp"

using namespace std;
using namespace strumienie;

int main(int argc, char* argv[])
{

    if(argc > 1)
        func_zad_5(argv[1],argv[2]);

    else {

        func_zad_1();
        func_zad_2();
        func_zad_3("test.txt");
    }


    return 0;
}
