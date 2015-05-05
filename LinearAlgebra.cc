#include <iostream>
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
typedef vector<bool>   VB;
// Given an m-by-n matrix A, compute its reduced row echelon form,
// returning a value like the determinant.
// If m = n, the returned value *is* the determinant of A.
// If m != n, the returned value is nonzero iff A has full row rank.
// To compute rank(A), get its RREF, and count the nonzero rows.
T GaussJordan( VVT &A ) {
	const size_t m = A.size(), n = A[0].size();
	T det = 1;
	size_t pj = 0;        // walking pointer for the pivot column
	FOR(k,0,m) {
		size_t pi = k;
		while( pj < n ) {    // find the best row below k to pivot
			FOR(i,k,m) if( fabs(A[i][pj]) > fabs(A[pi][pj]) ) pi = i;
			if( !feq(0.0, A[pi][pj]) ) { // we have our new pivot
				if( pi != k ) {
					swap( A[pi], A[k] );
					pi = k;
					det *= -1;
				}
				break;
			}
			FOR(i,k,m) A[i][pj] = 0; // This column is zeros below row k
			++pj;                    // So move on to the next column
		}
		if( pj == n ) { det = 0; break; } // we're done early
		T s = 1.0/A[pi][pj];        // scale the pivot row
		FOR(j,pj,n) A[pi][j] *= s;
		det /= s;
		FOR(i,0,m) if( i != pi ) { // subtract pivot from other rows
			T a = A[i][pj];          // multiple of pivot row to subtract
			FOR(j,pj,n) A[i][j] -= a*A[pi][j];
		}
		++pj;
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
// Given m-by-n A and m-by-q b, compute a matrix x with Ax = b.
// This solves q separate systems of equations simultaneously.
// Fix k in [0,q).
// x[*][k] indicates a candidate solution to the jth equation.
// has_sol[k] indicates whether a solution is actually solution.
// The return value is the dimension of the kernel of A.
// Note that this is the dimension of the space of solutions when
// they exist.
size_t SolveLinearSystems( const VVT &A, const VVT &b, VVT &x, VB &has_sol ) {
	const size_t m = A.size(), n = A[0].size(), q = b[0].size();
	VVT M = A;                                     // copy
	FOR(i,0,m) FOR(j,0,q) M[i].push_back(b[i][j]); // augment
	GaussJordan( M );                              // RREF
	x = VVT(n, VT(q, 0));
	size_t i = 0;
	while( i < m ) {
		size_t jz = 0; // find first nonzero column
		while( jz < n && feq(M[i][jz],0) ) ++jz;
		if( jz == n ) break; // all zero means we're starting the kernel
		FOR(k,0,q) x[jz][k] = M[i][n+k]; // first nonzero is always 1
		++i;
	}
	size_t kerd = m - i;
	has_sol = VB(q,true);
	while( i < m ) {
		FOR(k,0,q) if( !feq(M[i][n+k],0) ) has_sol[k] = false;
		++i;
	}
	return kerd;
}
// Given m-by-n A and m-by-1 b, compute a matrix x with Ax=b.
// This solves a single system of equations.
// If a solution exists, x contains one of them;
// otherwise x is empty.
// The return value is the dimension of the kernel of A.
// Note that this is the dimension of the space of solutions.
size_t SolveLinearSystem( const VVT &A, const VT &b, VT &x ) {
	const size_t m = A.size(), n = A[0].size(), q = b[0].size();
	VVT M = A;                       // copy
	FOR(i,0,m) M[i].push_back(b[i]); // augment
	GaussJordan( M );                // RREF
	x = VT(n, 0);
	size_t i = 0;
	while( i < m ) {
		size_t jz = 0; // find first nonzero column
		while( jz < n && feq(M[i][jz],0) ) ++jz;
		if( jz == n ) break; // all zero means we're starting the kernel
		x[jz] = M[i][n]; // first nonzero is always 1
		++i;
	}
	size_t kerd = m - i;
	while( i < m ) {
		if( !feq(M[i][n],0) ) x.clear();
		++i;
	}
	return kerd;
}
// END

#include <iostream>
#include <iomanip>

int main() {
	cout << fixed << setprecision(6);
	const size_t n = 4;
	const size_t m = 2;
	T A[n][n] = { {1,2,3,4},{1,0,1,0},{5,3,2,4},{6,1,4,6} };
	T B[n][m] = { {1,2},{4,3},{5,6},{8,7} };
	//T A[n][n] = { {1,1,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,1} };
	//T B[n][m] = { {1,0},{1,1},{1,1},{1,1} };
	VVT a(n), b(n);
	FOR(i,0,n) a[i] = VT(A[i], A[i]+n);
	FOR(i,0,n) b[i] = VT(B[i], B[i]+m);
	cout << "A:" << endl;
	FOR(i,0,n) {
		FOR(j,0,n) cout << a[i][j] << " ";
		cout << endl;
	}
	cout << "b:" << endl;
	FOR(i,0,n) {
		FOR(j,0,m) cout << b[i][j] << " ";
		cout << endl;
	}
	double det = GaussJordan(a);
	// expected: 60	
	cout << "Determinant: " << det << endl;
	// expected: -0.233333 0.166667 0.133333 0.0666667
	//					 0.166667 0.166667 0.333333 -0.333333
	//					 0.233333 0.833333 -0.133333 -0.0666667
	//					 0.05 -0.75 -0.1 0.2
	FOR(i,0,n) a[i] = VT(A[i], A[i]+n);
	InvertMatrix( a );
	cout << "Inverse: " << endl;
	FOR(i,0,n) {
		FOR(j,0,n) cout << a[i][j] << " ";
		cout << endl;
	}
	// expected: 1.63333 1.3
	//					 -0.166667 0.5
	//					 2.36667 1.7
	//					 -1.85 -1.35
	FOR(i,0,n) a[i] = VT(A[i], A[i]+n);
	VVT x; VB exists;
	size_t k = SolveLinearSystems( a, b, x, exists );
	cout << "Solution (" << k << "-dimensional): " << endl;
	FOR(i,0,m) cout << exists[i] << " "; cout << endl;
	FOR(i,0,n) {
		FOR(j,0,m) cout << x[i][j] << " ";
		cout << endl;
	}
}

