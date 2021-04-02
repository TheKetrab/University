
// mapujemy marchewki po x na kolumny (zeby nie bylo pustych kolumn)
// potem srotujemy po y i wstawiamy OD DOLU do drzewa maximum
// obserwacja: marchewka z kolumny np 5 moze poprawic kolumny: 1,2,3,4,5
// 1. nie moze poprawic kolumn na prawo (bo krolik nie umie chodzic w lewo)
// 2. nie bedzie konfliktu z marchewkami z kolumn 1...5, ale w wyzszych wierszach,
//    bo idziemy od dolu! (czyli ich jeszcze nie wstawilismy)
// drzewo maximum: w czasie O(logn) zwraca maximum (1...k)

// mtree: drzewo maximum - logarytmicznie znajduje maximum 1...(k-1)
// begin: ostatni index, ktory nie jest lisciem
// array: tablica z maximami, array[begin+1] to pierwszy lisc

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#define MAX(u,v) { u.val > v.val ? u : v }
typedef struct coords { int32_t x,y; } tcoords;
typedef struct carrot { tcoords c; int64_t w; } tcarrot;
typedef struct solution { int64_t val; int owner; } tsol;
int fill_map(std::unordered_map<int,int> &map, std::vector<tcarrot> &carrots, int size);

class mtree {
    int begin; std::vector<tsol> array;
  public:
    mtree(int n);
    void insert(int pos, tsol s);
    tsol maximum(int pos);
    tsol maximum();
    void print();
};


int main() {

    int n, m, k;
    scanf("%d %d %d",&m,&n,&k);

    std::vector<int> prev(k);
    std::vector<tcarrot> carrots(k);
    for (int i=0; i<k; i++) scanf("%d %d %ld",
      &carrots[i].c.x,&carrots[i].c.y,&carrots[i].w);

    // skrajne przypadki
    if (k <= 0) { printf("%d\n%d\n",0,0); return 0; }
    if (k == 1) { printf("%ld\n%d\n%d %d\n",
      carrots[0].w,1,carrots[0].c.x, carrots[0].c.y); return 0; }
    // ----- ----- -----


    std::sort(carrots.begin(),carrots.end(), // sort by X
      [](tcarrot x, tcarrot y) { return x.c.x < y.c.x; });

    std::unordered_map<int,int> map;
    int columns = fill_map(map,carrots,k);

    std::sort(carrots.begin(),carrots.end(), // sort by Y, then X
      [](tcarrot x, tcarrot y) { return x.c.y < y.c.y || (x.c.y == y.c.y && x.c.x < y.c.x); });

    // maximum tree - logn
    mtree t(columns);
    for (int i=0; i<k; i++) {
        int column = map[ carrots[i].c.x ];
        tsol max = t.maximum(column); prev[i] = max.owner;
        t.insert(column , { max.val+carrots[i].w , i });
    }

    // odtwarzanie sciezki
	tsol max = t.maximum();
    std::vector<tcoords> path;
    for (int idx = max.owner; idx != -1; idx = prev[idx])
        path.push_back(carrots[idx].c);

    // print
    int size = path.size();
    printf("%ld\n%d\n",max.val,size);
    for (int i=size-1; i>=0; i--)
        printf("%d %d\n",path[i].x,path[i].y);

    return 0;
}

// tworzy slownik: x->column i zwraca ilosc kolumn (roznych x)
int fill_map(std::unordered_map<int,int> &map, std::vector<tcarrot> &carrots, int size) {

    int col = 0;
    for (int i=0; i<size; i++)
        if (map[ carrots[i].c.x ] == 0)
            map[ carrots[i].c.x ] = ++col;

    return col;
}

// n > 1 !!!
mtree::mtree(int n) {
    int pow2 = 1; while (pow2 < n) pow2<<=1;
    this->begin = pow2 - 1;
    array.resize(2*pow2,{0,-1});
}

// wstawia val do drzewa i aktualizuje maksimum, O(logn)
// nie da sie ZMNIEJSZYC wstawionej juz wartosci
void mtree::insert(int pos, tsol s) {
    
    for (int u = begin+pos; u != 0; u/=2) {
        if (array[u].val < s.val) array[u] = s;
        else break; // jesli mniejsze to nie ma co aktualizowac maximum
    }
}

// zwraca maximum w calym drzewie w czasie stalym
tsol mtree::maximum() { return array[1]; }

// zwraca maksimum z indeksow 1...pos, O(logn)
tsol mtree::maximum(int pos) {

    // polluted - maksimum w tym drzewie pochodzi
    // od kogos o wiekszym indeksie niz pos
    bool v_is_polluted = false;
    
    int u = begin+1, v = begin+pos;
    tsol res = array[v];

    // dopoki to nie sa bracia szukaj po stronie v
    while (u/2 != v/2) {

        if (!v_is_polluted) {
            
            // jesli v to lewy syn i prawy jest wiekszy - wtedy to koniec na tej sciezce,
            // wszystko wyzej pochodzi od tego prawego syna, a prawy nie jest w 1...pos

            if (v%2 == 1) res = MAX(array[v],array[v-1]);
            else if (array[v+1].val > array[v].val) v_is_polluted = true;
            // else res sie nie zmienia
        }

        else { // v_is_polluted
            
            // jesli v to prawy syn, a lewy jest wiekszy, to v nie jest juz polluted,
            // bo maksimum u ojca bedzie pochodzic od kogos kto mial indeks mniejszy niz pos

            if (v%2==1 && array[v-1].val > res.val) {
                res = array[v-1];
                if (array[v-1].val > array[v].val) v_is_polluted = false;

            } // else still polluted 
        }

        u/=2, v/=2;
    }

    // skonfrontuj rozwiazanie z u, ktore ma
    // na pewno maksimum indeksu mniejszego
    res = MAX(res,array[u]);
    return res;
}

/*
void mtree::print() {

    int size = array.size(), k = 1, j = 0;
    for (int i=1; i<size; i++) {
        printf("(%ld|%d) ",array[i].val,array[i].owner); j++;
        if (k == j) { putchar('\n'); j=0; k*=2; }
    }

}
*/
