#include <iostream>
#include <vector>
#include <inttypes.h>
#include <cmath>

using namespace std;

int median_force(int start, int end, vector<int64_t> &numbers);
void insert_sort(int start, int end, vector<int64_t> &numbers);
int selection(int k, int start, int end, vector<int64_t> &numbers);

int main() {

    int n, k;
    scanf("%d",&n);
    scanf("%d",&k);

    vector<int64_t> numbers(n);
    for (int i=0; i<n; i++)
        scanf("%ld",&numbers[i]);

    printf("%d\n",selection(k,0,n-1,numbers));
    return 0;
}

// k - k-ta liczba liczac od 1, start - pierwszy index, end - ostatni index
int selection(int k, int start, int end, vector<int64_t> &numbers) {

    if (end-start < 7) {
        insert_sort(start,end,numbers);
        return numbers[start+k-1];
    }

    // mediany piatek
    int meds_size = ceil((end-start)/5.0);
    vector<int64_t> meds(meds_size);
    for (int i=0; i<meds_size-1; i++)
        meds[i] = median_force( start+5*i , start+5*(i+1)-1 , numbers );
    meds[meds_size-1] = median_force(start + 5*(meds_size-1), end, numbers);

    // podzial na dwa zbiory
    int64_t p = selection( (int)ceil(meds_size / 2.0) , 0, meds_size-1, meds );
    int i=start-1, j=end+1;
    while (true) {
        do { i++; } while(numbers[i] < p);
        do { j--; } while(numbers[j] > p);
        if (i<j) swap(numbers[i],numbers[j]);
        else break;
    } // j ma indeks podzialu

    int left = j-start+1; // ilosc elementow w lewym zbiorze
    if (k <= left) return selection(k,start,start+left-1,numbers);
    else return selection(k-left,start+left,end,numbers);
}

// start - pierwszy index, end - ostatni index
int median_force(int start, int end, vector<int64_t> &numbers) {
    int size = end - start + 1;
    int mid = start + size/2;
    insert_sort(start,end,numbers);
    return numbers[mid];
}

// start - pierwszy index, end - ostatni index
void insert_sort(int start, int end, vector<int64_t> &numbers) {

    for (int i=start+1; i<=end; i++) {
        int a=numbers[i], j = i-1;
        for (; numbers[j]>a && j>=start; j--)
            numbers[j+1] = numbers[j];
        numbers[j+1] = a;
    }

}
