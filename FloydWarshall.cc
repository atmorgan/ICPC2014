#include <vector>
using namespace std;
// BEGIN
// Floyd-Warshall implementation with negative cycle detection.
// This will modify the graph, computing its transitive closure.
//
// If there is an upper bound for any simple path length,
// then create a constant INF equal to that,
// and set W[i][j] = INF when there is no edge i->j.
// You can then remove all reference to A.
// Notable generalizations:
// - Finding paths with maximum minimum-capacity-along-path
// - Transitive closure (done with A below)
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )
typedef signed long long int T; // anything with <, +, and 0
typedef vector<T>      VT;
typedef vector<VT>     VVT;
typedef vector<size_t> VI;  // only if you want the actual paths
typedef vector<VI>     VVI; // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
typedef vector<bool>   VB;  // only if you don't have an upper bound
typedef vector<VB>     VVB; // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

struct floydwarshall_graph {
	size_t N;   // Number of nodes
	VVB    A;   // [i][j] is true iff there exists an edge i -> j
	VVT    W;   // [i][j] is the weight of the edge i -> j.
	VVI    P;   // [i][j] is the next node in shortest path i -> j
	floydwarshall_graph( size_t n ) :
		N(n),  A(n,VB(n,false)),  W(n,VT(n,0)),  P(n,VI(n,n)) {}
	void add_edge( size_t s, size_t t, T w ) {
		A[s][t] = true;     W[s][t] = w;    P[s][t] = t;
	}
	bool floydwarshall() {
		FOR(k,0,N)    // We've computed paths using only {0, 1, ..., k-1}
		FOR(i,0,N)    // Now compute the shortest path from i -> j
		FOR(j,0,N) {  // when considering a path using k.
			if( A[i][k] && A[k][j] && !A[i][j] ) {   // transitive closure
				A[i][j] = true;
				W[i][j] = W[i][k] + W[k][j];
				P[i][j] = P[i][k];
			}
			if( !A[i][j] || !A[i][k] || !A[k][j] )   // skip invalids
				continue;
			if( W[i][k] + W[k][j] < W[i][j] ) {      // shortest path
				P[i][j] = P[i][k];
				W[i][j] = W[i][k] + W[k][j];
			}
		}
		FOR(i,0,N) if( W[i][i] < 0 ) return false; // negative cycle.
		return true; // no negative cycle.
	}
};
// END

#include <iostream>

void test_floydwarshall_correct() {
	{
		floydwarshall_graph G(6);
		G.add_edge(0,1, 7); G.add_edge(1,2, 8); G.add_edge(2,5, 3);
		bool r = G.floydwarshall();
		if( !r ) {
			cerr << "(test #1) algo. wrongly stated existence of negative cycles." << endl;
		}
		if( G.W[0][5] != 7+8+3 ) {
			cerr << "(test #1) algo. wrongly computes shortest path length." << endl;
		}
		if( G.P[1][5] != 2 ) {
			cerr << "(test #1) algo. wrongly computes shortest path (the path itself)." << endl;
		}
		if( !G.A[0][5] ) {
			cerr << "(test #1) algo. wrongly computes transitive closure." << endl;
		}
	}
	{
		floydwarshall_graph G(5);
		G.add_edge(0,1, 1); G.add_edge(1,2, 8000);
		G.add_edge(2,3, -2000000); G.add_edge(3,1, 8000);
		G.add_edge(1,4, 1);
		bool r = G.floydwarshall();
		if( r ) {
			cerr << "(test #2) algo. wrongly stated nonexistence of negative cycles." << endl;
		}
	}
}

int main() {
	test_floydwarshall_correct();
	return 0;
}

