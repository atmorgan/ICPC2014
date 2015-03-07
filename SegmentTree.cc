#include <vector>
#include <iostream>

using namespace std;

// BEGIN

typedef signed long long int T;
typedef vector<size_t> VI;
typedef vector<T> VT;

struct SegmentNode {
    size_t L, R;

    // segment data
    T maxVal;

    // update data
    T updateVal;

    SegmentNode(size_t L, size_t R, T maxVal) : L(L), R(R), maxVal(maxVal) {
        // initialize update values to no updates necessary
        updateVal = 0;
    }

    SegmentNode() {}
};

struct SegmentTree {
    vector<SegmentNode> st;

    size_t left(size_t cur) { return cur << 1; }
    size_t right(size_t cur) { return (cur << 1) + 1; }

    // create merging function here
    SegmentNode merge(SegmentNode &left, SegmentNode &right) {
        return SegmentNode(left.L, right.R, max(left.maxVal, right.maxVal));
    }

    // merge handles all querying, no changes needed here
    SegmentNode query(size_t cur, size_t LQ, size_t RQ) {
        if (st[cur].L >= LQ && st[cur].R <= RQ)
            return st[cur];

        updateChildren(cur);

        if (st[left(cur)].R < LQ)
            return query(right(cur), LQ, RQ);

        if (st[right(cur)].L > RQ)
            return query(left(cur), LQ, RQ);

        SegmentNode leftResult = query(left(cur), LQ, RQ);
        SegmentNode rightResult = query(right(cur), LQ, RQ);

        return merge(leftResult, rightResult);
    }

    // do not use if rangeUpdate is needed
    void update(size_t cur, size_t idx, T val) {
        if (st[cur].L == idx && st[cur].R == idx) {
            // write update of single value here
            st[cur].maxVal = val;
        }
        else if (st[cur].L <= idx && st[cur].R >= idx) {
            update(left(cur), idx, val);
            update(right(cur), idx, val);

            st[cur] = merge(st[left(cur)], st[right(cur)]);
        }
    }

    // only implement if necessary
    void rangeUpdate(size_t cur, size_t Lbound, size_t Rbound, T val) {
        if (st[cur].L >= Lbound && st[cur].R <= Rbound) {
            // implement range update here
            st[cur].maxVal += val;

            // set update vals here
            st[cur].updateVal += val;
        }
        else if (st[cur].L <= Rbound && st[cur].R >= Lbound) {
            updateChildren(cur);

            rangeUpdate(left(cur), Lbound, Rbound, val);
            rangeUpdate(right(cur), Lbound, Rbound, val);

            st[cur] = merge(st[left(cur)], st[right(cur)]);
        }
    }

    void updateChildren(size_t cur) {
        rangeUpdate(left(cur), st[cur].L, st[cur].R, st[cur].updateVal);
        rangeUpdate(right(cur), st[cur].L, st[cur].R, st[cur].updateVal);

        // reset update vals
        st[cur].updateVal = 0;
    }

    void build(size_t cur, size_t L, size_t R, VT &A) {
        if (L == R) {
            // initialize single value here
            st[cur] = SegmentNode(L, R, A[L]);
        }
        else {
            build(left(cur), L, (L+R)/2, A);
            build(right(cur), (L+R)/2+1, R, A);

            st[cur] = merge(st[left(cur)], st[right(cur)]);
        }
    }

    SegmentTree(VT &A) {
        st = vector<SegmentNode>(A.size()*4);
        build(1, 0, A.size()-1, A);
    }
};
// END

void test_case(T expected, T actual) {
    cerr << "Expected: " << expected << " Actual: " << actual << endl;
}

void test_segment_tree_correct() {
    size_t N = 10;
    T vals[] = {0, 42, 3, 4, 5, 983, 48, 28, 59, 1};

    VT A(vals, vals+N);

    SegmentTree tree(A);

    bool testLazy = true;

    cerr << "test segment tree correctness" << endl;
    if (!testLazy) {
        test_case(tree.query(1, 0, 3).maxVal, 42);
        test_case(tree.query(1, 2, 4).maxVal, 5);
        
        tree.update(1, 1, -10); //42 -> -10
        test_case(tree.query(1, 0, 3).maxVal, 4);
        test_case(tree.query(1, 0, 9).maxVal, 983);
        
        tree.update(1, 5, 80); //983 -> 80
        test_case(tree.query(1, 0, 9).maxVal, 80);
        test_case(tree.query(1, 6, 8).maxVal, 59);
    }
    else {
        test_case(tree.query(1, 0, 3).maxVal, 42);
        test_case(tree.query(1, 2, 4).maxVal, 5);

        tree.rangeUpdate(1, 2, 4, 40); //3, 4, 5 -> 43, 44, 45
        test_case(tree.query(1, 0, 3).maxVal, 44);
        test_case(tree.query(1, 0, 9).maxVal, 983);

        tree.rangeUpdate(1, 0, 9, 10); //everything +10
        test_case(tree.query(1, 0, 9).maxVal, 993);
        test_case(tree.query(1, 6, 8).maxVal, 69);
    }

}

int main() {
    test_segment_tree_correct();

    return 0;
}
