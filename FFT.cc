#include <complex>
#include <vector>

using namespace std;

#define FOR(v,l,u) for( size_t v = l; v < u; ++v )

// Implementation notes:
//
// - FFT_r uses arrays (as opposed to vectors), since that lets us do pointer
// arithmetic without needing to think about iterators. This makes the code
// quite simple overall.
//
// - FFT is just a wrapper that lets us think in vectors even though the FFT
// is implemented with arrays.

// BEGIN
// Based heavily off the implementation found at
// http://web.stanford.edu/~liszt90/acm/notebook.html#file16

// Usage:
// f[0...N-1] and g[0..N-1] are numbers
// Want to compute the convolution h, defined by
// h[n] = sum of f[k]g[n-k] (k = 0, ..., N-1).
// Here, the index is cyclic; f[-1] = f[N-1], f[-2] = f[N-2], etc.
// Let F[0...N-1] be FFT(f), and similarly, define G and H.
// The convolution theorem says H[n] = F[n]G[n] (element-wise product).
// To compute h[] in O(N log N) time, do the following:
//   1. Compute F and G (pass d = 1 as the argument).
//   2. Get H by element-wise multiplying F and G.
//   3. Get h by taking the inverse FFT (pass d = -1)

// Example situation:
// You have polynomials p(x) and q(x), stored as the coefficients of
// the powers of x, and you want to compute (pq)(x) in the same format.
// This is the same as the convolution of the coefficient vectors.
// Let N be so that N is a power of 2 and N/2 >= max(deg(p),deg(q)).
// Zero-pad the coefficients of p and q to have size N.
// Now convolve p and q: compute FFT(p) and FFT(q). Multiply component-wise.
// compute FFT^{-1}( result ). This is pq.
//
// In code:
// FFT(p,1); FFT(q,1); FOR(i,0,N) pq[i] = p[i]q[i]; FFT(pq,-1);

typedef complex<double> T;
typedef vector<T> VT;

const double PI = 4*atan(1);

void FFT_r( T *A, T *B, size_t p, size_t n, int d ) {
	if( n == 1 ) { B[0] = A[0]; return; }
	FFT_r( A  , B    , 2*p, n/2, d );
	FFT_r( A+p, B+n/2, 2*p, n/2, d );
	FOR(k,0,n/2) {
		T w = polar(1.0, 2*PI*k/n*d);
		T even = B[k], odd = B[k+n/2];
		B[k    ] = even + w * odd;
		B[k+n/2] = even - w * odd;
	}
}
void FFT( VT &_A, int d ) {
	const size_t n = _A.size();
	T *A = new T[n], *B = new T[n];
	FOR(i,0,n) A[i] = _A[i];
	FFT_r( A, B, 1, n, d );
	FOR(i,0,n) _A[i] = B[i];
	delete[] A;
	delete[] B;
	if( d < 0 ) FOR(i,0,n) _A[i] /= n;
}
// END

#include <iostream>
#include <iomanip>

int main(void)
{
  cout << "If rows come in identical pairs, then everything works." << endl;
  cout << setprecision(8) << fixed;
  
  const size_t n = 8;
  T _a[n] = {0, 1, T(1,3), T(0,5), 1, 0, 2, 0};
  T _b[n] = {1, T(0,-2), T(0,1), 3, -1, -3, 1, -2};
  VT a(_a, _a+n), b(_b,_b+n);
  VT A = a, B = b;
  FFT(A,1);
  FFT(B,1);
  
  // Test convolution
  cout << A[0]; FOR(i,1,n) cout << " " << A[i]; cout << endl;
  FOR(i,0,n) {
    T Ai = 0;
    FOR(j,0,n) Ai += a[j] * polar(1.0, j*i*2*PI/n);
    if( i > 0 ) cout << " ";
    cout << Ai;
  }
  cout << endl;
  
  // Test convolution theorem/inverse FFT
  VT AB(n);
  FOR(i,0,n) AB[i] = A[i] * B[i];
  VT ab(AB);
  FFT(ab,-1);
  cout << ab[0]; FOR(i,1,n) cout << " " << ab[i]; cout << endl;
  FOR(i,0,n) {
    T aconvbi = 0;
    FOR(j,0,n) aconvbi += a[j] * b[(n + i - j) % n];
    if( i > 0 ) cout << " ";
    cout << aconvbi;
  }
  cout << endl;
  
  return 0;
}
