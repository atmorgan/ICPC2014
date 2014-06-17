#include <vector>
#include <algorithm>
using namespace std;
// BEGIN
// A prefix-doubling suffix array construction implementation.
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )
typedef vector<size_t> VI;
struct prefix_cmp {
	size_t  prefix_len;    // half the length of prefixes being compared
	VI     &rank;          // rank[i] is the rank of the ith prefix
	size_t  n;             // the length of the underlying string
	prefix_cmp( VI &r ) : prefix_len(1), rank(r), n(r.size()) {}
	bool operator() ( size_t i, size_t j ) {
		if( rank[i] != rank[j] ) return rank[i] < rank[j]; // first half
		i += prefix_len; j += prefix_len;                  // second half
		if( i < n && j < n ) return rank[i] < rank[j];
		else                 return i > j;
	}
};
// given `w' of length `n', construct the suffix array in SA
// (SA assumed to have size >= n)
void suffix_array( VI &w, VI &SA ) {
	size_t N = w.size();   VI rank(N);
	FOR(i,0,N) SA[i] = i;      // initially unsorted
	FOR(i,0,N) rank[i] = w[i]; // or some suitable conversion
	prefix_cmp cmp( rank );
	for(;;) {
		sort( SA.begin(), SA.end(), cmp );
		VI new_rank(N);
		new_rank[ SA[0] ] = 0;
		for( size_t i = 1; i < w.size(); ++i ) {
			new_rank[ SA[i] ] = new_rank[ SA[i-1] ];
			if( cmp(SA[i-1],SA[i]) ) ++new_rank[ SA[i] ];
		}
		if( new_rank[ SA[N-1] ] == N-1 ) break;
		cmp.prefix_len *= 2;
		rank = new_rank;
	}
}
// END

#include <iostream>

bool cmp_suffixes( VI arry, size_t a, size_t b ) {
	if( a == b ) return false;
	while( max(a,b) < arry.size() && arry[a] == arry[b] ) { ++a; ++b; }
	if( a == arry.size() ) return true;
	if( b == arry.size() ) return false;
	return arry[a] < arry[b];
}

void test_suffix_array_correct() {
	cerr << "test suffix array correctness" << endl;
	{
		const size_t _s[] = { 1, 2, 1, 3, 1, 1, 2, 5, 4, 2 };
		VI s( _s, _s+10 ), sa(10);
		suffix_array( s, sa );
		FOR(i,0,9) {
			if( !cmp_suffixes(s,sa[i],sa[i+1]) ) {
				cerr << "suffixes " << sa[i] << " and " << sa[i+1] << " out of order." << endl;
			}
		}
	}
}

int main() {
	test_suffix_array_correct();
	return 0;
}

