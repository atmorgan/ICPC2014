#include <vector>
#include <iostream>

using namespace std;

// BEGIN
// Segment Tree with lazy propagation. This solves
// AhoyPirates on UVa Online Judge in 1.172 seconds.
// To use, you only need to change SegmentNode data, the merge function, update, and initialization;
// the functions in the are written in such a way as to handle everything else.
// If range updates are not necessary, ignore the updateVal and rangeUpdate function.
// The following implementation is an example for RMQ.

#define MAXN 1000 // the maximum input length of the sequnece, good idea to add 10 or so to this

typedef signed long long int T; // the type of the underlying sequence
typedef vector<size_t> VI;
typedef vector<T> VT;

struct SegmentNode {
    // segment data
    T maxVal;

    // update data
    T updateVal;
};

SegmentNode st[MAXN*4];
T A[MAXN];

size_t left(size_t cur) { return cur << 1; }
size_t right(size_t cur) { return (cur << 1) + 1; }

// create merging function here
void merge(SegmentNode &left, SegmentNode &right, SegmentNode &result) {
    result.maxVal = max(left.maxVal, right.maxVal);
}

// only use if range update is needed
void updateChildren(size_t cur, size_t L, size_t R);

// merge handles all querying, no changes needed here
// note that when calling any segment funcitons in main,
// the first three parameters will be 1, 0, length-1
SegmentNode query(size_t cur, size_t L, size_t R, size_t LQ, size_t RQ) {
    if (L >= LQ && R <= RQ)
        return st[cur];

    updateChildren(cur, L, R);
    size_t M = (L+R)/2;

    if (M < LQ)
        return query(right(cur), M+1, R, LQ, RQ);

    if (M+1 > RQ)
        return query(left(cur), L, M, LQ, RQ);

    SegmentNode leftResult = query(left(cur), L, M, LQ, RQ);
    SegmentNode rightResult = query(right(cur), M+1, R, LQ, RQ);
    SegmentNode result;

    merge(leftResult, rightResult, result);

    return result;
}

// only implement if necessary
void update(size_t cur, size_t L, size_t R, size_t idx, T val) {
    if (L == idx && R == idx) {
        // write update of single value here
        st[cur].maxVal = val;
    }
    else if (L <= idx && R >= idx) {
        size_t M = (L+R)/2;
        update(left(cur), L, M, idx, val);
        update(right(cur), M+1, R, idx, val);

        merge(st[left(cur)], st[right(cur)], st[cur]);
    }
}

// only implement if necessary
void rangeUpdate(size_t cur, size_t L, size_t R, size_t Lbound, size_t Rbound, T val) {
    if (L >= Lbound && R <= Rbound) {
        // implement range update here
        st[cur].maxVal += val;

        // set update vals here
        st[cur].updateVal += val;
    }
    else if (L <= Rbound && R >= Lbound) {
        updateChildren(cur, L, R);

        size_t M = (L+R)/2;
        rangeUpdate(left(cur), L, M, Lbound, Rbound, val);
        rangeUpdate(right(cur), M+1, R, Lbound, Rbound, val);

        merge(st[left(cur)], st[right(cur)], st[cur]);
    }
}

void updateChildren(size_t cur, size_t L, size_t R) {
    rangeUpdate(left(cur), L, R, L, R, st[cur].updateVal);
    rangeUpdate(right(cur), L, R, L, R, st[cur].updateVal);

    // reset update vals
    st[cur].updateVal = 0;
}

void build(size_t cur, size_t L, size_t R) {
    // initialize update vals
    st[cur].updateVal = 0;
    
    if (L == R) {
        // initialize single valiue here
        st[cur].maxVal = A[L];
    }
    else {
        size_t M = (L+R)/2;
        build(left(cur), L, M);
        build(right(cur), M+1, R);

        merge(st[left(cur)], st[right(cur)], st[cur]);
    }
}

// END

void test_case(T expected, T actual) {
    cerr << "Expected: " << expected << " Actual: " << actual << endl;
}

void test_segment_tree_correct() {
    size_t N = 10;
    int cur[] = {0, 42, 3, 4, 5, 983, 48, 28, 59, 1};
    for (size_t i = 0; i < N; ++i) {
        A[i] = cur[i];
    }
    
    build(1, 0, N-1);

    bool testLazy = true;

    cerr << "test segment correctness" << endl;
    if (!testLazy) {
        test_case(query(1, 0, N-1, 0, 3).maxVal, 42);
        test_case(query(1, 0, N-1, 2, 4).maxVal, 5);
        
        update(1, 0, N-1, 1, -10); //42 -> -10
        test_case(query(1, 0, N-1, 0, 3).maxVal, 4);
        test_case(query(1, 0, N-1, 0, 9).maxVal, 983);
        
        update(1, 0, N-1, 5, 80); //983 -> 80
        test_case(query(1, 0, N-1, 0, 9).maxVal, 80);
        test_case(query(1, 0, N-1, 6, 8).maxVal, 59);
    }
    else {
        test_case(query(1, 0, N-1, 0, 3).maxVal, 42);
        test_case(query(1, 0, N-1, 2, 4).maxVal, 5);

        rangeUpdate(1, 0, N-1, 2, 4, 40); //3, 4, 5 -> 43, 44, 45
        test_case(query(1, 0, N-1, 0, 3).maxVal, 44);
        test_case(query(1, 0, N-1, 0, 9).maxVal, 983);

        rangeUpdate(1, 0, N-1, 0, 9, 10); //everything +10
        test_case(query(1, 0, N-1, 0, 9).maxVal, 993);
        test_case(query(1, 0, N-1, 6, 8).maxVal, 69);
    }

}

int main() {
    test_segment_tree_correct();

    return 0;
}
