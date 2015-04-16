#include <vector>
#include <iostream>

using namespace std;

// BEGIN

typedef signed long long int T;

//binary indexed tree - taken from Stanford ACM
#define LOGSZ 20

// remember, BIT is 1-based indexing!
T ft[(1<<LOGSZ)+1];
size_t N = (1<<LOGSZ);

// add v to value at x
void adjust(size_t x, T v) {
    while(x <= N) {
        ft[x] += v;
        x += (x & -x);
    }
}

// get cumulative sum up to and including x
T rsq(size_t x) {
    T res = 0;
    while(x) {
        res += ft[x];
        x -= (x & -x);
    }
    return res;
}

// get largest index with cumulative sum less than or equal to x;
T getLind(T x) {
    size_t idx = 0, mask = N;
    while(mask && idx < N) {
        size_t t = idx + mask;
        if(x >= ft[t]) {
            idx = t;
            x -= ft[t];
        }
        mask >>= 1;
    }
    return idx;
}

// get smallest index with cumulative sum greater than or equal to x;
T getSind(T x) {
    return getLind(x-1)+1;
}

// END

void test_BIT_correct() {
    memset(ft, 0, sizeof(ft));
    adjust(3, 4);
    adjust(5, -2);
    adjust(10, 3);
    T result1 = rsq(8);
    bool success = true;
    if (result1 != 2) {
        success = false;
        cerr << "Error in BIT, Expected: 2, Actual: " << result1 << endl;
    }

    T result2 = getSind(5);
    if (result2 != 10) {
        success = false;
        cerr << "Error in BIT, Expected: 10, Actual: " << result2 << endl;
    }

    if (success) {
        cerr << "BIT correct!" << endl;
    }
}

int main() {
    test_BIT_correct();
    return 0;
}
