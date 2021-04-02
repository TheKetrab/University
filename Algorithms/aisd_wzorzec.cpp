#include <iostream>
#include <vector>
#include <string>

// ALGORYTM KARPA-RABINA

// a - wysokosc wzorca
// b - szerokosc wzorca
// c - wysokosc tabeli
// d - szerokosc tabeli

using namespace std;
#define FIRST 'A' // pierwsza litera alfabetu
#define Q 251 // liczba pierwsza, cialo Z_251
#define MAP(c) (c - FIRST + 1) // mapuje litere alfabetu na liczbe (A=1, B=2, ...)

int A, B, C, D, HASHLINE_SIZE; // dimensions
bool pattern_probably_ok(vector<int16_t> &hp, vector<vector<int16_t>> &ha, int row, int col);
int pattern_ok(vector<string> &model, vector<string> &array, int row, int col);
int16_t quick_pow_mod(int16_t a, int16_t b, int16_t q);
vector<int16_t> hash_pattern(vector<string> &pattern);
vector<int16_t> hash_line(const string &line);
int16_t save_mod(int dividend, int divisor);

int main() {

    // dimensions
    scanf("%d %d %d %d\n",&A,&B,&C,&D);
    HASHLINE_SIZE = D-B+1;

    // jesli wzorzec jest za duzy, to nie ma sensu sprawdzac
    if (A > C || B > D) { printf("0\n"); return 0; }

    // pattern
    vector<string> pattern(A);
    for (int i=0; i<A; i++) cin >> pattern[i];
    vector<int16_t> hp = hash_pattern(pattern);

    // array
    vector<string> array(C);
    vector<vector<int16_t>> ha(C);
    for (int i=0; i<C; i++) {
        cin >> array[i];
        ha[i] = hash_line(array[i]);
    }

    int result = 0;
    for (int i=0; i<C-A+1; i++) {
        for (int j=0; j<HASHLINE_SIZE; j++) {
            if (pattern_probably_ok(hp,ha,i,j))
                result += pattern_ok(pattern,array,i,j);
            if (result >= 200) goto finish;
        }
    }

finish:
    printf("%d\n",result);
    return 0;
}

// [row,col] - startowa pozycja, zwraca 1 jesli wzorzec pasuje (alg naiwny)
int pattern_ok(vector<string> &pattern, vector<string> &array, int row, int col) {

    for (int i=row; i<row+A; i++) {
        if (i == C) return 0;
        for (int j=col; j<col+B; j++) {
            if (j == D) return 0;
            if (array[i][j] != pattern[i-row][j-col])
                return 0;
        }
    }

    return 1;

}

// sprawdza, czy hashe sie zgadzaja i czy jest sens sprawdzac dokladnie
bool pattern_probably_ok(vector<int16_t> &hp, vector<vector<int16_t>> &ha, int row, int col) {

    // przejedz po kolumnach i sprawdz czy pasuje
    for (int i=0; i<A; i++) {
        if (row+i >= C || col >= D) return false;
        if (hp[i] != ha[row+i][col]) return false;
    }

    return true;
}

// pattern
// AAA        pattern
// AAB  --> (156,54,42)
// BCA
vector<int16_t> hash_pattern(vector<string> &pattern) {

    vector<int16_t> result(A);

    // zewnetrzna petla po wierszach
    for (int i=0; i<A; i++) {

        // petla po literach
        int32_t hash = 0;
        for (int j=0; j<B; j++)
            hash = (hash*10 + MAP(pattern[i][j])) % Q;

        result[i] = (int16_t)hash;
    }

    return result;

}

// a^b mod q
// 10^39 = 10^1 * 10^2 * 10^4 * 10^32
int16_t quick_pow_mod(int16_t a, int16_t b, int16_t q) {
    
    int16_t result = 1;
    while (b > 0) {
        if (b%2 == 1) result = (result*a) % q;
        a = (a*a) % q;
        b /= 2;
    } return result;
}

// zamienia stringa na vector z hashami
vector<int16_t> hash_line(const string &line) {

    vector<int16_t> result(HASHLINE_SIZE);

    int32_t hash = 0;
    for (int i=0; i<B; i++)
        hash = (hash*10 + MAP(line[i])) % Q;

    result[0] = (int16_t)hash;

    for (int s=1; s<HASHLINE_SIZE; s++) {
        hash = save_mod((hash - ((MAP(line[s-1]) * quick_pow_mod(10,B-1,Q)) %Q)), Q);
        hash = (hash*10 + MAP(line[s+B-1])) % Q;
        result[s] = (int16_t)hash;
    }

    return result;

}

// modulo w ciele Zp, dla ujemnych liczb zwraca dodatnia reszte
int16_t save_mod(int dividend, int divisor) {
    int remainder = dividend % divisor;
    return remainder < 0 ? remainder + divisor : remainder;
}
