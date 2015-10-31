#ifndef UNIONFIND_CC
#define UNIONFIND_CC
#include <vector>

using namespace std;

// BEGIN
// UnionFind data structure that implements
// path compression and weighted-union heuristic
// (add smaller lists into larger). Implemented
// after Stanford's implementation gave stack overflow.

typedef vector<size_t> VI;

struct UF {
    VI p;
    VI size;

    UF(size_t n) {
        p = VI(n);
        size = VI(n, 1);
        for (size_t i = 0; i < n; ++i) {
            p[i] = i;
        }
    }

    size_t find(size_t i) {
        if (p[i] == i) return i;
        return p[i] = find(p[i]);
    }

    void merge(size_t i, size_t j) {
        if (size[find(i)] >= size[find(j)]) {
            p[find(j)] = find(i);
            size[find(i)] += size[find(j)];
        }
        else {
            merge(j, i);
        }
    }
};

// END

#ifdef BUILD_TEST_UF
int main() {
    //TODO: implement tests
    //This code has successfully been used and accepted on Southeast USA
    //2012 ACM-ICPC Regional Tsunami
    return 0;
}
#endif // BUILD_TEST_UNIONFIND
#endif // UNIONFIND_CC
