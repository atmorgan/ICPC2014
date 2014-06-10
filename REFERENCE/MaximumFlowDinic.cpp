#include <cstring>
#include <cstdio>
#include <algorithm>

using namespace std;

// BEGIN
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
// END
