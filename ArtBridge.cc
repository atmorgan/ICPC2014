#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>
using namespace std;
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )
// BEGIN
// This is code for computing articulation points of graphs,
// ie points whose removal increases the number of components in the graph.
// This works when the given graph is not necessarily connected, too.
typedef vector<size_t> VI;
typedef vector<VI>     VVI;
typedef vector<bool>   VB;
typedef pair<size_t, size_t> II;
typedef vector<II> VII;

struct artbridge_graph {
    size_t N;     VVI adj;        // basic graph stuff
    VI  parent, n_children, rank; // dfs tree
    VB  is_art;   VI reach;       // articulation points
    VII bridges;              // bridges
    VB visited; size_t R;
    artbridge_graph( size_t N ) : N(N), adj(N), is_art(N) {}
    void add_edge( size_t s, size_t t ) {
        adj[s].push_back(t);
        adj[t].push_back(s);
    }
    void dfs_artpts( size_t rt ) {
        visited[rt] = true;
        rank[rt] = R++;
        reach[rt] = rank[rt];
        FOR(i,0,adj[rt].size()) {
            size_t v = adj[rt][i];
            if( v == parent[rt] ) continue;
            if( visited[v] )
                reach[rt] = min(reach[rt], rank[v]);
            else {
                ++n_children[rt];
                parent[v] = rt;
                dfs_artpts( v );
                reach[rt] = min(reach[rt], reach[v]);
                if (reach[v] >= rank[rt])
                    is_art[rt] = true;
                if (reach[v] > rank[rt])
                    bridges.push_back(II(min(rt, v), max(rt, v)));
            }
        }
    }
    // an iterative version. Should not be needed if environment is set up right.
    void dfs_artpts_it( size_t rt) {
        stack<size_t> s;
        s.push(rt);
        while (!s.empty()) {
            size_t cur = s.top();
            if (!visited[cur]) {
                visited[cur] = true;
                rank[cur] = R++;
                reach[cur] = rank[cur];
            }
            bool done = true;
            FOR(i,0,adj[cur].size()) {
                size_t v = adj[cur][i];
                if (v == parent[cur]) continue;
                if (visited[v]) {
                    reach[cur] = min(reach[cur], rank[v]);
                    if (parent[v] == cur) {
                        reach[cur] = min(reach[cur], reach[v]);
                    }
                }
                else {
                    done = false;
                    ++n_children[cur];
                    parent[v] = cur;
                    s.push(v);
                    break;
                }
                if (reach[v] >= rank[cur])
                    is_art[cur] = true;
                if (reach[v] > rank[cur])   //this might create duplicates
                    bridges.push_back(II(min(cur, v), max(cur, v)));
            }
            if (done)
                s.pop();
        }
    }
    void comp_artbridge() {
        is_art = VB(N, false);  reach = VI(N);
        parent = VI(N,N);      rank = VI(N);      n_children = VI(N,0);
        visited = VB(N,false);   R = 0;
        FOR(i,0,N) {
            if( visited[i] ) continue;
            dfs_artpts(i); // this is not right on i
            is_art[i] = (n_children[i] >= 2); // but we can fix it!
        }
    }
};
// END

#include<set>

void test_artpts_correct() {
    cerr << "test correctness" << endl;
    {
        artbridge_graph G(5);
        FOR(v,1,5) G.add_edge(0,v);
        G.comp_artbridge();
        if( !G.is_art[0] ) {
            cerr << "(test #1) algo. says 0 is not an articulation point." << endl;
        }
        size_t bad_ct = 0;
        FOR(v,1,5) if( G.is_art[v] ) ++bad_ct;
        if( bad_ct > 0 ) {
            cerr << "(test #1) algo. says " << bad_ct << " extra articulation points." << endl;
        }
    }
    {
        artbridge_graph G(3);
        FOR(v,0,3) G.add_edge(v, (v+1)%3);
        G.comp_artbridge();
        size_t bad_ct = 0;
        FOR(v,0,3) if( G.is_art[v] ) ++bad_ct;
        if( bad_ct > 0 ) {
            cerr << "(test #2) algo. says " << bad_ct << " extra articulation points." << endl;
        }
    }
    {
        artbridge_graph G(3);
        FOR(v1,0,3) FOR(v2,v1+1,3) G.add_edge(v1,v2);
        G.comp_artbridge();
        size_t bad_ct = 0;
        FOR(v,0,3) if( G.is_art[v] ) ++bad_ct;
        if( bad_ct > 0 ) {
            cerr << "(test #3) algo. says " << bad_ct << " extra articulation points." << endl;
        }
    }
    {
        artbridge_graph G(4);
        G.add_edge(3,2); G.add_edge(2,0); G.add_edge(3,1);
        G.comp_artbridge();
        size_t bad_ct1 = 0, bad_ct2 = 0;
        if( G.is_art[0] ) ++bad_ct1;
        if( G.is_art[1] ) ++bad_ct1;
        if( !G.is_art[2] ) ++bad_ct2;
        if( !G.is_art[3] ) ++bad_ct2;
        if( bad_ct1+bad_ct2 > 0 ) {
            cerr << "(test #4) algo. is wrong about " << bad_ct1 << " extra + " << bad_ct2 << " missed vertices." << endl;
        }
    }
    {
        artbridge_graph g(6);
        g.add_edge(0, 1);
        g.add_edge(1, 2);
        g.add_edge(2, 3);
        g.add_edge(3, 0);
        g.add_edge(1, 4);
        g.add_edge(4, 5);
        g.comp_artbridge();
        if (g.is_art[0] != 0) {
            cerr << "(test #5) algo. is wrong about vertex " << 0 << endl;
        }
        if (g.is_art[1] != 1) {
            cerr << "(test #5) algo. is wrong about vertex " << 1 << endl;
        }
        if (g.is_art[2] != 0) {
            cerr << "(test #5) algo. is wrong about vertex " << 2 << endl;
        }
        if (g.is_art[3] != 0) {
            cerr << "(test #5) algo. is wrong about vertex " << 3 << endl;
        }
        if (g.is_art[4] != 1) {
            cerr << "(test #5) algo. is wrong about vertex " << 4 << endl;
        }
        if (g.is_art[5] != 0) {
            cerr << "(test #5) algo. is wrong about vertex " << 5 << endl;
        }
    }
    {
        artbridge_graph g(8);
        g.add_edge(0, 1);
        g.add_edge(1, 2);
        g.add_edge(2, 0);
        g.add_edge(1, 4);
        g.add_edge(4, 3);
        g.add_edge(3, 1);
        g.add_edge(4, 5);
        g.add_edge(5, 7);
        g.add_edge(5, 6);
        g.comp_artbridge();
        
        set<II> bridges(g.bridges.begin(), g.bridges.end());

        if (bridges.size() != 3) {
            cerr << "(test #6) algo. is wrong about number of bridges" << endl;
            cerr << "bridges:" << endl;
            for (auto it = g.bridges.begin(); it != g.bridges.end(); ++it) {
                cerr << it->first << " " << it->second << endl;
            }
            cerr << g.reach[1] << endl;
        }
        if (bridges.count(II(4, 5)) != 1) {
            cerr << "(test #6) algo. did not detect bridge 4-5" << endl;
        }
        if (bridges.count(II(5, 7)) != 1) {
            cerr << "(test #6) algo. did not detect bridge 5-7" << endl;
        }
        if (bridges.count(II(5, 6)) != 1) {
            cerr << "(test #6) algo. did not detect bridge 5-6" << endl;
        }
        if (g.is_art[1] == 0) {
            cerr << "(test #6) algo. is wrong about vertex " << 1 << endl;
        }
        if (g.is_art[4] == 0) {
            cerr << "(test #6) algo. is wrong about vertex " << 4 << endl;
        }
        if (g.is_art[5] == 0) {
            cerr << "(test #6) algo. is wrong about vertex " << 5 << endl;
        }
    }
}

void test_artpts_stack() {
    {
        cerr << "Testing stackoverflow" << endl;
        cerr << "This should not segfault if system environment is set up correctly" << endl;
        //cerr << "Run command \"ulimit -s 268435456\"" << endl;
        cerr << "Run command \"ulimit -s 262144\" for codeforces stack size" << endl;
        cerr << "Run command \"ulimit -s 65536\" for ICPC World Finals stack size" << endl;
        size_t N = 650000;
        artbridge_graph G(N);
        for (size_t i = 0; i < N-1; ++i) {
            G.add_edge(i, i+1);
        }
        G.comp_artbridge();
    }
}

void test_artpts_speed() {
    const size_t N = 200000, D = 30; // 2e6
    cerr << "Start speed test... N = " << N << ", D = " << D << endl;
    artbridge_graph *G = new artbridge_graph(N);
    FOR(d,1,1+D) FOR(i,0,N) G->add_edge( i, (d*(i+1)) % N );
    G->comp_artbridge();
    cerr << "End speed test." << endl;
    size_t bad_ct = 0;
    FOR(v,0,N) if( G->is_art[v] ) ++bad_ct;
    cerr << bad_ct << " incorrect articulation points." << endl;
    delete G;
}

int main() {
    test_artpts_correct();
    test_artpts_stack();
    test_artpts_speed();
    return 0;
}

