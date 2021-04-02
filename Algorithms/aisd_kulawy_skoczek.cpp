#include <iostream>
#include <vector>
#include <cstdint>
#include <cinttypes>

using namespace std;

int w = 0;
int k = 0;


// zwraca zero jesli indeks spoza tablicy
uint64_t getVal(vector<uint64_t> &vec, int size, int index) {

    if (index < 0 || index >= size)
        return 0;

    return vec[index];
}


// skok o jeden w dol, o dwa w bok
vector<uint64_t> improve_jumpDown1(vector<uint64_t> v[], vector<uint64_t> original[], int index) {

    vector<uint64_t> result;
    result.resize(k,0);

	for (int i=0; i<k; i++)
        result[i] = original[index][i] + max(getVal(v[index-1],k,i-2),
                                             getVal(v[index-1],k,i+2));

    return result;
}


// skok o dwa w dol, o jedno w bok
vector<uint64_t> improve_jumpDown2(vector<uint64_t> v[], vector<uint64_t> original[], int index) {

    vector<uint64_t> result;
    result.resize(k,0);

	for (int i=0; i<k; i++)
        result[i] = original[index][i] + max(getVal(v[index-2],k,i-1),
                                          getVal(v[index-2],k,i+1));

    return result;
}


// skok o jeden w gore, o dwa w bok
vector<uint64_t> improve_jumpUp1(vector<uint64_t> v[], vector<uint64_t> original[], int index) {

    vector<uint64_t> result;
    result.resize(k,0);

	for (int i=0; i<k; i++)
        result[i] = max(getVal(v[index],k,i),
                        max(getVal(v[index+1],k,i-2),
                            getVal(v[index+1],k,i+2))
                        + original[index][i]);

    return result;
}

vector<uint64_t> init_seven() {

    vector<uint64_t> res;
    res.resize(11,0);

    uint64_t pow7 = 1;
    for (int i=0; i<11; i++) {
        res[i] = pow7;
        pow7 *= 7;
    }

    return res;
}

// przekształca bufor na wektor poteg siódemki -> zwraca wiersz
vector<uint64_t> readInput(char buffer[], vector<uint64_t> &seven) {
	
    scanf("%s", buffer);
    vector<uint64_t> row;
    row.resize(k,0);

    for (int i=0; i<k; i++) {
        row[i] = seven[ (int)(buffer[i] - '0') ];
    }

	return row;
}

void print(vector<uint64_t> v) {
    
    int s = v.size();
    for (int i=0; i<s; i++)
        cout<<v[i]<<" ";

    cout<<endl;

}

int main() {


    scanf("%d",&w);
    scanf("%d",&k);

    char buffer[k];

    vector<uint64_t> seven = init_seven();

    vector<uint64_t> modified[4];
	vector<uint64_t> original[4];
    
    for(int i=0; i<3; i++) {
        original[i] = readInput(buffer,seven);
        modified[i].resize(k,0); // wyzerowane wektory
    }

	modified[0] = original[0];

	if (w == 3) {

        

		modified[2] = improve_jumpDown2(modified,original,2);
        modified[1] = improve_jumpUp1(modified,original,1);

	}

	else

        // tylko w-2 razy, bo przeczytalismy juz 3 wiersze
        for (int i=0; i<w-3; i++) {

            original[3] = readInput(buffer,seven);
            modified[3] = original[3];


            modified[2] = improve_jumpDown2(modified,original,2);
            modified[1] = improve_jumpUp1(modified,original,1);
            modified[3] = improve_jumpDown2(modified,original,3);
            modified[2] = improve_jumpUp1(modified,original,2);

/*
            cout<<"MODIFIED[0] = "; print(modified[0]);
            cout<<"MODIFIED[1] = "; print(modified[1]);
            cout<<"MODIFIED[2] = "; print(modified[2]);
            cout<<"MODIFIED[3] = "; print(modified[3]);*/

            for (int j=0; j<3; j++) {
                original[j] = original[j+1];
                modified[j] = modified[j+1];
            }
		}


    uint64_t res = 0;
    for (int i=0; i<k; i++)
        res = max(res,modified[2][i]);

    printf("%" PRIu64 "\n", res);

	return 0;

}
