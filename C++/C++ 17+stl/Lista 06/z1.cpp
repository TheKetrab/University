// cpp stl Bartlomiej Grochowski 300951
// zadanie 1 - algorytmy stl

#include <iostream>
#include <algorithm>
#include <deque>

using namespace std;

class person {

public:
    string name, surname;
    float weight, height; // kg, m
    int age;

public:
    person(string name, string surname, float weight, float height, int age)
        : name(name), surname(surname), weight(weight), height(height), age(age) {}

    float bmi() const {
        return weight / (height * height);
    }

    void print() const {
        cout << name << " " << surname << " (" << age << ")\t\t"
        << weight << " kg \t" << height*100 << " cm" << endl;
    }
};

// dane
person p1("Paulina","Zielińska",    50.3,   1.63,   14);
person p2("Jakub","Marczacki",      90.1,   1.81,   18);
person p3("Adam","Dąbrowski",       66.8,   1.72,   32);
person p4("Jowita","Zawadzka",      72.1,   1.83,   25);
person p5("Monika","Kowalewicz",    49.1,   1.68,   22);
person p6("Antoni","Nowakiewicz",   92.3,   1.93,   21);
person p7("Gabriela","Walczak",     66.1,   1.63,   48);
person p8("Maksymilian","Lange",    66.1,   1.59,   13);
person p9("Michał","Wiśniewski",    78.2,   1.81,   27);
person p10("Wojciech","Karpiński",  87.3,   1.88,   28);
person p11("Jakub","Chmielewski",   94.5,   1.83,   34);
deque<person> d = {p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11};

// sortowanie wg BMI
void A() {

    deque<person> da(d);
    sort(da.begin(), da.end(),
        [](const person p1, const person p2) {
            return p1.bmi() < p2.bmi();   
        });

    for (auto p : da)
        p.print();
}

// nadpisywanie obiektów w kolekcji
void B() {

    deque<person> db(d);
    transform(db.begin(), db.end(), db.begin(), 
      [](person &p) { p.weight = 0.9 * p.weight; return p; });

    for (auto p : db)
        p.print();
}

// podział na dwie grupy
void C() {

    deque<person> dc(d);
    auto pivot = partition(dc.begin(), dc.end(),
        [](const person &p) { return p.weight <= 80; });

    // weight <= 80
    cout << "Lżejsze niż 80kg:" << endl;
    for (auto it = dc.begin(); it != pivot; it++)
        (*it).print();
    
    // weight > 80
    cout << "Cięższe niż 80kg:" << endl;
    for (auto it = pivot; it != dc.end(); it++)
        (*it).print();
}

// mediana przesunięta na pozycję k
void D() {

    deque<person> dd(d);

    int m = d.size() / 2;
    nth_element(dd.begin(), dd.begin() + m, dd.end(),
        [](const person& p1, const person &p2) {
            return p1.height < p2.height;
        });

    // teraz n-ty element co do wielkości jest na miejscu dd.begin() + n
    // opcjonalnie przesuwamy go na pozycję k:
    // int k = 1;
    // swap(dd.begin() + m, dd.begin() + k);

    for (auto x : dd)
        x.print();
}

// shuffle
void E() {

    deque<person> de(d);
    random_shuffle(de.begin(),de.begin()+5);
    random_shuffle(de.end()-5,de.end());

    for (auto x : de)
        x.print();
}

// minmax
void F() {

    auto res = minmax_element(d.begin(),d.end(),
        [](const person &p1, const person &p2) {
            return p1.age < p2.age;
        });

    cout << "Osoba najmłodsza:\t"; res.first->print();
    cout << "Osoba najstarsza:\t"; res.second->print();
}

#define TEST(f,msg) \
    cout << "----- ----- ----- ----- -----" << endl; \
    cout << "  " << msg << endl; \
    cout << "----- ----- ----- ----- -----" << endl; \
    f(); cout << endl;

int main() {

    TEST(A,"Posortowane wg BMI");
    TEST(B,"Waga mniejsza o 10%");
    TEST(C,"Podział na dwie grupy");
    TEST(D,"Mediana wysokości");
    TEST(E,"Losowo poprzestawiane");
    TEST(F,"Najmłodszy, najstarszy");

    return 0;
}
