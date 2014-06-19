#include "FloatCompare.cc"
#include <vector>
#include <cmath>
using namespace std;
// BEGIN
// Useful linear algebra routines.
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )
typedef double         T;   // the code below only supports fields
typedef vector<T>      VT;
typedef vector<VT>     VVT;
typedef vector<size_t> VI;
// Given an m-by-n matrix A, compute its reduced row echelon form,
// returning a value like the determinant.
// If m = n, the returned value *is* the determinant of A.
// If m != n, the returned value is nonzero iff A has full row rank.
// To compute rank(A), get its RREF, and count the nonzero rows.
T GaussJordan( VVT &A ) {
	const size_t m = A.size(), n = A[0].size();
	T det = 1;
	size_t p = 0;         // walking pointer for the pivot column
	FOR(k,0,m) {          // for each row...
		while( p < n ) {    // find the best row below k to pull up
			size_t i_max = k; // index of the best row
			FOR(i,k,m) if( fabs(A[i][p]) > fabs(A[i_max][p]) ) i_max = i;
			if( ApproxEq(0.0, A[i_max][p]) ) { // we have out new pivot
				if( i_max != k ) {               // swap if it's not k
					swap( A[i_max], A[k] );
					det *= -1;
				}
				break;                         // don't let p grow to n
			}
			FOR(i,k,m) A[i][p] = 0; // This column is zeros below row k
			++p;                    // So move on to the next column
		}
		if( p == n ) { det = 0; break; } // we're done early
		T s = 1/A[k][p];         // scale the pivot row
		FOR(j,p,n) A[k][j] *= s;
		det *= s;
		// subtract pivot row from the other rows
		FOR(i,0,m) if( i != k ) FOR(j,p,n) A[i][j] -= A[i][p]*A[k][j];
		++p;
	}
	return det;
}
// In-place invert A.
void InvertMatrix( VVT &A ) {
	const size_t n = A.size();
	FOR(i,0,n) FOR(j,0,n) A[i].push_back( (i==j) ? 1 : 0 ); // augment
	GaussJordan( A );                          // compute RREF
	FOR(i,0,n) FOR(j,0,n) A[i][j] = A[i][j+n]; // copy A inverse over
	FOR(i,0,n) A[i].resize(n);                 // get rid of cruft
}
// Given n-by-n A and n-by-q b, compute a vector x with Ax = b.
// The output is returned in x, when it exists. (else, x is empty)
// If there are solutions, return the dimension of the kernel of A
// (which equals the dimension of the affine space of solutions),
// or else return -1 when there are no solutions.
// (There is a unique solution iff we return 0.)
int SolveLinearSystem( const VVT &A, const VVT &b, VVT &x ) {
	const size_t n = A.size(), q = b[0].size();
	x = VVT(A);                                    // copy
	FOR(i,0,n) FOR(j,0,q) x[i].push_back(b[i][j]); // augment
	GaussJordan( x );                              // RREF
	int kerd = 0;
	FOR(i,0,n) { // dim(ker(A)) = # of all-zero rows
		FOR(j,0,n) if( ApproxEq(0.0,x[i][j]) )   goto solexists;
		FOR(j,0,q) if( ApproxEq(0.0,x[i][n+j]) ) goto nosolution;
		++kerd;
	}
	solexists: {
		FOR(i,0,n) FOR(j,0,q) x[i][j] = x[i][n+j];
		FOR(i,0,n) x[i].resize(q);
		return kerd;
	}
	nosolution: {
		x.clear();
		return -1;
	}
}
// END

/*  Old main-- useful for testing
int main() {
	const int n = 4;
	const int m = 2;
	double A[n][n] = { {1,2,3,4},{1,0,1,0},{5,3,2,4},{6,1,4,6} };
	double B[n][m] = { {1,2},{4,3},{5,6},{8,7} };
	VVT a(n), b(n);
	for (int i = 0; i < n; i++) {
		a[i] = VT(A[i], A[i] + n);
		b[i] = VT(B[i], B[i] + m);
	}
	double det = GaussJordan(a, b);
	// expected: 60	
	cout << "Determinant: " << det << endl;
	// expected: -0.233333 0.166667 0.133333 0.0666667
	//					 0.166667 0.166667 0.333333 -0.333333
	//					 0.233333 0.833333 -0.133333 -0.0666667
	//					 0.05 -0.75 -0.1 0.2
	cout << "Inverse: " << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			cout << a[i][j] << ' ';
		cout << endl;
	}
	// expected: 1.63333 1.3
	//					 -0.166667 0.5
	//					 2.36667 1.7
	//					 -1.85 -1.35
	cout << "Solution: " << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			cout << b[i][j] << ' ';
		cout << endl;
	}
}
*/
