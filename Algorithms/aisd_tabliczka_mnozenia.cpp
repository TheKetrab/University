
/* ----- ----- ----- ----- ----- ----- ----- -----
 * Iterujemy od tylu inicjujac coraz to nowe slupki
 * slupki - kolumny, wysokosc na jakiej juz jestesmy.
 * W tych slupkach przechowujemy info o kolejnych
 * potencjalnych maksimach. Z potencjalnych maksim
 * wybieramy to jedno, modyfikujemy nowe potencjalne
 * maksima, a to wszystko w czasie O(log) - kopiec.
 * ----- ----- ----- ----- ----- ----- ----- ----- */

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <cinttypes>

using namespace std;

int K;
uint64_t N;
int size; // wielkosc kopca, wskaznik na koniec

typedef struct item {
	uint64_t val; // wartosc
	int index;    // z ktorego slupka ta wartosc przyszla
} __attribute__((packed)) Item; // zeby mialo 12B, a nie 16B


// HEAP
// zakladamy ze kopiec jest niepusty
// size - wskaznik na ostatni zajety element kopca
void insert(Item heap[], Item elem) {

	int actual, parent;
	
	actual = size+1;
	heap[actual] = elem;
	while(true) {
		
		parent = (actual-1)/2;
		if (heap[actual].val <= heap[parent].val) break; // ok
		else {
			swap(heap[actual],heap[parent]);
			actual = parent;
		}
	}
	
	size++;	
}


// zwraca element maksymalny i porzadkuje kopiec
Item take_max_inside(Item heap[]) {
	
	Item result = heap[0];
	swap(heap[0],heap[size]);
	size--;
	
	int actual, left, right;
	int ptr_max; // do sprawdzania, czy najwieksza wartosc jest w korzeniu kopczyka

	actual = 0;
	ptr_max = 0;
	while(true) {
		
		// byc moze left/right wychodzi poza tablice, czyli nie ma juz syna
		left = actual*2 + 1;
		right = actual*2 + 2;
		
		if (left<=size && heap[left].val > heap[actual].val)
			ptr_max = left;
		
		if (right<=size && heap[right].val > heap[actual].val)
			// wez prawy tylko jesli jest wiekszy niz lewy
			if (heap[right].val > heap[left].val)
				ptr_max = right;
		
		if (ptr_max == actual) // to regula kopca jest zachowana
			break;
		
		// zamien z wiekszym i kopcuj dalej
		swap(heap[actual],heap[ptr_max]);
		actual = ptr_max;
	}
	
	return result;
}


// zwraca wektor itemow (maxy, z ta sama wartoscia)
vector<Item> take_max(Item heap[]) {

	Item top = take_max_inside(heap);
	vector<Item> result = { top };

	// dopoki na topie kopca sa takie same wartosci i kopiec niepusty,
	// to je bierz, bo bedziesz zwiekszal ich slupki
	while (heap[0].val == top.val && size>=0) {
		Item item = take_max_inside(heap);
		result.push_back(item);
	}
	
	return result;	
}


// zwraca wartosc w tabliczce mnozenia (col = index+1)
uint64_t get_act(const int index, const int h) {
    return (uint64_t)(index+1) * (uint64_t)(N-h);
}


// podnosi slupek, moze zainicjowac nowy slupek i wrzucic na kopiec wartosc
void move_h(Item heap[], vector<int> &hvec, const int index) {

	// jesli podnosisz slupek zerowy, zainicjuj nowy slupek zerem
	// (ustaw mu wysokosc na 0) i wrzuc kolejna wartosc na kopiec
    if (hvec[index] == 0 && index != 0) {
        hvec[index-1] = 0;
		uint64_t new_val = get_act(index-1,hvec[index-1]);
		Item new_item = { new_val, index-1 };
		insert(heap,new_item);
    }
    
    hvec[index]++;
    
	// jesli slupek jeszcze nie doszedl do przekatnej,
	// to wstaw kolejna wartosc na kopiec
    if (hvec[index] < ((int)N-index)) { 
		uint64_t new_val = get_act(index,hvec[index]);
		Item new_item = { new_val, index };
		insert(heap,new_item);
	}

}


// modyfikuje wysokosc slupkow na podstawie usunietych wartosci przez take_max
void modify_h(vector<Item> &max_items, vector<int> &hvec, Item heap[]) {
	
	int s = max_items.size();

	// podnies wszystkie slupki tych co wziales z kopca
	for (int i=0; i<s; i++)
		move_h(heap,hvec,max_items[i].index);		
}


// wypisuje kopiec, tablice, item.val
void print_heap(Item heap[]) {
	
	for (int i=0; i<=size; i++)
		cout<<heap[i].val<<" ";
	
	cout<<endl;
}


int main() {

	Item heap[10000];
	size = 0; // wskazuje ostatni zajmowany przez kopiec index
    
	cin>>N;
	cin>>K;
	
    int found = 0;
    vector<int> h; // wysokosc wskaznika (na poczatku niezainicjowane)
    h.reserve(N);

	heap[0] = { N*N, (int)N-1 }; // wrzucamy najwieksza wartosc na kopiec
    h[N-1] = 0;

    while (found < K) {

		//cout<<"HEAP = "; print_heap(heap);
        vector<Item> max_items = take_max(heap);
        printf("%" PRIu64 "\n", max_items[0].val);
        found++;
		modify_h(max_items,h,heap);

    }

    return 0;
}
