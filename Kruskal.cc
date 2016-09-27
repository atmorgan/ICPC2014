#ifndef KRUSKAL_CC
#define KRUSKAL_CC
#include <algorithm>
#include "UnionFind.cc"

using namespace std;

// BEGIN
// Kruskal's algorithm to return MST using
// Union-Find data structure.

typedef pair<size_t, size_t> ii;
typedef pair<double, ii> dii;
typedef vector<dii> vdii;

// edges is a list of all edges in the graph, n is number
// of vertices in the graph
double kruskal(vdii &edges, size_t n) {
    sort(edges.begin(), edges.end());

    VI uf(n);
    for (size_t i = 0; i < n; ++i)
        uf[i] = i;
    
    double cost = 0;
    for (size_t i = 0; i < edges.size(); ++i) {
        size_t u = edges[i].second.first;
        size_t v = edges[i].second.second;

        if (find(uf, u) != find(uf, v)) {
            merge(uf, u, v);
            cost += edges[i].first;
            // if MST edges used are needed, add them here
        }
    }

    return cost;
}

// END

#ifdef BUILD_TEST_KRUSKAL
int main() {
    //TODO: implement tests
    //TODO: make Kruskal return edges used
    //successfully tested on UVa Online Judge Anti Brute Force Lock
}
#endif // BUILD_TEST_KRUSKAL
#endif // KRUSKAL_CC
