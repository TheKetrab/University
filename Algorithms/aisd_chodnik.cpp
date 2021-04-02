#include <iostream>
#include <cstdint>
#include <cinttypes>
#include <vector>

// j - kolumna, i=l (wiersz)
// pi, pj - wspolrzedne rodzica
// j - najstarszy bit to info czy odwiedzony
typedef struct brick { // 12B
    uint16_t l,m,r;   // dlugosci
    uint16_t j,pi,pj; // miejsce w bricks
} TBRICK;


void printResult(std::vector<TBRICK*> &result);
void printBrickRes(TBRICK brick);
void findResult(std::vector<std::vector<TBRICK*>> &bricks, std::vector<TBRICK*> &result);

inline bool END(TBRICK* brick) {
    return brick->r == 0;
}

// l,m,r nie wiecej niz 16 bit
inline TBRICK* MAKE_BRICK(uint16_t l, uint16_t m, uint16_t r, uint16_t col) {
    return new TBRICK {l,m,r,col,0,0};
}

inline uint16_t GET_J(TBRICK* brick) {
    return brick->j & 0x7FFF;
}

inline uint16_t GET_I(TBRICK* brick) {
    return brick->l;
}

inline void SET_VISITED(TBRICK* brick) {
    brick->j |= (0x1<<15) ;
}

inline bool VISITED(TBRICK* brick) {
    return (brick->j)>>15 & 0x1;
}

inline void SET_PARRENT(TBRICK* brick, uint64_t pi, uint64_t pj) {
    brick->pi = pi;
    brick->pj = pj;
}


int main() {

    uint16_t l,m,r;
    int n; scanf("%d",&n);

    // tablica list kostek, w itej jest taka że l=i
    std::vector<std::vector<TBRICK*>> bricks(10001);

    for (int i=0; i<n; i++) {
        scanf("%" SCNu16 ,&l);
        scanf("%" SCNu16 ,&m);
        scanf("%" SCNu16 ,&r);
        uint16_t col = bricks[l].size();
        bricks[l].push_back(MAKE_BRICK(l,m,r,col));
    }

    std::vector<TBRICK*> result;
    findResult(bricks,result);
    printResult(result);

    return 0;
}


void printResult(std::vector<TBRICK*> &result) {
    
    int s = result.size();
    if (s == 0) { printf("BRAK\n"); return; }

    printf("%d\n",s);
    // wypisz od konca
    for (int i=s-1; i>=0; i--) {
        printBrickRes(*result[i]);
    }
}

void printBrickRes(TBRICK brick) {
    printf("%"PRIu16" %"PRIu16" %"PRIu16"\n",brick.l,brick.m,brick.r);
}

void findResult(std::vector<std::vector<TBRICK*>> &bricks, std::vector<TBRICK*> &result) {

    // stos kolejnych do przetworzenia (DFS)
    std::vector<TBRICK*> pending;
    int n = bricks[0].size();
    for (int i=0; i<n; i++) {
        pending.push_back(bricks[0][i]);
    }

    // dfs az znajdziesz
    while(!pending.empty()) {

        TBRICK* current = pending.back();
        pending.pop_back();
        
        // jesli to brick zakanczajacy chodnik to odtworz rozwiazanie
        if (END(current)) {
            
            // wrzucaj na stos result kolejnych parrentow (cofaj sie do poczatku)
            while(true) {
                result.push_back(current);
                current = bricks[current->pi][current->pj];
                if (GET_I(current) == 0) {
                    result.push_back(current);
                    return; // END !!!
                }
            }
            
        }

        // else dodaj wszystkich nieodwiedzonych nastepnikow do pending jako parrent current
        int next = current->r;
        int sizeNext = bricks[next].size();
        for (int j=0; j<sizeNext; j++) {
            
            if ( !VISITED(bricks[next][j]) ) {
                SET_PARRENT( bricks[next][j] , GET_I(current) , GET_J(current) );
                SET_VISITED( bricks[next][j] );
                pending.push_back(bricks[next][j]);
            }
        }
    }


}
