#include <iostream>
#include <vector>
#include <inttypes.h>

typedef struct coin {
    int32_t p; // <= 10^5 (nominal w groszach)
    int32_t w; // <= 10^5 (waga w gramach)
} tcoin;

typedef struct result {
    int64_t min;         // minimalna wartosc monet o tej wadze
    int64_t max;         // maksymalna wartosc monet o tej wadze
    int16_t min_coin_id; // index monety, ktora poprawila rozwiazanie min
    int16_t max_coin_id; // index monety, ktora poprawila rozwiazanie max
    bool possible;        // INPUTED | POSSIBLE | CHECKED
    // aby obliczyc parrent ID, trzeba odjac warotsc monety, i tak az dojdziemy do 0
} tresult;

const int64_t MININF  = 0x8000000000000000;
const int64_t PLUSINF = 0x7FFFFFFFFFFFFFFF;

void find(std::vector<tresult> &results, std::vector<tcoin> &coins);
void try_improve_min(tresult &res, const tresult &smaller, int val, int coin_id);
void try_improve_max(tresult &res, const tresult &smaller, int val, int coin_id);
void print_coins(std::vector<tresult> &results, int F, std::vector<tcoin> &coins, int32_t C, bool max);

int main() {

    int32_t F; // <= 10^6 (masa monet w pudelku)
    int32_t C; // <= 100  (ilosc roznych monet)

    scanf("%d",&F);
    scanf("%d",&C);

    std::vector<tresult> results(F+1);

    // EMPTY RESULT
    results[0].min = 0;
    results[0].max = 0;
    results[0].min_coin_id = 0;
    results[0].max_coin_id = 0;
    results[0].possible = false;

    for (int i=1; i<=F; i++) {
        results[i].max_coin_id = -1;
        results[i].min_coin_id = -1;
        results[i].min = PLUSINF; // wszystko jest mniejsze niz +inf
        results[i].max = MININF;  // wszystko jest wieksze niz -inf
        results[i].possible = false;
    }

    // SCAN COINS
    std::vector<tcoin> coins(C);
    for (int i=0; i<C; i++) {

        scanf("%d %d",&(coins[i].p),&(coins[i].w));
        if (coins[i].w > F) continue;

        try_improve_min(results[ coins[i].w ],results[0],coins[i].p,i);
        try_improve_max(results[ coins[i].w ],results[0],coins[i].p,i);
        results[ coins[i].w ].possible = true;

    }

    // FIND RESULT
    find(results,coins);

    // PRINT RESULT
    if ( !(results[F]).possible ) printf("NIE\n");
    else {

        printf("TAK\n");
        printf("%ld\n",results[F].min);
        print_coins(results,F,coins,C,0); // min
        printf("\n%ld\n",results[F].max);
        print_coins(results,F,coins,C,1); // max
        printf("\n");
    }

    return 0;
}


// dynamicznie wypelnia tablice z wynikami
void find(std::vector<tresult> &results, std::vector<tcoin> &coins) {

    for (int k=0; k<(int)results.size(); k++) {

        bool found = false;
        tresult res = results[k];
        int n = coins.size();

        // dorzucenie jednej monety do mniejszego rozwiazania
        for (int i=0; i<n; i++) {

            tcoin coin = coins[i];
            int smaller = k-coin.w;

            if (smaller <= 0 || !(results[smaller]).possible)
                continue; // EXIT IF

            else {
                found = true;
                res.possible = true;
                try_improve_min(res,results[smaller],coin.p,i);
                try_improve_max(res,results[smaller],coin.p,i);
            }

        }

        if (found) results[k] = res;
    }

}

void try_improve_min(tresult &res, const tresult &smaller, int val, int coin_id) {
    if (smaller.min + val < res.min) {
        res.min = smaller.min + val;
        res.min_coin_id = coin_id;
    }
}

void try_improve_max(tresult &res, const tresult &smaller, int val, int coin_id) {
    if (smaller.max + val > res.max) {
        res.max = smaller.max + val;
        res.max_coin_id = coin_id;
    }
}

// F - zacznij od results[F], max - wersja max czy min?
void print_coins(std::vector<tresult> &results, int F, std::vector<tcoin> &coins, int32_t C, bool max) {

    int res_id = F;
    tresult temp = results[res_id]; // stad zaczynaj, jedz na dol, az do poczatkowej wartosci
    std::vector<int> coins_cnt(C); // wektor z rozwiazaniami (ilosc danych monet)

    while (res_id > 0) {
        int coin_id = max ? temp.max_coin_id : temp.min_coin_id;
        res_id = res_id - coins[coin_id].w;
        coins_cnt[coin_id]++;
        temp = results[res_id];
    }

    for (int i=0; i<C; i++)
        printf("%d ",coins_cnt[i]);

}
