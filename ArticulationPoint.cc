#include <vector>
#include <algorithm>
using namespace std;
// BEGIN
// This is code for computing articulation points of graphs,
// ie points whose removal increases the number of components in the graph.
// This works when the given graph is not necessarily connected, too.
typedef vector<size_t> VI;
typedef vector<VI>     VVI;
typedef vector<bool>   VB;
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )

struct artpt_graph {
	size_t N;     VVI adj;        // basic graph stuff
	VI  parent, n_children, rank; // dfs tree
	VB  is_art;   VI reach;       // articulation points
	artpt_graph( size_t N ) : N(N), adj(N), is_art(N) {}
	void add_edge( size_t s, size_t t ) {
		adj[s].push_back(t);
		adj[t].push_back(s);
	}
	size_t dfs_artpts( size_t rt, VB &visited, size_t R ) {
		visited[rt] = true;
		rank[rt] = R++;
		reach[rt] = rank[rt]; // reach[rt] <= rank[rt] always.
		FOR(i,0,adj[rt].size()) {
			size_t v = adj[rt][i];
			if( v == parent[rt] ) continue;
			if( visited[v] )
				reach[rt] = min(reach[rt], rank[v]);
			else {
				++n_children[rt];
				parent[v] = rt;
				R = dfs_artpts( v, visited, R );
				reach[rt] = min(reach[rt], reach[v]);
			}
		}
		if( reach[rt] < rank[rt] || n_children[rt] == 0 )
			is_art[rt] = false;
		return R;
	}
	void comp_articulation_points() {
		is_art = VB(N, true);  reach = VI(N);
		parent = VI(N,N);      rank = VI(N);      n_children = VI(N,0);
		VB visited(N,false);   size_t R = 0;
		FOR(i,0,N) {
			if( visited[i] ) continue;
			R = dfs_artpts(i, visited, R); // this is not right on i
			is_art[i] = (n_children[i] >= 2); // but we can fix it!
		}
	}
};
// END

#include <iostream>

void test_artpts_correct() {
	cerr << "test correctness" << endl;
	{
		artpt_graph G(5);
		FOR(v,1,5) G.add_edge(0,v);
		G.comp_articulation_points();
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
		artpt_graph G(3);
		FOR(v,0,3) G.add_edge(v, (v+1)%3);
		G.comp_articulation_points();
		size_t bad_ct = 0;
		FOR(v,0,3) if( G.is_art[v] ) ++bad_ct;
		if( bad_ct > 0 ) {
			cerr << "(test #2) algo. says " << bad_ct << " extra articulation points." << endl;
		}
	}
	{
		artpt_graph G(3);
		FOR(v1,0,3) FOR(v2,v1+1,3) G.add_edge(v1,v2);
		G.comp_articulation_points();
		size_t bad_ct = 0;
		FOR(v,0,3) if( G.is_art[v] ) ++bad_ct;
		if( bad_ct > 0 ) {
			cerr << "(test #3) algo. says " << bad_ct << " extra articulation points." << endl;
		}
	}
	{
		artpt_graph G(4);
		G.add_edge(3,2); G.add_edge(2,0); G.add_edge(3,1);
		G.comp_articulation_points();
		size_t bad_ct1 = 0, bad_ct2 = 0;
		if( G.is_art[0] ) ++bad_ct1;
		if( G.is_art[1] ) ++bad_ct1;
		if( !G.is_art[2] ) ++bad_ct2;
		if( !G.is_art[3] ) ++bad_ct2;
		if( bad_ct1+bad_ct2 > 0 ) {
			cerr << "(test #4) algo. is wrong about " << bad_ct1 << " extra + " << bad_ct2 << " missed vertices." << endl;
		}
	}
}

void test_artpts_speed() {
	const size_t N = 100000, D = 30; // 1e6
	cerr << "Start speed test... N = " << N << ", D = " << D << endl;
	artpt_graph *G = new artpt_graph(N);
	FOR(d,1,1+D) FOR(i,0,N) G->add_edge( i, (d*(i+1)) % N );
	G->comp_articulation_points();
	cerr << "End speed test." << endl;
	size_t bad_ct = 0;
	FOR(v,0,N) if( G->is_art[v] ) ++bad_ct;
	cerr << bad_ct << " incorrect articulation points." << endl;
	delete G;
}

int main() {
	/*
	artpt_graph G(5);
	FOR(v,1,5) G.add_edge(0,v);
	artpt_graph G(4);
	G.add_edge(3,2); G.add_edge(2,0); G.add_edge(3,1);
	G.comp_articulation_points();
	cerr << "is_art:"; FOR(v,0,4) cerr << " " << G.is_art[v]; cerr << endl;
	cerr << "parent:"; FOR(v,0,4) cerr << " " << G.parent[v]; cerr << endl;
	cerr << "reach: "; FOR(v,0,4) cerr << " " << G.reach[v];  cerr << endl;
	cerr << "rank:  "; FOR(v,0,4) cerr << " " << G.rank[v];   cerr << endl;
	*/
	test_artpts_correct();
	test_artpts_speed();
	return 0;
}

