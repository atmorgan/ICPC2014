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

struct dinic_graph {
	size_t N;
	VVI A;
	VVT C, F;  // references to F can be removed if you don't want flows
	dinic_graph( size_t _N ) : N(_N), A(N), C(N,VT(N,0)), F(N,VT(N,0)) {}
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

// TODO: TESTS!

// Ripped from http://web.stanford.edu/~liszt90/acm/notebook.html#file1
/****************
 * Maximum flow * (Dinic's on an adjacency list + matrix)
 ****************
 * Takes a weighted directed graph of edge capacities as an adjacency 
 * matrix 'cap' and returns the maximum flow from s to t.
 *
 * PARAMETERS:
 *      - cap (global): adjacency matrix where cap[u][v] is the capacity
 *          of the edge u->v. cap[u][v] is 0 for non-existent edges.
 *      - n: the number of vertices ([0, n-1] are considered as vertices).
 *      - s: source vertex.
 *      - t: sink.
 * RETURNS:
 *      - the flow
 *      - prev contains the minimum cut. If prev[v] == -1, then v is not
 *          reachable from s; otherwise, it is reachable.
 * RUNNING TIME:
 *      - O(n^3)
 **/
#include <cstring>
#include <cstdio>

// the maximum number of vertices
#define NN 1024
const int INF = 2000000000;

// adjacency matrix (fill this up)
// If you fill adj[][] yourself, make sure to include both u->v and v->u.
int cap[NN][NN], deg[NN], adj[NN][NN];

// BFS stuff
int q[NN], prev[NN];
int dinic( int n, int s, int t ) {
    int flow = 0;
    while( true ) {
        memset( prev, -1, sizeof( prev ) );
        int qf = 0, qb = 0;
        prev[q[qb++] = s] = -2;
        while ( qb > qf && prev[t] == -1 )
            for ( int u = q[qf++], i = 0, v; i < deg[u]; i++ )
                if( prev[v = adj[u][i]] == -1 && cap[u][v] )
                    prev[q[qb++] = v] = u;
        if ( prev[t] == -1 ) break;
        for ( int z = 0; z < n; z++ ) if( cap[z][t] && prev[z] != -1 ) {
            int bot = cap[z][t];
            for ( int v = z, u = prev[v]; u >= 0; v = u, u = prev[v] )
                bot = min(bot, cap[u][v]);
            if ( !bot ) continue;
            cap[z][t] -= bot;
            cap[t][z] += bot;
            for ( int v = z, u = prev[v]; u >= 0; v = u, u = prev[v] ) {
                cap[u][v] -= bot;
                cap[v][u] += bot;
            }
            flow += bot;
        }
    }

    return flow;
}

//----------------- EXAMPLE USAGE -----------------
int main() {
    // read a graph into cap[][]
    memset( cap, 0, sizeof( cap ) );
    int n, s, t, m;
    scanf( " %d %d %d %d", &n, &s, &t, &m );
    while( m-- ) {
        int u, v, c; scanf( " %d %d %d", &u, &v, &c );
        cap[u][v] = c;
    }

    // init the adjacency list adj[][] from cap[][]
    memset( deg, 0, sizeof( deg ) );
    for( int u = 0; u < n; u++ )
        for( int v = 0; v < n; v++ ) if( cap[u][v] || cap[v][u] )
            adj[u][deg[u]++] = v;

    printf( "%d\n", dinic( n, s, t ) );
    return 0;
}
