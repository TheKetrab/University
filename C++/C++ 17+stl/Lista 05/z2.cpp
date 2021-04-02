// cpp stl Bartlomiej Grochowski 300951
// zadanie 2 - graf

#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include <queue>
#include <set>

using namespace std;

// GRAF NIESKIEROWANY, NIE MULTIGRAF, NIE MOZE BYC PETLI
class graph {

    struct edge {
        int dest_id;
        float weight;
    };

    map<string,int> vertex_id;       // name -> id
    map<int,string> vertex_name;     // id -> name
    map<int,list<edge>> neighbours;  // id -> lista sasiadow
    int max_id = 0;

public:
    graph() = default;

    bool add_vertex(const string &name);
    bool remove_vertex(const string &name);

    bool add_edge(const string &from, const string &to, float weight);
    bool remove_edge(const string &from, const string &to);
    bool set_edge_weight(const string &from, const string &to, float weight);

    list<string> find_path(const string &from, const string &to);
    void print_graph();

};

bool graph::add_vertex(const string &name) {
    
    if (vertex_id.find(name) != vertex_id.end())
        return false; // vertex already exists

    int id = max_id++;
    vertex_id.insert({name,id});
    vertex_name.insert({id,name});
    return true;
}

bool graph::remove_vertex(const string &name) {

    if (vertex_id.find(name) == vertex_id.end())
        return false; // vertex not found

    int id = vertex_id[name];

    // ze wszystkich wierzcholkow usun krawedzie
    // prowadzace do tego wierzcholka
    for (auto& n : neighbours)
        n.second.remove_if([id](const edge &e)
            { return e.dest_id == id; });

    // usun liste sasiedztwa tego wierzcholka
    // i informacje o tym wierzcholku
    neighbours.erase(id);
    vertex_id.erase(name);
    vertex_name.erase(id);

    return true;
}

bool graph::add_edge(const string &from, const string &to, float weight) {

    if (from == to)
        return false; // loops forbidden

    if (vertex_id.find(from) == vertex_id.end()
     || vertex_id.find(to) == vertex_id.end())
        return false; // vertex not found

    int from_id = vertex_id[from];
    int to_id = vertex_id[to];
    list<edge> &el = neighbours[from_id];

    // jesli juz jest taka krawedz, to nie dodawaj
    if (find_if(el.begin(),el.end(),[to_id](const edge &e)
        { return e.dest_id == to_id; }) != el.end())
        return false;

    edge e = { to_id, weight };
    el.push_back(e);

    return true;
}

bool graph::remove_edge(const string &from, const string &to) {

    int from_id = vertex_id[from];
    int to_id = vertex_id[to];
    list<edge> &el = neighbours[from_id];

    auto it = remove_if(el.begin(),el.end(),[to_id](const edge &e)
        { return e.dest_id == to_id; });

    // czy taka krawedz wgl byla?
    if (it == el.end())
        return false;

    return true;
}

bool graph::set_edge_weight(const string &from, const string &to, float weight) {

    int from_id = vertex_id[from];
    int to_id = vertex_id[to];
    list<edge> &el = neighbours[from_id];

    // czy taka krawedz istnieje?
    if (find_if(el.begin(), el.end(), [to_id](const edge &e)
        { return e.dest_id == to_id; }) == el.end())
        return false;

    edge new_edge = { to_id, weight };
    replace_if(el.begin(),el.end(),[to_id](const edge &e)
        { return e.dest_id == to_id; }, new_edge);

    return true;
}

list<string> graph::find_path(const string &from, const string &to) {

    int from_id = vertex_id[from];
    int to_id = vertex_id[to];

    // BFS (O(m+n)) (to bedzie najkrotsza sciezka krawedziowo, ale nie wagowo)
    // do najkrotszej wagowo trzeba by uzyc Dijkstry (O(mlogv))

    queue<int> pending;
    set<int> visited;
    map<int,int> parent;

    parent[from_id] = from_id;
    visited.insert(from_id);
    pending.push(from_id);

    bool found = false;
    while (!pending.empty()) {

        int v = pending.front();
        pending.pop();

        if (v == to_id) {
            found = true; break;
        }

        list<edge> el = neighbours[v];
        for (auto& e : el) {

            int u = e.dest_id; // neighbour
            if (visited.count(u) == 0) {
                visited.insert(u);
                parent[u] = v;
                pending.push(u);
            }
        }
    }

    if (!found) return {}; // empty list

    // RESTORE PATH
    list<string> path;
    int v = to_id;
    do {
        path.push_front( vertex_name[v] );
        v = parent[v];
    } while (v != parent[v]);
    path.push_front( vertex_name[from_id] );

    return path;
}

void graph::print_graph() {

    for (auto x : neighbours)
        for (auto y : x.second) {
            cout << vertex_name[x.first];
            cout << " ---(" << y.weight << ")--> ";
            cout << vertex_name[y.dest_id] << endl;
        }
}


int main() {

    graph g;
    g.add_vertex("A");
    g.add_vertex("B");
    g.add_vertex("C");
    g.add_vertex("D");
    g.add_vertex("E");
    g.add_vertex("F");

    g.add_edge("A","E",7.5);
    g.add_edge("B","A",210);
    g.add_edge("B","C",1.2);
    g.add_edge("B","D",5.6);
    g.add_edge("C","D",113);
    g.add_edge("D","B",9.9);
    g.add_edge("E","C",0.5);
    g.add_edge("F","A",1.3);

    cout << endl;
    cout << "GRAPH IS:" << endl;
    g.print_graph();

    cout << endl;
    cout << "Path from E to A:" << endl;
    auto path = g.find_path("E","A");
    for (string& s : path)
        cout << s << " ";
    cout << endl;

    cout << endl;
    cout << "Set B->C to 200" << endl;
    g.set_edge_weight("B","C",200);
    g.print_graph();

    cout << endl;
    cout << "REMOVE 'C'" << endl;
    g.remove_vertex("C");
    g.print_graph();

    return 0;
}