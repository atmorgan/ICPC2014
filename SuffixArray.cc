#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
using namespace std;

// BEGIN
// A prefix-doubling suffix array construction implementation.
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )

typedef vector<size_t> VI;
typedef pair<int, int> II;

#define MAX_N 100010
int RA[MAX_N], tempRA[MAX_N];
int SA[MAX_N], tempSA[MAX_N];
int c[MAX_N];

// uses Radix Sort as a subroutine to sort in O(n)
void CountingSort(int n, int k) {
    int i, sum, maxi = max(300, n);
    memset(c, 0, sizeof c);
    for (i = 0; i < n; i++)
        c[i+k < n ? RA[i+k] : 0]++;
    for (i = sum = 0; i < maxi; i++) {
        int t = c[i]; c[i] = sum; sum += t;
    }
    for (i = 0; i < n; i++)
        tempSA[c[SA[i]+k < n ? RA[SA[i]+k] : 0]++] = SA[i];
    for (i = 0; i < n; i++)
        SA[i] = tempSA[i];
}

// Construct SA in O(n log n) time. Solves UVa Online Judge "Glass Beads" in .5 seconds
void ConstructSA(string T) {
    int i, k, r, n = T.size();
    for (i = 0; i < n; i++) RA[i] = T[i];
    for (i = 0; i < n; i++) SA[i] = i;
    for (k = 1; k < n; k <<= 1) {
        CountingSort(n, k);
        CountingSort(n, 0);
        tempRA[SA[0]] = r = 0;
        for (i = 1; i < n; i++)
            tempRA[SA[i]] =
                (RA[SA[i]] == RA[SA[i-1]] && RA[SA[i]+k] == RA[SA[i-1]+k]) ? r : ++r;
        for (i = 0; i < n; i++)
            RA[i] = tempRA[i];
        if (RA[SA[n-1]] == n-1) break;
    }
}

// Given a "string" w, and suffix array SA, compute the array LCP for which
// the suffix starting at SA[i] matches SA[i+1] for exactly LCP[i] characters
// It is assumed that the last character of w is the unique smallest-rank
// character in w. Runs in O(n).
void LongestCommonPrefix( const string &w, VI &LCP ) {
	const size_t N = w.size();   VI rk(N);
	FOR(i,0,N) rk[ SA[i] ] = i;
	LCP = VI(N-1);   size_t k = 0;
	FOR(i,0,N) {
		if( rk[i] == N-1 ) continue;
		size_t j = SA[ rk[i]+1 ];
		while( w[ i+k ] == w[ j+k ] ) ++k;
		LCP[ rk[i] ] = k;
		if( k > 0 ) --k;
	}
}
// Finds the smallest and largest i such that the prefix of suffix SA[i] matches
// the pattern string P. Returns (-1, -1) if P is not found in T. Runs in O(m log n).
II StringMatching(const string &T, const string P) {
    int n = T.size(), m = P.size();
    int lo = 0, hi = n-1, mid = lo;
    while (lo < hi) {
        mid = (lo + hi) / 2;
        int res = T.compare(SA[mid], m, P);
        if (res >= 0) hi = mid;
        else          lo = mid+1;
    }
    if (T.compare(SA[lo], m, P) != 0) return II(-1, -1);
    II ans; ans.first = lo;
    lo = 0; hi = n-1; mid = lo;
    while (lo < hi) {
        mid = (lo + hi) / 2;
        int res = T.compare(SA[mid], m, P);
        if (res > 0) hi = mid;
        else         lo = mid+1;
    }
    if (T.compare(SA[hi], m, P)) hi--;
    ans.second = hi;
    return ans;
}
// END

#include <iostream>

bool cmp_suffixes( string arry, size_t a, size_t b ) {
	if( a == b ) return false;
	while( max(a,b) < arry.size() && arry[a] == arry[b] ) { ++a; ++b; }
	if( a == arry.size() ) return true;
	if( b == arry.size() ) return false;
	return arry[a] < arry[b];
}

void test_suffix_array_correct() {
	cerr << "test suffix array correctness" << endl;
	{
        string s = "1213112542";
		ConstructSA(s);
		FOR(i,0,9) {
			if( !cmp_suffixes(s,SA[i],SA[i+1]) ) {
				cerr << "suffixes " << SA[i] << " and " << SA[i+1] << " out of order." << endl;
			}
		}
		VI lcp;  LongestCommonPrefix( s, lcp );
		VI rk(s.size()); FOR(i,0,s.size()) rk[ SA[i] ] = i;
		if( lcp[ rk[0] ] != 2 ) {
			cerr << "suffixes 0.. and " << SA[rk[0]+1] << ".. have bad lcp (exp. 2, got " << lcp[rk[0]] << "):" << endl;
			cerr << "\t"; FOR(i,0,s.size()) cerr << s[i]; cerr << endl;
			cerr << "\t"; FOR(i,SA[rk[0]+1],s.size()) cerr << s[i]; cerr << endl;
		}
		if( lcp[ rk[8] ] != 0 ) {
			cerr << "suffixes 8.. and " << SA[rk[8]+1] << ".. have bad lcp (exp. 0, got " << lcp[rk[8]] << "):" << endl;
			cerr << "\t"; FOR(i,8,s.size()) cerr << s[i]; cerr << endl;
			cerr << "\t"; FOR(i,SA[rk[8]+1],s.size()) cerr << s[i]; cerr << endl;
		}

        string T = "GATAGACA$";
        ConstructSA(T);
        
        II ans = StringMatching(T, "GA");
        
        if (ans.first != 6 || ans.second != 7) {
            cerr << "string matching not finding match" << endl;
        }

        II ans2 = StringMatching(T, "asdf");
        if (ans2.first != -1 || ans2.second != -1) {
            cerr << "string match returned false positive on query" << endl;
        }
	}
}

int main() {
	test_suffix_array_correct();
	return 0;
}

