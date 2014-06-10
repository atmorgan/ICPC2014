// Given a list of numbers of length n, this routine extracts a 
// longest increasing subsequence.
//
// Running time: O(n log n)
//
//   INPUT: a vector of integers
//   OUTPUT: a vector containing the longest increasing subsequence

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// BEGIN
typedef vector<int> VI;
typedef pair<int,int> PII;
typedef vector<PII> VPII;

#define STRICTLY_INCREASNG 0

VI LongestIncreasingSubsequence(VI v) {
    VPII best;
    VI dad(v.size(), -1);
  
    for (int i = 0; i < v.size(); i++) {
        #ifdef STRICTLY_INCREASNG
            PII item = make_pair(v[i], 0);
            VPII::iterator it = lower_bound(best.begin(), best.end(), item);
            item.second = i;
        #else
            PII item = make_pair(v[i], i);
            VPII::iterator it = upper_bound(best.begin(), best.end(), item);
        #endif
        if (it == best.end()) {
            dad[i] = (best.size() == 0 ? -1 : best.back().second);
            best.push_back(item);
        } else {
            dad[i] = dad[it->second];
            *it = item;
        }
    }
  
    VI ret;
    for (int i = best.back().second; i >= 0; i = dad[i])
        ret.push_back(v[i]);
    reverse(ret.begin(), ret.end());
    return ret;
}

int main() {
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    int n;
    VI v;
    scanf("%d", &n);
    while (n--) {
      int k;
      scanf("%d", &k);
      v.push_back(k);
    }
    VI ret = LongestIncreasingSubsequence(v);
    for (int i = 0; i < ret.size(); i++)
        printf("%d ", ret[i]);
}
// END
