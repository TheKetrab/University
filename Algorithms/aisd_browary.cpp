// zad A - Browary
// Bartlomiej Grochowski
// 300 951

#include <iostream>
#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdint>
#include <cinttypes>

using namespace std;

// zwraca dystans miedzy punktami
uint64_t get_dist(int i, int j, uint64_t total, vector<uint64_t> &l) {
	
	int bigger_index, smaller_index;

	if (i>j) {
		bigger_index = i;
		smaller_index = j;
	} else {
		bigger_index = j;
		smaller_index = i;
	}

	uint64_t d1 = l[bigger_index] - l[smaller_index];
	uint64_t d2 = total - d1;
	
	return min(d1,d2);
}


int main() {

    int n; scanf("%d",&n);
    vector<uint64_t> l;
	l.reserve(n);

    uint64_t total = 0;

    for (int i=0; i<n; i++) {
        int x; scanf("%d",&x);
        l.push_back(total);
        total += x;
    }

    // dwa browary, dwa wskazniki: j=0, i=1
    int j=0;
    uint64_t result=0;
    uint64_t current_dist, next_dist;
    
    for (int i=1; i!=0; ) {

        if (j==i) break;

        current_dist = get_dist(i,j,total,l);
        next_dist = get_dist((i+1)%n,j,total,l);

        result = max(result,current_dist);

        if (next_dist < current_dist) {
            j = (j+1)%n;
        }

        else {
            i = (i+1)%n;
        }
    }

    printf("%" PRIu64 "\n", result);

    return 0;
}
