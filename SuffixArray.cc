// BEGIN
#include <algorithm>
using namespace std;

struct prefix_cmp {
	size_t prefix_len;
	size_t *rank;
	size_t n;
	bool operator() (size_t i, size_t j) {
		if( rank[i] != rank[j] ) return rank[i] < rank[j];
		i += prefix_len; j += prefix_len;
		if( i < n && j < n ) return rank[i] < rank[j];
		else                 return i > j;
	}
};

// given `string' of length `n', construct the suffix array in SA
// (SA assumed to have size >= n)
void suffix_array( size_t *string, size_t *SA, size_t n ) {
	size_t * rank[2]; rank[0] = new size_t[n]; rank[1] = new size_t[n];
	for( size_t i = 0; i < n; ++i ) { SA[i] = i; rank[0][i] = string[i]; }
	prefix_cmp cmp; cmp.n = n; cmp.prefix_len = 1;
	for( size_t x = 0;; ) {
		cmp.rank = rank[x];
		sort( SA, SA+n, cmp );
		x ^= 1;
		rank[x][SA[0]] = 0;
		for( size_t i = 0; i < n-1; ++i ) {
			rank[x][ SA[i+1] ] = rank[x][ SA[i] ];
			if( cmp(SA[i], SA[i+1]) ) ++rank[x][ SA[i+1] ];
		}
		cmp.prefix_len *= 2;
		if( rank[x][ SA[n-1] ] == n-1 ) break;
	}
	delete[] rank[0]; delete[] rank[1];
}
// END
