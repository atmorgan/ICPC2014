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
	edge *dual;
};
typedef vector<edge>   VE;
typedef vector<VE>     VVE;
typedef pair<T,size_t> DijkP;  // Dijkstra PQ element.
typedef priority_queue<DijkP, vector<DijkP>, std::greater<DijkP> >  DijkPQ;

struct mcmf_graph {
	size_t N;   VVE adj;  VT pot;
	mcmf_graph( size_t N ) : N(N), adj(N), pot(N,0) {}
	~mcmf_graph() { FOR(i,0,N) FOR(j,0,adj[i].size()) delete adj[i][j]; }
	void add_edge( size_t s, size_t t, T cap, T cost ) {
		edge f, r;
		f.s = s; f.t = t; f.cap = cap; f.flow = 0; f.cost =  cost;
		r.s = t; r.t = s; r.cap =   0; r.flow = 0; r.cost = -cost;
		adj[s].push_back(f); adj[t].push_back(r);
		adj[s].dual = &adj[t].back(); adj[t].dual = &adj[s].back();
	}
	void Augment( const VE &path ) {
		T push = path[0].cap;
		FOR(i,0,path.size()) push = min(push, path[i].cap);
		FOR(i,0,path.size()) {
			edge &e = *(path[i].dual->dual); // the actual edge, not a copy
			e.cap -= push;  e.dual->cap += push;
			if( e.dual->flow >= push ) e.dual->flow -= push;
			else { e.flow += push - e.dual->flow;  e.dual->flow = 0; }
		}
	}
	void ApplyPotential( const VT &delta ) {
		FOR(v,0,N) {
			FOR(i,0,adj[v].size()) {
				adj[v][i].cost += delta; // TODO: check signs
				adj[v][i].dual->cost -= delta;
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
			if( (e.cap - e.flow) == 0 || e.cost >= 0 ) continue;
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
	}
	bool dfs_negcycle( VE &cycle ) {
		cycle.clear();   VI par(N,N); // state of the DFS
		FOR(v,0,N) if( par[v] == N && dfs_negcycle_r(v,vis,cycle) ) return true;
		return false;
	}
	void CancelNegativeCyles() { // only if the graph has negative cycles
		VE cycle;
		while( dfs_negcycle(cycle) )
			Augment(cycle);
	}
	/* The following is unnecessary if the graph is guaranteed to have no
	 * negative forward edges in the first place. */
	void FixNegativeEdges() {
		VT W(N); VI P(N,N); P[0] = 0;
		FOR(kk,0,N-1) {
			FOR(v,0,N) FOR(i,0,adj[v].size()) {
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
		Q.push( DijkP(0,x) );
		while( !Q.empty() ) {
			edge e = Q.top().second;
			if( P[e.t].s != N && Q.top().first >= W[ e.t ] ) continue;
			W[ e.t ] = Q.top().first;
			P[ e.t ] = e.s;
			Q.pop();
			FOR(i,0,adj[ e.t ].size()) {
				edge &f = adj[v][i];
				if( f.cap - f.flow == 0 ) continue;
				Q.insert( DijkP( W[e.t]+f.cost, f ) );
			}
		}
	}
	// Note that this returns the total *maximum flow*, not its cost. Use
	// "Cost()" after calling this for that.
	T ComputeMinCostMaxFlow( size_t S, size_t T ) {
		CancelNegativeCycles(); // Only if necessary!
		FixNegativeEdges();     // Ditto!
		VE P;  VT W;   shortest_paths(S,P,W);
		while( P[T].s != N ) { // while there is a path S->T
			VE ap;
			for( size_t v = T; v != S; v = P[v].s ) ap.push_back(P[v]);
			ap = VE( ap.rbegin(), ap.rend() ); // I love C++
			Augment( ap );
			ApplyPotential( W ); // This eliminates negative cycles from ^
		}
	}
	T Cost() {
		T c = 0;
		FOR(v,0,N) FOR(i,0,adj[v].size())
			c += e.flow * (e.cost - pot[e.s] + pot[e.t]);
		return c;
	}
};
// END

