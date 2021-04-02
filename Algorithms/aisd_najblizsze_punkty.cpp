#include <iostream>
#include <cstdint>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

typedef struct point {
    int64_t x,y;
} Point;


Point newPoint(int64_t x, int64_t y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

// zwraca liczbe dodatnia, jesli p1 jest bardziej na prawo
int cmpX(const void* p1, const void* p2) {

	int64_t res = ((Point*)p1)->x - ((Point*)p2)->x;
	if (res>0) return 1;
	if (res==0) return 0;
	return -1;
}

// zwraca liczbe dodatnia, jesli p1 jest wyzej
int cmpY(const void* p1, const void* p2) {

	int64_t res = ((Point*)p1)->y - ((Point*)p2)->y;
	if (res>0) return 1;
	if (res==0) return 0;
	return -1;
}

// odl p1,p2
long double dist(Point p1, Point p2) {
    return sqrtl((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}


pair<Point,Point> bruteForce(Point points[], int size) {  

    Point a = points[0];
    Point b = points[1];
    long double d = dist(a,b);

    for (int i=0; i<size; i++)
        for (int j=i+1; j<size; j++)
            if (dist(points[i], points[j]) < d) {
                a = points[i];
                b = points[j];
                d = dist(a,b);
            }

    return {a,b};
}  

pair<Point,Point> findNearest(Point X[], Point Y[], int size) {

    if (size <= 100)
		return bruteForce(X,size);


    // wyszukujemy srodkowy punkt
    int m = size/2;
    Point midP = X[m];

    pair<Point,Point> left  = findNearest(X, Y, m);             // lewa czesc
    pair<Point,Point> right = findNearest(X+m, Y, size-m);      // prawa czesc


    long double leftDist = dist(left.first,left.second);
    long double rightDist = dist(right.first,right.second);

    Point a,b;
    long double minDist;

    // wybieramy lepsze rozwiazanie z tych dwoch
    if (leftDist < rightDist) {
        minDist = leftDist;
        a = left.first;
        b = left.second;
    }

    else {
        minDist = rightDist;
        a = right.first;
        b = right.second;
    }


	
	
	
    // potencjalni kandydaci na nowa pare
    Point *potential = new Point[size];
    //vector<Point> potential;

    int potential_size = 0;
    for (int i=0; i<size; i++)
        // jesli jest potencjalnym kandydatem, to go dodaj
		// korzystamy z tablicy Y, ktora juz jest posortowana, zachowujemy posortowanie
        if (abs(midP.x - Y[i].x) < minDist) {
            potential[potential_size] = Y[i];
            potential_size++;
        }

    // zmodyfikuj rozwiazanie

    for (int i=0; i<potential_size; i++) {
        for (int j=1; i+j<potential_size && j<=8; j++) {
            if (dist(potential[i],potential[i+j]) < minDist) // sprawdzamy tylko 7 kolejnych punktow
            {
                a = potential[i];
                b = potential[j];
                minDist = dist(a,b);
            }
		}
	}		
	
	delete[] potential;

    return {a,b};
}

void printPoints(Point points[], int size) {

    for (int i=0; i<size; i++)
        cout<<points[i].x<<" "<<points[i].y<<endl;

}

int main() {

    int size; cin>>size;
    Point *X = new Point[size];
    Point *Y = new Point[size];
    int64_t x,y;

    for (int i=0; i<size; i++) {
        cin>>x>>y;
        Point p = newPoint(x,y);
		X[i] = p; Y[i] = p;
    }

    qsort(X, size, sizeof(struct point), cmpX); 
    qsort(Y, size, sizeof(struct point), cmpY); 

	
    //printPoints(points,size);
    pair<Point,Point> result = findNearest(X,Y,size);

    cout<<result.first.x<<" "<<result.first.y<<endl;
    cout<<result.second.x<<" "<<result.second.y<<endl;
    
    return 0;
}
