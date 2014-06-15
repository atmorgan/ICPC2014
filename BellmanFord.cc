#include <vector>
#include <limits>
using namespace std;
// BEGIN
// A Bellman-Ford implementation.
// bellmanford(S) computes the shortest paths from S to all other nodes.
// It returns true if there are no negative cycles in the graph,
// and false otherwise.
// D[v] is set to the shortest path from S to v (when it exists).
// P[v] is set to the parent of v in the shortest-paths tree,
// or N (for which there is no index) if v is not reachable from S.
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )
typedef signed long long int T;
typedef vector<T>       VT;
typedef vector<VT>     VVT;
typedef vector<bool>    VB;
typedef vector<VB>     VVB;
typedef vector<size_t>  VI;
const T UNBOUNDED = numeric_limits<T>::min(); // -infinity for doubles
const T INFINITY  = numeric_limits<T>::max(); // infinity for doubles

struct bellmanford_graph {
	size_t N; // number of nodes
	VVB    A; // adjacency matrix
	VVT    W; // weight of edges (take mins if there are multiple)
	VT     D; // shortest distance
	VI     P; // parent in the shortest path tree
	bellmanford_graph( size_t N ) : N(N), A(N,VB(N,false)), W(N,VT(N,0)) {}
	void add_edge( size_t s, size_t t, T w ) {
		A[s][t] = true; W[s][t] = w;
	}
	bool bellmanford( size_t S ) {
		D = VT(N, INFINITY); D[S] = 0; P = VI(N,N);
		FOR(k,0,N)
		FOR(i,0,N)
		FOR(j,0,N) {
			if( !A[i][j] ) continue;
			if( D[i] == INFINITY ) continue;
			if( D[j] > D[i] + W[i][j] ) {
				if( k == N-1 ) return false;
				D[j] = D[i] + W[i][j];
				P[j] = i;
			}
		}
		return true;
	}
};
// END

#include <iostream>

void test_bellmanford_correct() {
	cerr << "test bellmanford_graph correctness" << endl;
	{
		bellmanford_graph G(4);
		G.add_edge(0, 1, 7);
		G.add_edge(0, 2, 5);
		G.add_edge(1, 3, 2);
		G.add_edge(2, 3, 6);
		if( !G.bellmanford(0) ) {
			cerr << "(test #1) algo. incorrectly reported existence of negative cycle" << endl;
		}
		if( G.D[1] != 7 ) cerr << "(test #1) algo. incorrectly reported distance 0->1" << endl;
		if( G.D[2] != 5 ) cerr << "(test #1) algo. incorrectly reported distance 0->2" << endl;
		if( G.D[3] != 9 ) cerr << "(test #1) algo. incorrectly reported distance 0->3" << endl;
		if( G.P[1] != 0 ) cerr << "(test #1) algo. incorrectly reported parent of 1" << endl;
		if( G.P[2] != 0 ) cerr << "(test #1) algo. incorrectly reported parent of 2" << endl;
		if( G.P[3] != 1 ) cerr << "(test #1) algo. incorrectly reported parent of 3" << endl;
	}
	{
		bellmanford_graph G(4);
		G.add_edge(0, 1, -7);
		G.add_edge(0, 2, -5);
		G.add_edge(1, 3, -2);
		G.add_edge(2, 3, -6);
		if( !G.bellmanford(0) ) {
			cerr << "(test #2) algo. incorrectly reported existence of negative cycle" << endl;
		}
		if( G.D[1] != -7  ) cerr << "(test #2) algo. incorrectly reported distance 0->1" << endl;
		if( G.D[2] != -5  ) cerr << "(test #2) algo. incorrectly reported distance 0->2" << endl;
		if( G.D[3] != -11 ) cerr << "(test #2) algo. incorrectly reported distance 0->3" << endl;
	}
	{
		bellmanford_graph G(4);
		G.add_edge(0, 1, -7);
		G.add_edge(1, 2, -5);
		G.add_edge(2, 3, -2);
		G.add_edge(3, 0, -6);
		if( G.bellmanford(0) ) {
			cerr << "(test #3) algo. incorrectly reported nonexistence of negative cycle" << endl;
		}
	}
	{
		bellmanford_graph G(5);
		G.add_edge(0,1,-50000);
		G.add_edge(0,2, 50000);
		G.add_edge(2,1, 50000);
		G.add_edge(2,3, 0);
		G.add_edge(3,4,-1);
		G.add_edge(4,2, 0);
		if( G.bellmanford(0) ) {
			cerr << "(test #4) algo. incorrectly reported nonexistence of negative cycle" << endl;
		}
	}
}

void test_bellmanford_speed() {
	const size_t N = 700;
	bellmanford_graph G(N);
	FOR(i,0,N)
	FOR(j,0,N)
		if( i != j )
			G.add_edge(i,j, 1);
	cerr << "Start speed test..." << endl;
	G.bellmanford(0);
	cerr << "End speed test." << endl;
}

int main() {
	test_bellmanford_correct();
	test_bellmanford_speed();
	return 0;
}

