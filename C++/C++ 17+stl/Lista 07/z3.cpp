// cpp stl Bartlomiej Grochowski 300951
// zadanie 3 - własny rozkład

#include <iostream>
#include <vector>
#include <random>
#include <fstream>

using namespace std;

#define err(msg) \
    { cerr << msg << endl; return EXIT_SUCCESS; }

vector<pair<char,double>> english_letters = {
    {'e', 12.02}, {'t', 9.10}, {'a', 8.12}, {'o', 7.68},
    {'i', 7.31}, {'n', 6.95}, {'s', 6.28}, {'r', 6.02},
    {'h', 5.92}, {'d', 4.32}, {'l', 3.98}, {'u', 2.88},
    {'c', 2.71}, {'m', 2.61}, {'f', 2.30}, {'y', 2.11},
    {'w', 2.09}, {'g', 2.03}, {'p', 1.82}, {'b', 1.49},
    {'v', 1.11}, {'k', 0.69}, {'x', 0.17 }, {'q', 0.11},
    {'j', 0.10}, {'z', 0.07}
};

template <typename ResType>
class custom_distribution {

    vector<pair<ResType,double>> distribution;
    uniform_real_distribution<double> urd {0,100};

public:
    custom_distribution(vector<pair<ResType,double>> &v) {
        
        // check sum up to 100%
        double sum = 0; for (auto x : v) sum += x.second;
        if (sum < 99 || sum > 101) throw invalid_argument(
            "Distribution does not sum up to 100%");
        // ----- ----- -----

        distribution = vector<pair<ResType,double>>(v);
    }

    template <class Generator>
    ResType operator()(Generator &g) {
        return get_section( urd(g) );
    }

private:
    ResType get_section(double r) {

        double a = 0; // section begin
        for (auto x : distribution)
            if ((a += x.second) > r)
                return x.first;
        
        // return last
        return distribution[distribution.size()-1].first;
    }
};

int main(int argc, const char* argv[]) {

    if (argc != 3)
        err("Run program with: ./z3 size file.txt");

    ofstream result( argv[2] );
    default_random_engine dre;
    auto cd = custom_distribution<char>(english_letters);
    auto bd = binomial_distribution<int>(11,0.5);
    
    int len = atoi( argv[1] ), i = 0;
    while (i < len) {

        int n = bd(dre) + 1; // [1,12]
        for (int j=0; j<n; j++,i++) {

            if (i >= len) break;

            char letter = cd(dre);
            result << letter;
        }

        if (i >= len) break;
        result << ' '; i++;
    }

    result.close();

    return EXIT_SUCCESS;
}