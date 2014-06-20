#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
using namespace std;
// BEGIN
// Min-cost Maximum Flow. The implementation has a few stages, some of which
// can be outright ignored if the graph is guaranteed to satisfy certain
// conditions. These are documented below.
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )
typedef signed long long int T;   // the basic type of costs and flow.
typedef vector<T>            VT;
struct edge {
	size_t s, t;
	T cap, flow, cost;  // Note: cap is *residual* capacity.
	size_t di; // index of dual in t's edgelist.
	edge *dual; // the actual dual (see "compile_edges")
};
typedef vector<edge>   VE;
typedef vector<VE>     VVE;
typedef vector<size_t> VI;
typedef pair<T,size_t> DijkP;  // Dijkstra PQ element.
typedef priority_queue<DijkP, vector<DijkP>, std::greater<DijkP> >  DijkPQ;

struct mcmf_graph {
	size_t N;   VVE adj;  VT pot;
	mcmf_graph( size_t N ) : N(N), adj(N), pot(N,0) {}
	void add_edge( size_t s, size_t t, T cap, T cost ) {
		edge f, r;
		f.s = s; f.t = t; f.cap = cap; f.flow = 0; f.cost =  cost;
		r.s = t; r.t = s; r.cap =   0; r.flow = 0; r.cost = -cost;
		f.di = adj[t].size(); r.di = adj[s].size();
		adj[s].push_back(f);  adj[t].push_back(r);
	}
	void compile_edges() {
		FOR(v,0,N) FOR(i,0,adj[v].size()) { // This has to be done after all
			edge &e = adj[v][i];      // edges are added because vectors can
			e.dual = &adj[e.t][e.di]; // resize and move their contents.
		}
	}
	T Augment( const VE &path ) {
		T push = path[0].cap;
		FOR(i,0,path.size()) push = min(push, path[i].cap);
		FOR(i,0,path.size()) {
			edge &e = *(path[i].dual->dual); // the actual edge, not a copy
			e.cap -= push;  e.dual->cap += push;
			if( e.dual->flow >= push ) e.dual->flow -= push;
			else { e.flow += push - e.dual->flow;  e.dual->flow = 0; }
		}
		return push;
	}
	void ApplyPotential( const VT &delta ) {
		FOR(v,0,N) {
			FOR(i,0,adj[v].size()) {
				adj[v][i].cost += delta[v];
				adj[v][i].dual->cost -= delta[v];
			}
			pot[v] += delta[v];
		}
	}
	/* The following, down to "CancelNegativeCycles", are unnecessary if the
	 * graph is guaranteed to have no negative cycles.
	 * Alternatively, if you compute any maxflow, you can include these, and
	 * run CancelNegativeCycles to find a cost-optimal maxflow. */
	bool dfs_negcycle_r( const size_t rt, VI &par, VE &cycle ) {
		FOR(i,0,adj[rt].size()) {
			edge &e = adj[rt][i];
			if( e.cap == 0 || e.cost >= 0 ) continue;
			size_t v = e.t;
			if( par[v] < N ) { // found a negative cycle!
				size_t fr = 0; while( cycle[fr].s != v ) ++fr;
				cycle = VE( cycle.begin()+fr, cycle.end() );
				cycle.push_back(e);
				return true;
			}
			else if( par[v] == N ) { // unvisited node
				par[v] = rt;  cycle.push_back(e);
				if( dfs_negcycle_r(v,par,cycle) ) return true;
				par[v] = N+1; cycle.pop_back();
			}
		}
		return false;
	}
	bool dfs_negcycle( VE &cycle ) {
		cycle.clear();   VI par(N,N); // state of the DFS
		FOR(v,0,N) if( par[v] == N && dfs_negcycle_r(v,par,cycle) ) return true;
		return false;
	}
	void CancelNegativeCycles() { // only if the graph has negative cycles
		VE cycle;
		while( dfs_negcycle(cycle) )
			Augment(cycle);
	}
	/* The following is unnecessary if the graph is guaranteed to have no
	 * negative-cost edges with positive capacity before MCMF is run. */
	void FixNegativeEdges( size_t SRC ) {
		VT W(N); VI P(N,N); P[ SRC ] = 0;
		FOR(kk,0,N-1) {
			FOR(v,0,N) FOR(i,0,adj[v].size()) {
				if( adj[v][i].cap == 0 ) continue;
				size_t u = adj[v][i].t;  T w = adj[v][i].cost;
				if( P[u] == N || W[v]+w < W[u] ) {
					W[u] = W[v]+w;
					P[u] = v;
				}
			}
		}
		ApplyPotential( W );
	}
	/* The following form the crux of min-cost max-flow, unless you go with a
	 * pure cycle-canceling approach by precomputing a maxflow. */
	void shortest_paths( size_t S, VE &P, VT &W ) {
		DijkPQ Q;  P = VE(N);  W = VT(N,0); // DO initialize Ws to 0!
		FOR(i,0,N) P[i].s = N;  edge x; x.s = x.t = S;
		Q.push( DijkP(0,0) );  VE trv;  trv.push_back(x);
		while( !Q.empty() ) {
			DijkP z = Q.top();  Q.pop();
			edge e = trv[z.second];  size_t v = e.t;
			if( P[v].s != N ) continue;
			W[v] = z.first; P[v] = e;
			FOR(i,0,adj[v].size()) {
				edge &f = adj[v][i];
				if( f.cap == 0 ) continue;
				Q.push( DijkP( W[v]+f.cost, trv.size() ) );
				trv.push_back(f);
			}
		}
	}
	// Note that this returns the total *maximum flow*, not its cost. Use
	// "Cost()" after calling this for that.
	T ComputeMinCostMaxFlow( size_t SRC, size_t DST ) {
		compile_edges();  // we have to do this after all edges are added
		T flow = 0;
		CancelNegativeCycles();          // Only if necessary!
		FixNegativeEdges( SRC );         // Ditto!
		VE P;  VT W;   shortest_paths( SRC, P, W );
		while( P[DST].s != N ) { // while there is a path S->T
			VE ap;
			for( size_t v = DST; v != SRC; v = P[v].s ) ap.push_back(P[v]);
			ap = VE( ap.rbegin(), ap.rend() ); // I love C++ sometimes
			flow += Augment( ap );
			ApplyPotential( W ); // This eliminates negative cycles from ^
			shortest_paths( SRC, P, W );
		}
		return flow;
	}
	T Cost() {
		T c = 0;
		FOR(v,0,N) FOR(i,0,adj[v].size()) {
			edge &e = adj[v][i];
			c += e.flow * (e.cost - pot[e.s] + pot[e.t]);
		}
		return c;
	}
};
// END

#include <iostream>

void test_mcmf_correct() {
	cerr << "test mincost maxflow correctness" << endl;
	{
		mcmf_graph G(7);
		G.add_edge(0,1, 6, 2);  // 1 through here
		G.add_edge(0,2, 8, 3);  // 2 through here
		G.add_edge(1,3, 2, 14);
		G.add_edge(2,3, 2, 10);
		G.add_edge(3,4, 1, 1);  // these are in the min-cut
		G.add_edge(3,5, 2, 3);  // ^ (max-flow = 3)
		G.add_edge(4,6, 10, 0); // no costs here
		G.add_edge(5,6, 10, 0);
		T fl = G.ComputeMinCostMaxFlow(0,6);
		if( fl != 3 ) {
			cerr << "(test #1) Expected maxflow=3, got " << fl << endl;
		}
		T ct = G.Cost();
		if( ct != 49 ) {
			cerr << "(test #1) Expected cost=49, got " << ct << endl;
		}
	}
	{
		mcmf_graph G(5);
		G.add_edge( 0,1, 1, -1 );
		G.add_edge( 1,2, 1, -1 );
		G.add_edge( 2,3, 1, -1 );
		G.add_edge( 3,4, 1, -1 );
		G.add_edge( 4,2, 1, -1 );
		T fl = G.ComputeMinCostMaxFlow(0,2);
		if( fl != 1 ) {
			cerr << "(test #2) Expected maxflow=1, got " << fl << endl;
		}
		T ct = G.Cost();
		if( ct != -5 ) {
			cerr << "(test #2) Expected cost=-5, got " << ct << endl;
		}
	}
	{
		mcmf_graph G(5);
		G.add_edge( 4,3, 1, -1 );
		G.add_edge( 4,2, 1,  1 );
		G.add_edge( 3,1, 1,  1 );
		G.add_edge( 2,1, 1, -4 );
		G.add_edge( 1,0, 1,  0 );
		T fl = G.ComputeMinCostMaxFlow(4,0);
		if( fl != 1 ) {
			cerr << "(test #3) Expected maxflow=1, got " << fl << endl;
		}
		T ct = G.Cost();
		if( ct != -3 ) {
			cerr << "(test #3) Expected mincost=-3, got " << ct << endl;
		}
	}
}

int main() {
	test_mcmf_correct();
	return 0;
}

