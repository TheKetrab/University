#include <iostream>
#include <vector>
#include <cinttypes>


using namespace std;

int find(vector<uint64_t> &vec, int n, uint64_t x);

int main() {

    uint64_t input;

    // N
    int n; scanf("%d",&n);
    vector<uint64_t> vec(n);
    for (int i=0; i<n; i++) {
        scanf("%"SCNu64,&input);
        vec[i] = input;
    }

    // M
    int m; scanf("%d",&m);
    for (int i=0; i<m; i++) {
        scanf("%"SCNu64,&input);
        int res = find(vec,n,input);
        printf("%d ",res);
    }

    return 0;
}

int find(vector<uint64_t> &vec, int n, uint64_t x) {

    if (n == 0) return 0;
    if (x < vec[0]) return n;
    if (x > vec[n-1]) return 0;
    
    // r - pierwszy poza
    // l - poczatkowy
    // r-l - ilosc liczb
    int l = 0, r = n, m = (m%2) ? (l+r)/2 : (l+r)/2 + 1;
    int res; // taki index, vec[i] >= x i jest minimalnym indexem

    while (true) {

        // 1 element - rozwazamy 3 przypadki, gdzie to trafi
        if (r-l == 1) {
            if (x > vec[l]) res=r;
            else res=l;
            break;
        }

        // 2 elementy - rozwazamy 5 przypadkow, gdzie to trafi
        else if (r-l == 2) {
            if (x > vec[l+1]) res=r;
            else if ( x<=vec[l+1] && x>vec[l] ) res=l+1;
            else res = l;
            break;
        }

        // > 2 elementy
        else {
            if (vec[m] == x) { res = m; break; }
            else if (x > vec[m]) l=m;
            else r=m;

            m = (m%2) ? (l+r)/2 : (l+r)/2 + 1;
        }

    }

    while ( res>0 && vec[res-1] >= x ) res--; // dosuwanie w lewo najbardziej jak się da

    return n-res;
}
