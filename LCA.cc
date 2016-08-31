#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

// BEGIN
// Lowest Common Ancestor on a weighted, rooted tree.
// Solves UVa Online Judge "AntsColony" in 2.04 seconds.  
// O(n log n) time and space precomputation, O(log n) LCA query.

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef pair<int, int> II;
typedef vector<II> VII;
typedef vector<VII> VVII;

// computes an integer upper bound of the binary logarithm of n
int log2(int n) {
    int j;
    for (j = 0; (1 << j) < n; ++j);
    return j;
}

// computes the distance from the root to every node (not necessary for LCA)
// and the level of each node in the tree (necessary for LCA)
// expects an adjacency list of the tree, where each entry is (idx, weight)
void get_dist(VVII &tree, VI &dist, VI &L) {
    stack<int> s;
    s.push(0);
    dist[0] = 0;
    L[0] = 0;
    while (!s.empty()) {
        int cur = s.top(); s.pop();
        for (int i = 0; i < tree[cur].size(); ++i) {
            dist[tree[cur][i].first] = dist[cur] + tree[cur][i].second;
            s.push(tree[cur][i].first);
            L[tree[cur][i].first] = L[cur]+1;
        }
    }
}

// uses dynamic programming to preprocess the P table
void preprocess(VVI &P, int N) {
    for (int j = 1; (1 << j) < N; ++j) {
        for (int i = 0; i < N; ++i) {
            if (P[i][j-1] != -1) {
                P[i][j] = P[P[i][j-1]][j-1];
            }
        }
    }
}

// comptues the LCA of p and q, given P, L, and N
int LCA(int p, int q, VVI &P, VI &L, int N) {
    if (L[p] < L[q])
        swap(p, q);

    for (int i = log2(N)-1; i >= 0; --i) {
        if (P[p][i] != -1 && L[P[p][i]] >= L[q])
            p = P[p][i];
    }

    if (p == q)
        return p;

    for (int i = log2(N)-1; i >= 0; --i) {
        if (P[p][i] != P[q][i]) {
            p = P[p][i], q = P[q][i];
        }
    }

    return P[p][0];
}

// END

void test_case(int expected, int actual) {
    cerr << "Expected: " << expected << " Actual: " << actual << endl;
}

void test_LCA_correct() {
    size_t N = 8;
    VVII tree(N);

    tree[0].push_back(II(1, 2));
    tree[1].push_back(II(2, 6));
    tree[1].push_back(II(3, 13));
    tree[1].push_back(II(4, 2));
    tree[0].push_back(II(5, 4));
    tree[5].push_back(II(6, 7));
    tree[6].push_back(II(7, 20));

    VVI P(N, VI(log2(N), -1));
    
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < tree[i].size(); ++j) {
            P[tree[i][j].first][0] = i;
        }
    }

    VI dist(N), L(N);
    get_dist(tree, dist, L);

    preprocess(P, N);

    int a15 = LCA(1, 5, P, L, N);
    test_case(0, a15);

    int a24 = LCA(2, 4, P, L, N);
    test_case(1, a24);

    int a57 = LCA(5, 7, P, L, N);
    test_case(5, a57);

    int a36 = LCA(3, 6, P, L, N);
    test_case(0, a36);
}

int main() {
    test_LCA_correct();

    return 0;
}
