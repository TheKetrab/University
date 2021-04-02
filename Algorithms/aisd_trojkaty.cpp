#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>

typedef struct point {
    double x, y;
} tpoint;

typedef struct solution {
    tpoint a,b,c;
    double len;
} tsolution;

typedef std::vector<tpoint> array;
double inf = std::numeric_limits<double>::infinity();
tsolution empty_solution = {{0,0},{0,0},{0,0},inf};

bool cmp_x (tpoint a, tpoint b) { return a.x < b.x; }
bool cmp_y (tpoint a, tpoint b) { return a.y < b.y; }

inline double square(double x) {
    return x*x;
};

inline double d(const tpoint a, const tpoint b) {
    return sqrt( square(b.x-a.x) + square(b.y-a.y) );
}
inline double len(const tpoint a, const tpoint b, const tpoint c) {
    return d(a,b) + d(b,c) + d(c,a);
}

void splitY(const array &Y, array &leftY, array &rightY, double line);
array clear_to_area(const array &Y, const double line, const double d);
tsolution find(const array &X, const array &Y, int l, int r);

int main() {

    int n, xi, yi;
    scanf("%d",&n);
    array X(n), Y(n);

    for (int i=0; i<n; i++) {
        scanf("%d %d",&xi, &yi);
        tpoint p = {(double)xi,(double)yi};
        X[i] = Y[i] = p;
    }

    std::sort (X.begin(), X.end(), cmp_x); // sortuj po x
    std::sort (Y.begin(), Y.end(), cmp_y); // sortuj po y
    
    tsolution res = find(X,Y,0,X.size());
    printf("%d %d\n",(int)res.a.x,(int)res.a.y);
    printf("%d %d\n",(int)res.b.x,(int)res.b.y);
    printf("%d %d\n",(int)res.c.x,(int)res.c.y);

    return 0;
}

// zachowuje posortowanie po Y, wywala te, ktore sa dalej od L niz odleglosc d
// Y - sorted by yi, line - division line (x cord), d - max distance
array clear_to_area(const array &Y, const double line, const double d) {

    array Yprim;
    for (tpoint p : Y)
        if (fabs(p.x - line) < d)
            Yprim.push_back(p);

    return Yprim;
}

// rozdziela punkty z Y na te, ktore maja isc do lewego wywolania rekurencyjnego i do prawego
void splitY(const array &Y, array &leftY, array &rightY, double line) {
    for (tpoint p : Y)
        if (p.x < line) leftY.push_back(p);
        else rightY.push_back(p);
}

// l - pierwszy z lewej, r - pierwszy poza
tsolution find(const array &X, const array &Y, int l, int r) {

    if (r-l < 3) return empty_solution;

    int m = (r+l)/2;
    double line = X[m].x;

    array leftY, rightY;
    splitY(Y,leftY,rightY,line);

    tsolution left = find(X,leftY,l,m);
    tsolution right = find(X,rightY,m,r);
    tsolution bestsol = (left.len < right.len) ? left : right;

    // sens jest szukac tylko w pasie odleglym od L o len/2,
    // bo obwod to jakby 'tam i z powrotem' czyli 2*len/2 = len
    double max_dist = bestsol.len/2.0;
    array Yprim = clear_to_area(Y,line,max_dist);

    // * ta zagniezdzona petla jest liniowa! dla kazdego zrobi okolo 10-16 krokow
    // * mozna to dokladnie policzyc z zasady szufladkowej
    // * szukamy dopoki odleglosc miedzy dwoma punkami jest mniejsza niz max_dist
    // * jesli wieksza, to bez sensu, bo obw bedzie min. 2*max_dist, czyli nie poprawi
    int size = Yprim.size();
    for (int i=0; i<size; i++) {
        for (int j=i+1; j<size && d(Yprim[i],Yprim[j])<max_dist; j++) {
            for (int k=j+1; k<size && d(Yprim[i],Yprim[k])<max_dist; k++) {

                tsolution newsol = {
                    Yprim[i],Yprim[j],Yprim[k],
                    len(Yprim[i],Yprim[j],Yprim[k])
                };

                /*
                printf(
                    "bestsol[(%d,%d)(%d,%d),(%d,%d)] = %.2lf \t inside: (%d,%d,%d) newsol = %.2lf\n",
                    (int)bestsol.a.x,(int)bestsol.a.y, (int)bestsol.b.x,(int)bestsol.b.y,
                    (int)bestsol.c.x,(int)bestsol.c.y, bestsol.len,i,j,k,newsol.len
                );
                */

                bestsol = (newsol.len < bestsol.len) ? newsol : bestsol;
                max_dist = bestsol.len / 2.0;
            }
        }
    }

    return bestsol;
}
