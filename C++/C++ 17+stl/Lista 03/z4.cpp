// cpp stl Bartlomiej Grochowski 300951
// zadanie 4 - type traits

#include <iostream>
#include <type_traits>
#include <vector>
#include <functional>

using namespace std;

// NORMAL VERSION
template <typename From, typename To>
To& move_impl (From f, To &t, true_type) {

    static_assert(is_convertible< typename remove_pointer<From>::type , To >::value,
        "Unable to convert given pointer type.");

    cout << " >>> from is convertable pointer type <<< " << endl;
    t = (To)(*f); return t;
}

template <typename From, typename To>
To& move_impl (From f, To &t, false_type) {

    static_assert(is_convertible< From , To >::value,
        "Unable to convert given value type.");

    cout << " >>> from is convertable value type <<< " << endl;
    t = (To)f; return t;
}

template <typename From, typename To>
To& my_move (From f, To &t) {
    return move_impl(f ,t ,is_pointer<From>());
}

// CONVERT VERSION
template <typename From, typename To>
To& move_impl (From f, To &t, true_type, void (*convert)(From,To&)) {

    cout << " >>> from is pointer, given converter <<< " << endl;
    convert(*f,t); return t;
}

template <typename From, typename To>
To& move_impl (From f, To &t, false_type, void (*convert)(From,To&)) {

    cout << " >>> from is value, given converter <<< " << endl;
    convert(f,t); return t;
}

template <typename From, typename To>
To& my_move (From f, To &t, void (*convert)(From,To&)) {
    return move_impl(f ,t ,is_pointer<From>(), convert);
}

struct A { int v; };
struct B : A { };

void conv_vecINT_to_string(vector<int> vec, string &s) {
    
    string res; int n = vec.size();
    for (int i=0; i < n-1; i++)
        res += to_string(vec[i]) + " ";
    
    res += to_string(vec[n-1]);
    s = res;
}

int main() {

    string s0 = "string zero";
    string s1 = "string one";

    string *s2 = new string("string two");
    string s3 = "string three";

    struct A a { 5 };
    struct B b; b.v = 10;

    vector<int> v1 = {1,2,3,4,5};
    string s4 = "string four";

    // string ~> string
    cout << "Before move:" << endl;
    cout << "s0 ..... " << s0 << endl;
    cout << "s1 ..... " << s1 << endl;
    my_move(s0,s1);
    cout << "After move: (s0 -> s1)" << endl;
    cout << "s0 ..... " << s0 << endl;
    cout << "s1 ..... " << s1 << endl;

    cout << endl;

    // string_ptr ~> string
    cout << "Before move:" << endl;
    cout << "s2 ..... " << *s2 << endl;
    cout << "s3 ..... " << s3 << endl;
    my_move(s2,s3);
    cout << "After move: (s2[ptr] -> s3)" << endl;
    cout << "s2 ..... " << *s2 << endl;
    cout << "s3 ..... " << s3 << endl;

    cout << endl;

    // B ~> A, gdzie B:A
    cout << "Before move:" << endl;
    cout << "a.v .... " << a.v << endl;
    cout << "b.v .... " << b.v << endl;
    my_move(b,a);
    // my_move(a,b); // blad kompilacji
    cout << "After move: (b -> a)" << endl;
    cout << "a.v .... " << a.v << endl;
    cout << "b.v .... " << b.v << endl;

    cout << endl;

    // Jawnie podany konwerter: vector<int> ~> string
    cout << "Before move:" << endl;
    cout << "v1 ..... "; 
        for (auto &v : v1) 
            cout << v << " ";
        cout << endl;
    cout << "s4 ..... " << s4 << endl;
    
    my_move(v1,s4,conv_vecINT_to_string);

    cout << "After move: (b -> a)" << endl;
    cout << "v1 ..... "; 
        for (auto &v : v1) 
            cout << v << " ";
        cout << endl;
    cout << "s4 ..... " << s4 << endl;

    return 0;
}
