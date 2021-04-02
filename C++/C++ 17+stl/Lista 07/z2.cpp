// cpp stl Bartlomiej Grochowski 300951
// zadanie 2 - rozkłady i wykresy

#include <iostream>
#include <random>
#include <fstream>
#include <chrono>

#define UNIFORM_N 1000 
#define BINOMIAL_N 1000 
#define NORMAL_N 1000 

using namespace std;

int main() {

    default_random_engine dre;
    dre.seed(chrono::system_clock::now()
        .time_since_epoch().count());
    
    ofstream uniform("uniform.csv");
    ofstream binomial("binomial.csv");
    ofstream normal("normal.csv");  

    auto ud = uniform_real_distribution<double>(0, 100);
    auto bd = binomial_distribution<int>(100,0.5);
    auto nd = normal_distribution<double>(0, 0.2);

    cout << "Generating data..." << endl;
    for (int i = 0; i < UNIFORM_N; i++)
        uniform << ud(dre) << endl;

    for (int i = 0; i < BINOMIAL_N; i++)
        binomial << bd(dre) << endl;

    for (int i = 0; i < NORMAL_N; i++)
        normal << nd(dre) << endl;

    cout << "Running plots..." << endl;
    system("gnuplot -p uniform.gnuplot"); 
    system("gnuplot -p binomial.gnuplot"); 
    system("gnuplot -p normal.gnuplot"); 
    cout << "Done!" << endl;

    return 0;
}