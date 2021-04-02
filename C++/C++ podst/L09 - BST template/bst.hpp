
#include <iostream>
#include <string>
#include <cstring>

#ifndef DEF_BST
#define DEF_BST



namespace struktury {


template <class T>
class por {

public:
    static bool eq (T a, T b) { return a==b; }
    static bool l_less (T a, T b) { return a<b; }
    static bool r_less (T a, T b) { return a>b; }

};



template<class T> class por<T*> {

public:
    static bool eq (T a, T b) { return *a==*b; }
    static bool l_less (T a, T b) { return *a<*b; }
    static bool r_less (T a, T b) { return *a>*b; }

};

template<> class por<const char*> {

public:
    static bool eq (const char* a, const char* b) {
        if(strcmp(a,b)==0)
            return true;
        return false;
    }

    static bool l_less (const char* a, const char* b) {
        if(strcmp(a,b)<0)
            return true;
        return false;
    }

    static bool r_less (const char* a, const char* b) {
        if(strcmp(a,b)>0)
            return true;
        return false;
    }

};









template <class T>
class rev {

public:
    static bool eq (T a, T b) { return a==b; }
    static bool l_less (T a, T b) { return a>b; }
    static bool r_less (T a, T b) { return a<b; }

};


class str_len {

public:
    static bool eq (std::string s1, std::string s2) { return s1.length()==s2.length(); }
    static bool l_less (std::string s1, std::string s2) { return s1.length() < s2.length(); }
    static bool r_less (std::string s1, std::string s2) { return s1.length() > s2.length(); }

};


// domyslne porownywanie wzgledem por
template <typename T, class C=por<T>>
class bst {

private:
    class Node;
    Node *root;

public:
    bst();
    bst(T val);
    bst(Node* root);
    bst(std::initializer_list<T> args);
    ~bst();
    bst(const bst &b);
    bst(bst &&b);
    bst<T,C>& operator= (const bst &b);
    bst<T,C>& operator= (bst &&b);

    void wstaw(T val);              /// wstawia do drzewa
    bst<T,C> wyszukaj(T val) const; /// zwraca nowe drzewo (kopie)
    T& usun(T val);                 /// zwraca usuwana wartosc
    void wypisz();                  /// wypisuje drzewo

    template <typename X, class Y>
    friend std::ostream& operator<< (std::ostream &wyj, const bst<X,Y> &b);

};

template <typename T, class C>
class bst<T,C>::Node {

    friend class bst;

private:
    Node *left;
    Node *right;
    T val;

    static Node* clone(Node *root);                 /// do grupowego kopiowania
    static Node* min_val(Node* n);                  /// zwraca node'a z minimalna wartoscia
    static Node* usun(Node* n, T val, Node*& ret);  /// usuwa val z n i zwraca wartosc usuwana
    static Node* wyszukaj(Node* n, T val);          /// zwraca adres val w n
    static void destroy(Node* n);                   /// delete wszystkich synow

public:
    Node(T val);
    Node(T val, Node *left, Node *right);
    ~Node();

    Node(const Node &n);
    Node(Node &&n);
    Node& operator= (const Node &n);
    Node& operator= (Node &&n);

public:

    // ===== ===== =====
    friend std::ostream& operator<< (std::ostream &wyj, typename bst<T,C>::Node &n) {

        if(&n==NULL) return wyj;
        wyj << *n.left;
        wyj << n.val << " ";
        wyj << *n.right;

        return wyj;
    }
    // ===== ===== =====

    private:
        void wstaw(T val);
        public: void wypisz();

    };





}

#endif
