// cpp stl Bartlomiej Grochowski 300951
// zadanie 5 - nta liczba Lucasa

#include <iostream> 
using namespace std;

auto lucas(uint32_t n) {

    if (n == 0) return 2;
    if (n == 1) return 1;
    return lucas(n-1) + lucas(n-2);
}

auto lucas_tail(uint32_t n, uint64_t prev = 2, uint64_t res = 1) {

    //cout << "lucas_tail("<<n<<","<<prev<<","<<res<<")"<<endl;
    if (n == 0) return prev;
    if (n == 1) return res;
    return lucas_tail(n-1, res, prev+res);

}

int main() {

    //cout << endl << "LUCAS:" << endl;
    //for (int i=0; i<50; i++) cout << "lucas " << i << " is: " << lucas(i) << endl;

    cout << endl << "LUCAS TAIL:" << endl;
    for (int i=0; i<=50; i++) cout << "lucas " << i << " is: " << lucas_tail(i) << endl;


    return 0;

}