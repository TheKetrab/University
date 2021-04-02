#include "bst.hpp"
#include <iostream>
#include <string>

namespace struktury {

/// KONSTRUKTORY
template <typename T, class C>
bst<T,C>::bst(std::initializer_list<T> args) {

    for(int i=0; i!=(int)args.size(); ++i)
        wstaw(args.begin()[i]);
}

template <typename T, class C>
bst<T,C>::bst() {
    root = nullptr;
}

template <typename T, class C>
bst<T,C>::bst(Node* root) {
    this->root = root;
}

template <typename T, class C>
bst<T,C>::bst(T val) {
    root = new Node(val);
}

template <typename T, class C>
bst<T,C>::Node::Node(T val) {
    left = nullptr;
    right = nullptr;
    this->val=val;
}

template <typename T, class C>
bst<T,C>::Node::Node(T val, Node *left, Node *right) {
    this->left = left;
    this->right = right;
    this->val=val;
}

/// ----- ----- -----
template <typename T, class C>
bst<T,C>::~bst() {
    Node::destroy(root);
    root = nullptr;
}

template <typename T, class C>
bst<T,C>::Node::~Node() {

    left=nullptr, right=nullptr;

}

/// ----- ----- -----
template <typename T, class C>
bst<T,C>::bst(const bst &b) {
    root = clone(b.root);
}

template <typename T, class C>
bst<T,C>::Node::Node(const Node &n) {
    *this=clone(n);
}

/// ----- ----- -----
template <typename T, class C>
bst<T,C>::bst(bst &&b) {
    root = b.root;
    b.root=nullptr;
}

template <typename T, class C>
bst<T,C>::Node::Node(Node &&n) {
    swap(left,n.left);
    swap(right,n.right);
    swap(val,n.val);
}

/// ----- ----- -----
template <typename T, class C>
bst<T,C>& bst<T,C>::operator= (const bst &b) {
    destroy(root); // czyszczenie
    root = clone(b.root);
    return *this;
}

template <typename T, class C>
typename bst<T,C>::Node& bst<T,C>::Node::operator= (const Node &n) {
    // czyszczenie
    destroy(left);
    destroy(right);
    // kopiowanie
    *this = clone(n);
    return *this;
}

/// ----- ----- -----
template <typename T, class C>
bst<T,C>& bst<T,C>::operator= (bst &&b) {
    destroy(root); // czyszczenie
    root = b->root;
    return *this;
}

template <typename T, class C>
typename bst<T,C>::Node& bst<T,C>::Node::operator= (Node &&n) {
    // czyszczenie
    destroy(left);
    destroy(right);
    // przepisanie wskaznika
    *this = n;
    return *this;
}


/// ----- WYPISYWANIE -----
template <typename T, class C>
void bst<T,C>::Node::wypisz() {
    if(this == nullptr) return;

    std::cout<<"(";
    left->wypisz();
    std::cout<<" "<<val<<" ";
    right->wypisz();
    std::cout<<")";
}

template <typename T, class C>
void bst<T,C>::wypisz() {
    root->wypisz();
}

template <typename X, class Y>
std::ostream& operator<< (std::ostream &wyj, const bst<X,Y> &b) {

    wyj << *b.root;
    return wyj;
}
/// -----


/// ----- WSTAWIANIE -----
template <typename T, class C>
void bst<T,C>::Node::wstaw(T val) {

    if(C::eq(val,this->val)) {
        Node *temp = right;
        right = new Node(val,nullptr,temp);
    }

    else if(C::l_less(val,this->val))
        if(left==nullptr) left = new Node(val);
        else left->wstaw(val);
    else
        if(right==nullptr) right = new Node(val);
        else right->wstaw(val);

}

template <typename T, class C>
void bst<T,C>::wstaw(T val) {

    if(root==nullptr)
        root = new Node(val);

    else
        root->wstaw(val);
}
/// -----


/// ----- WYSZUKIWANIE -----
template <typename T, class C>
typename bst<T,C>::Node* bst<T,C>::Node::min_val(Node* n) {

    Node* x = n;
    while (x->left != NULL)
        x = x->left;

    return x;
}

template <typename T, class C>
typename bst<T,C>::Node* bst<T,C>::Node::wyszukaj(Node* n, T val) {

    // null?
    if(n==NULL) return n;

    // znalezione?
    if(C::eq(val,n->val)) return n;

    // szukaj w odpowiednim poddrzewie
    if(C::l_less(val,n->val))
        return wyszukaj(n->left,val);
    else
        return wyszukaj(n->right,val);
}

template <typename T, class C>
bst<T,C> bst<T,C>::wyszukaj(T val) const {

    Node *res = Node::wyszukaj(root,val);
    return bst<T,C>(Node::clone(res));
}
/// -----

/// ----- USUWANIE -----
template <typename T, class C>
typename bst<T,C>::Node* bst<T,C>::Node::usun(Node* n, T val, Node*& ret) {

    /// jesli nie ma czego usunac
    if (n==nullptr) return n;

    /// szukaj w lewym/prawym
    if (C::l_less(val,n->val))
        n->left = usun(n->left,val,ret);
    else if (C::r_less(val,n->val))
        n->right = usun(n->right,val,ret);

    /// jesli trafiles na klucz
    else {
 //       ret = n->val; // zeby zwrocic ten wezel
        if(ret==nullptr) ret = new Node(n->val);

        // jesli brak poddrzew
        if(n->left==nullptr && n->right==nullptr)
            return nullptr;

        // jesli jedno puste
        if(n->left==nullptr) {
            Node *r = n->right;
            delete n;
            return r;
        }

        if(n->right==nullptr) {
            Node *l = n->left;
            delete n;
            return l;
        }

        // jesli oba niepuste
        Node *m = min_val(n->right);
        n->val = m->val;
        n->right = usun(n->right, m->val, ret);

    }

    return n;
}

template <typename T, class C>
T& bst<T,C>::usun(T val) {

    Node* res = nullptr;
    root = Node::usun(root,val,res);

    if(res==nullptr) {
        std::cerr<<"NIE ZNALEZIONO KLUCZA DO USUNIECIA"<<std::endl;
        return root->val;
    }

    return res->val;
}
/// -----



/// INNE
template <typename T, class C>
typename bst<T,C>::Node* bst<T,C>::Node::clone(Node *n) {

    if(n==nullptr) return n;

    Node *temp = new Node(n->val);
    temp->left = clone(n->left);
    temp->right = clone(n->right);
    return temp;
}

template <typename T, class C>
void bst<T,C>::Node::destroy(Node* n) {

	if(n == nullptr) return;

    if(n->left == nullptr && n->right == nullptr) {
        delete n;
        n = nullptr;
    }

    else {
        destroy(n->left);
        destroy(n->right);
    }
}



}
