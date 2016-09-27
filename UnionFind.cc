#ifndef UNIONFIND_CC
#define UNIONFIND_CC
#include <vector>

using namespace std;

// BEGIN
// UnionFind data structure that implements
// path compression, stolen from stanfordacm
// should not stackoverflow in correcty configured environment
// (ulimit -s BIGNUMBER)

typedef vector<size_t> VI;

// Union find is now a vector of integers, C[i] = parent(i).
// Initialize with C[i] = i

int find(VI &C, size_t x) { return (C[x] == x) ? x : C[x] = find(C, C[x]); }
void merge(VI &C, size_t x, size_t y) { C[find(C, x)] = find(C, y); }

// END

#ifdef BUILD_TEST_UF
int main() {
    //TODO: implement tests
    // successfully tested on UVa Online Judge Anti Brute Force Lock
    return 0;
}
#endif // BUILD_TEST_UNIONFIND
#endif // UNIONFIND_CC
