#include <vector>
#include <string>
using namespace std;
// BEGIN
// An implemention of Knuth-Morris-Pratt substring-finding.
// The table constructed with KMP_table may have other uses.
typedef vector<size_t> VI;
// In the KMP table, T[i] is the *length* of the longest *prefix*
// which is also a *proper suffix* of the first i characters of w.
void KMP_table( string &w, VI &T ) {
	T = VI( w.size()+1 );
	size_t i = 2, j = 0;
	T[1] = 0; // T[0] is undefined
	while( i < w.size() ) {
		if(  w[i-1] == w[j]  ) { T[i] = j+1; ++i; ++j; } // extend previous
		else if( j > 0 )       { j = T[j]; }             // fall back
		else                   { T[i] = 0; ++i; }        // give up
	}
}
// Search for first occurrence of q in s in O(|q|+|s|) time.
size_t KMP( string &s, string &q ) {
	size_t m, z;   m = z = 0; // m is the start, z is the length so far
	VI T; KMP_table(q, T);    // init the table
	while( m+z < s.size() ) { // while we're not running off the edge...
		if( q[z] == s[m+z] ) {  // next character matches
			++z;
			if( z == q.size() ) return m; // we're done
		}
		else if( z > 0 ) {      // fall back to the next best match
			m += z - T[z];   z = T[z];
		}
		else {                  // go back to start
			m += 1;          z = 0;
		}
	}
	return s.size();
}
// END

#include <iostream>

void test_KMP_correct() {
  string a = (string)"The example above illustrates the general technique for assembling "+
    "the table with a minimum of fuss. The principle is that of the overall search: "+
    "most of the work was already done in getting to the current position, so very "+
    "little needs to be done in leaving it. The only minor complication is that the "+
    "logic which is correct late in the string erroneously gives non-proper "+
    "substrings at the beginning. This necessitates some initialization code.";
  string b1 = "table";
  size_t p = KMP(a, b1);
  if( p != 71 ) {
		cerr << "(test #1) KMP failed." << endl;
  }
  string b2 = "nonexistentthingy";
  p = KMP(a, b2);
  if( p != a.size() ) {
		cerr << "(test #2) KMP failed." << endl;
  }
}

int main() {
	test_KMP_correct();
	return 0;
}

