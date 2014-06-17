#include <vector>
using namespace std;
// BEGIN
// An implementation of Kosaraju's algorithm for strongly-connected components,
// including code which constructs a "meta" graph with one node per SCC.
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )
typedef vector<size_t> VI;
typedef vector<VI>     VVI;
typedef vector<bool>   VB;
struct graph {
	size_t N;
	VVI    A; // Adjacency lists.
	VVI    B; // Reversed adjacency lists.
	VI     scc;    // scc[i] is the component to which i belongs
	size_t n_sccs; // the number of components
	graph( size_t n ) : N(n), A(n), B(n), scc(n) {}
	void add_edge( size_t s, size_t t ) {
		A[s].push_back(t);
		B[t].push_back(s);
	}
	bool has_edge( size_t s, size_t t ) { // only for compute_scc_graph
		FOR(i,0,A[s].size()) if( A[s][i] == t ) return true;
		return false;
	}
	void dfs_order( size_t rt, VB &Vis, VI &order ) {
		Vis[rt] = true;
		FOR(i,0,A[rt].size()) {
			size_t v = A[rt][i];
			if( Vis[v] ) continue;
			dfs_order( v, Vis, order );
		}
		order.push_back(rt);
	}
	void dfs_label( size_t rt, VB &Vis, size_t lbl, VI &out ) {
		Vis[rt] = true;
		out[rt] = lbl;
		FOR(i,0,A[rt].size()) {
			size_t v = A[rt][i];
			if( Vis[v] ) continue;
			dfs_label( v, Vis, lbl, out );
		}
	}
	void compute_sccs() {
		VB visited(N,false);   VI order;
		FOR(v,0,N) if( !visited[v] ) dfs_order(v, visited, order);
		swap(A,B);
		visited = VB(N,false); n_sccs = 0;
		FOR(i,0,N) {
			size_t v = order[N-1-i];
			if( !visited[v] ) dfs_label(v, visited, n_sccs++, scc);
		}
		swap(A,B);
	}
	void compute_scc_graph( graph &H ) {
		H = graph(n_sccs);
		FOR(v,0,N) {
			FOR(i,0,A[v].size()) {
				size_t u = A[v][i];
				size_t vv = scc[v], uu = scc[u];
				if( vv != uu && !H.has_edge(vv,uu) )
					H.add_edge(vv,uu);
			}
		}
	}
};
// END

#include <iostream>
#include <set>

void test_scc_correct() {
	cerr << "test SCC correctness" << endl;
	{
		graph G(6);
		G.add_edge(0,1); G.add_edge(1,0);
		G.add_edge(2,3); G.add_edge(3,4);
		G.add_edge(4,5); G.add_edge(5,2);
		G.compute_sccs();
		if( G.n_sccs != 2 ) {
			cerr << "(test #1) incorrect number of SCCs" << endl;
		}
		if( G.scc[0] != G.scc[1] ) {
			cerr << "(test #1) algo. mislabeled 0 and 1 as separate components." << endl;
		}
		set<size_t> t;
		t.insert( G.scc[2] ); t.insert(G.scc[3]); t.insert(G.scc[4]); t.insert(G.scc[5]);
		if( t.size() > 1 ) {
			cerr << "(test #1) algo. mislabeled 2,3,4,5 as not-the-same-component." << endl;
		}
	}
	{
		graph G(5);
		G.add_edge(0,1); G.add_edge(1,2); G.add_edge(2,0); G.add_edge(1,3); G.add_edge(3,4); G.add_edge(0,4);
		G.compute_sccs();
		if( G.n_sccs != 3 ) {
			cerr << "(test #2) incorrect number of SCCs" << endl;
		}
		set<size_t> t; t.insert(G.scc[0]); t.insert(G.scc[1]); t.insert(G.scc[2]);
		if( t.size() != 1 ) {
			cerr << "(test #2) algo. mislabeled 0,1,2 as not-the-same-component." << endl;
		}
	}
	{
		graph G(5);
		G.add_edge(0,1); G.add_edge(1,2); G.add_edge(2,3); G.add_edge(3,4);
		G.add_edge(0,3); G.add_edge(4,3);
		G.compute_sccs();
		graph H(0); G.compute_scc_graph(H);
		if( H.N != 4 ) {
			cerr << "(test #3) algo. mis-created SCC graph as having not the right number of nodes." << endl;
		}
		if( !H.has_edge(G.scc[0], G.scc[1]) ) {
			cerr << "(test #3) algo. mis-created SCC graph as not having an edge for 0->1" << endl;
		}
		if( !H.has_edge(G.scc[0], G.scc[3]) ) {
			cerr << "(test #3) algo. mis-created SCC graph as not having an edge for 0->3" << endl;
		}
		if( H.has_edge(G.scc[4], G.scc[0]) ) {
			cerr << "(test #3) algo. mis-created SCC graph as having an edge for 4->0" << endl;
		}
	}
}

int main() {
	test_scc_correct();
	return 0;
}

