// cpp stl Bartlomiej Grochowski 300951
// zadanie 3 - najczęstszy element

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

template <typename T>
vector<pair<T,int>> most_common_element(const vector<T> &v) {

    // zliczanie
    map<T,int> counts;
    for (T x : v)
        if (counts.find(x) != counts.end())
            counts[x]++; else counts[x] = 1;

    // najwieksza liczba wystapien
    auto it = max_element(counts.begin(),counts.end(),
        [](const pair<T,int> p1, const pair<T,int> p2)
            { return p1.second < p2.second; });

    // filtrowanie
    vector<pair<T,int>> res;
    for (pair<T,int> p : counts)
        if (p.second == it->second)
            res.push_back(p);

    return res;
}

int main() {

    vector<int> data = { 1, 1, 3, 5, 8, 9, 5, 8, 8, 5 };
    auto res = most_common_element(data);

    for (auto p : res)
        cout << "{" << p.first << "," << p.second << "}" << endl;

    return 0;
}