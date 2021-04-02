#include <iostream>
#include <vector>
#include <cinttypes>

typedef std::pair<uint64_t,uint64_t> f_matrix;

struct matrix {
    uint64_t m00, m01;
    uint64_t m10, m11;
};

int fibmod(int n, int m);
inline f_matrix multmod(struct matrix mat, f_matrix fibs, int m);
inline struct matrix multmod(struct matrix m1, struct matrix m2, int m);
inline struct matrix addmod(struct matrix m1, struct matrix m2, int m);
struct matrix powmod(struct matrix mat, int n, int m);
char* print_mat(struct matrix mat);

int main() {

    int t, n, m;

    // T
    scanf("%d",&t);
    for (int i=0; i<t; i++) {
        scanf("%d %d",&n,&m);
        printf("%d\n",fibmod(n,m));
    }

/*
    // TEST
    int n = 3; int m = 10;
    struct matrix m1 = {18,62,54,11};
    struct matrix res = powmod(m1,n,m);
    printf("%s ^ %d = %s",print_mat(m1),n,print_mat(res));
*/
    return 0;
}

inline struct matrix multmod(struct matrix m1, struct matrix m2, int m) {
    return {
        (m1.m00*m2.m00 + m1.m01*m2.m10) % m, // m00
        (m1.m00*m2.m01 + m1.m01*m2.m11) % m, // m01
        (m1.m10*m2.m00 + m1.m11*m2.m10) % m, // m10
        (m1.m10*m2.m01 + m1.m11*m2.m11) % m, // m11
    };
}

inline struct matrix addmod(struct matrix m1, struct matrix m2, int m) {
    return {
        (m1.m00 + m2.m00) % m, // m00
        (m1.m01 + m2.m01) % m, // m01
        (m1.m10 + m2.m10) % m, // m10
        (m1.m11 + m2.m11) % m  // m11
    };
}

struct matrix powmod(struct matrix mat, int n, int m) {

    struct matrix res = {1,0,0,1}; // identity

    for (int i=n; i>0; i/=2) {

        if (i%2 == 1) // odd
            // mult M to res from LEFT side
            res = multmod(mat,res,m);

        mat = multmod(mat,mat,m); // m = m*m
    }

    return res;
}

inline f_matrix multmod(struct matrix mat, f_matrix fibs, int m) {
    return f_matrix(
        (mat.m00*fibs.first+mat.m01*fibs.second) % m, // first
        (mat.m10*fibs.first+mat.m11*fibs.second) % m  // second
    );
}

int fibmod(int n, int m) {
    
    // M * [f0,f1] = [f1,f2]
    // M^2 * [f0,f1] = [f2,f3]
    // M^(n-1) * [f0,f1] = [f(n-1),f(n)]
    // M^(n-1) -> fast powering

    f_matrix fibs(0,1);
    struct matrix mat = {0,1,1,1}; // M
    mat = powmod(mat,n-1,m);      // M^n
    fibs = multmod(mat,fibs,m);  // M^(n-1) * [f0,f1]

    return fibs.second;
}

char* print_mat(struct matrix mat) {
    char* str = new char[50];
    sprintf(str,"[%d,%d,%d,%d]",mat.m00,mat.m01,mat.m10,mat.m11);
    return str;
}
