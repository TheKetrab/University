// cpp stl Bartlomiej Grochowski 300951
// zadanie 1 - losowa permutacja

#include <iostream>
#include <random>
#include <vector>
#include <chrono>

using namespace std;

static default_random_engine dre;

template <typename T>
void random_perm(vector<T>& v) {
    
    int n = v.size();
    for (int i = 0; i < n; i++) {
        // postaw jakiś element (j) na miejscu i
        int j = uniform_int_distribution<int>{i, n-1}(dre);
        swap(v[i], v[j]);
    }
}

int main() {

    dre.seed(chrono::system_clock::now()
        .time_since_epoch().count());

    vector<int> vec = {0,1,2,3,4,5,6,7,8,9};
    random_perm(vec);

    for (auto x : vec)
        cout << x << " ";

    cout << endl;

    return 0;
}