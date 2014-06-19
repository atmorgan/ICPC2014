#include <vector>
#include <algorithm>
using namespace std;
// BEGIN
// A prefix-doubling suffix array construction implementation.
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )
typedef vector<size_t> VI;
struct prefix_cmp {
	size_t prefix_len;    // half the length of prefixes being compared
	VI     rank;          // rank[i] is the rank of the ith prefix
	prefix_cmp() : prefix_len(1) {}
	bool operator() ( size_t i, size_t j ) {
		if( rank[i] != rank[j] ) return rank[i] < rank[j]; // first half
		i += prefix_len; j += prefix_len;                  // second half
		if( i < rank.size() && j < rank.size() ) // prefixes are long.
			return rank[i] < rank[j];
		else return i > j; // prefixes are short, so return the shorter.
	}
};
// given a "string" w, construct the suffix array in SA
void SuffixArray( VI &w, VI &SA ) {
	size_t N = w.size();      SA = VI(N);
	prefix_cmp cmp;   cmp.rank.resize(N);
	FOR(i,0,N) SA[i] = i;          // initially unsorted
	FOR(i,0,N) cmp.rank[i] = w[i]; // (or some suitable conversion)
	for(;;) {
		sort( SA.begin(), SA.end(), cmp );
		VI new_rank(N);
		new_rank[ SA[0] ] = 0;
		FOR(i,1,w.size()) {
			new_rank[ SA[i] ] = new_rank[ SA[i-1] ];
			if( cmp(SA[i-1],SA[i]) ) ++new_rank[ SA[i] ];
		}
		if( new_rank[ SA[N-1] ] == N-1 ) break;
		cmp.prefix_len *= 2;
		cmp.rank = new_rank;
	}
}
// Given a "string" w, and suffix array SA, compute the array LCP for which the
// suffix starting at SA[i] matches SA[i+1] for exactly LCP[i] characters.
// (i = 0, ..., w.size()-1).
// To make this work, it is assumed that the last character of w is the unique
// smallest-rank character in w.
void LongestCommonPrefix( const VI &w, const VI &SA, VI &LCP ) {
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
		SuffixArray( s, sa );
		FOR(i,0,9) {
			if( !cmp_suffixes(s,sa[i],sa[i+1]) ) {
				cerr << "suffixes " << sa[i] << " and " << sa[i+1] << " out of order." << endl;
			}
		}
		VI lcp;  LongestCommonPrefix( s, sa, lcp );
		VI rk(sa.size()); FOR(i,0,sa.size()) rk[ sa[i] ] = i;
		if( lcp[ rk[0] ] != 2 ) {
			cerr << "suffixes 0.. and " << sa[rk[0]+1] << ".. have bad lcp (exp. 2, got " << lcp[rk[0]] << "):" << endl;
			cerr << "\t"; FOR(i,0,s.size()) cerr << s[i]; cerr << endl;
			cerr << "\t"; FOR(i,sa[rk[0]+1],s.size()) cerr << s[i]; cerr << endl;
		}
		if( lcp[ rk[8] ] != 0 ) {
			cerr << "suffixes 8.. and " << sa[rk[8]+1] << ".. have bad lcp (exp. 0, got " << lcp[rk[8]] << "):" << endl;
			cerr << "\t"; FOR(i,8,s.size()) cerr << s[i]; cerr << endl;
			cerr << "\t"; FOR(i,sa[rk[8]+1],s.size()) cerr << s[i]; cerr << endl;
		}
	}
}

int main() {
	test_suffix_array_correct();
	return 0;
}

