#include <algorithm>
#include <limits>
#include <vector>

using namespace std;

#define FOR(v,l,u) for( size_t v = l; v < u; ++v )

// BEGIN
typedef double T; // also works for doubles, but use feq, etc
const T INFTY = numeric_limits<T>::max(); // ::infinity() for doubles

typedef vector<size_t> VI;
typedef vector<VI>     VVI;
typedef vector<T>      VT;
typedef vector<VT>     VVT;

// Edmonds-Karp algorithm for max-flow
// Runs in O(VE^2). Alternate complexity: O(f(V+E)),
// where f is the value of the maximum flow
struct edmondskarp_graph {
	size_t N;
	VVI A;
	VVT C, F;  // references to F can be removed if you don't want flows
	edmondskarp_graph( size_t _N ) : N(_N), A(N), C(N,VT(N,0)), F(N,VT(N,0)) {}
	void add_capacity( size_t s, size_t t, T cap ) {
		if( cap == 0 ) return;
		if( C[s][t] == 0 && C[t][s] == 0 ) {
			A[s].push_back(t);
			A[t].push_back(s);
		}
		C[s][t] += cap;
		// If you subtract capacities, and want to remove edges with cap 0,
		// do so here, or afterward.
	}
	T Augment( const VI &P ) {
		T amt = INFTY;
		FOR(i,0,P.size()-1) amt = min(amt, C[ P[i] ][ P[i+1] ]);
		FOR(i,0,P.size()-1) {
			size_t u = P[i], v = P[i+1];
			C[u][v] -= amt;
			C[v][u] += amt;
			if( F[v][u] > amt ) {
				F[v][u] -= amt;
			}
			else {
				F[u][v] += amt - F[v][u];
				F[v][u] = 0;
			}
		}
		return amt;
	}
	bool bfs( size_t s, size_t t, VI &P ) {
		P = VI(N,N);
		VI Q(N);  size_t qh=0, qt=0;
		P[  Q[qt++] = s  ] = s;
		while( qh < qt && P[t] == N ) {
			size_t c = Q[qh++];
			FOR(i,0,A[c].size()) {
				size_t u = A[c][i];
				if( C[c][u] == 0 ) continue;
				if( P[u] != N ) continue;
				P[  Q[qt++] = u  ] = c;
			}
		}
		return P[t] != N;
	}
	T ComputeMaxFlow( size_t s, size_t t ) {
		T flow = 0;
		VI P;
		while( bfs(s,t,P) ) {
			VI path(1,t);
			size_t z = t;
			while( z != P[z] ) path.push_back( z = P[z] );
			path = VI(path.rbegin(), path.rend());
			flow += Augment(path);
		}
		return flow;
	}
};
// END

int main() {
    //TODO: implement tests
    return 0;
}
