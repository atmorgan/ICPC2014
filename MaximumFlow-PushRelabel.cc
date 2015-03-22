#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
// BEGIN
// Push-relabel implementation of maximum flow.
// This achieves an O( |V|^3 ) complexity by sheer force of will.
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )
typedef signed long long int T; // basic flow/capacity data type.
typedef vector<T>            VT;
typedef vector<VT>           VVT;
typedef vector<size_t>       VI;
typedef vector<VI>           VVI;
typedef queue<size_t>        QI;
typedef vector<bool>         VB;

struct maxflow_graph {
	const size_t N;
	VVI    adj;
	VVT    cap, flow;     // cap is *residual* capacity!
	VT     excess;        // excesses
	VI     height, count; // height, # nodes of specific height
	QI     Q; VB inQ;     // discharge queue
	maxflow_graph( size_t N ) : N(N), adj(N), cap(N,VT(N,0)), flow(N,VT(N,0)) {}
	void add_cap( size_t s, size_t t, T c ) {
		if( c == 0 ) return;
		if( cap[s][t] + cap[t][s] == 0 ) {
			adj[s].push_back(t);
			adj[t].push_back(s);
		}
		cap[s][t] += c;
	}
	void enqueue( size_t v ) {
		if( inQ[v] || excess[v] == 0 ) return;
		inQ[v] = true;  Q.push(v);
	}
	void push( size_t s, size_t t ) {
		T amt = min( excess[s], cap[s][t] );
		if( height[s] <= height[t] || amt == 0 ) return;
		cap[s][t] -= amt;  cap[t][s] += amt;
		if( flow[t][s] >= amt ) flow[t][s] -= amt;
		else { flow[s][t] = amt - flow[t][s]; flow[t][s] = 0; }
		excess[s] -= amt;  excess[t] += amt;
		enqueue( t );
	}
	void checkgap( size_t h ) {
		FOR(v,0,N) {
			if( height[v] < h ) continue;
			--count[height[v]];
			height[v] = max(height[v], N+1);
			++count[height[v]];
			enqueue( v );
		}
	}
	void relabel( size_t v ) {
		--count[ height[v] ];
		height[v] = 2*N;
		FOR(i,0,adj[v].size()) {
			size_t u = adj[v][i];
			if( cap[v][u] > 0 )
				height[v] = min(height[v], height[u]+1);
		}
		++count[ height[v] ];
		enqueue(v);
	}
	void discharge( size_t v ) {
		FOR(i,0,adj[v].size()) {
			if( excess[v] == 0 ) break;
			size_t u = adj[v][i];
			push( v, u );
		}
		if( excess[v] > 0 ) {
			if( count[ height[v] ] == 1 ) checkgap(v);
			else                          relabel(v);
		}
	}
	T ComputeMaxFlow( size_t s, size_t t ) {
		excess = VT(N,0);  height = VI(N,0);  count = VI(2*N,0);
		inQ = VB(N,false); while( !Q.empty() ) Q.pop();
		count[0] = N-1;  count[N] = 1;
		height[s] = N;
		inQ[s] = inQ[t] = true; // don't process s or t
		FOR(i,0,adj[s].size()) {
			excess[s] += cap[s][ adj[s][i] ];
			push( s, adj[s][i] );
		}
		while( !Q.empty() ) {
			size_t v = Q.front(); Q.pop(); inQ[v] = false;
			discharge( v );
		}
		T ret = 0;
		FOR(i,0,adj[s].size()) ret += flow[s][ adj[s][i] ];
		return ret;
	}
};
// END

#include <iostream>

void test_pushrelabel_correct() {
	cerr << "test push-relabel correctness" << endl;
	{
		maxflow_graph G(5);
		G.add_cap( 0,1, 3 );
		G.add_cap( 0,2, 5 );
		G.add_cap( 1,3, 5 );
		G.add_cap( 2,3, 4 );
		G.add_cap( 3,4, 10 );
		T f = G.ComputeMaxFlow( 0, 4 );
		if( f != 7 ) {
			cerr << "(test #1) expected maximum flow of 7, got " << f << endl;
		}
	}
	{
		maxflow_graph G(5);
		G.add_cap( 0,1, 3 );
		G.add_cap( 0,2, 5 );
		G.add_cap( 1,3, 5 );
		G.add_cap( 2,3, 4 );
		G.add_cap( 3,4, 6 );
		T f = G.ComputeMaxFlow( 0, 4 );
		if( f != 6 ) {
			cerr << "(test #2) expected maximum flow of 6, got " << f << endl;
		}
	}
}

int main() {
	test_pushrelabel_correct();
	return 0;
}

