#include <complex>
#include <vector>
#include <algorithm>

using namespace std;

// BEGIN
// Based on implementation at: http://e-maxx.ru/algo/fft_multiply,
// first seen from: http://serjudging.vanb.org/wp-content/uploads/kinversions_hcheng.cc
// Solves UVa Online Judge "Golf Bot" in 2.35 seconds

// Usage:
// f[0...N-1] and g[0..N-1] are numbers, N is a power of 2.
// Want to compute the convolution h, defined by
// h[n] = sum of f[k]g[n-k] (k = 0, ..., N-1).
// Here, the index is cyclic; f[-1] = f[N-1], f[-2] = f[N-2], etc.
// Let F[0...N-1] be FFT(f), and similarly, define G and H.
// The convolution theorem says H[n] = F[n]G[n] (element-wise product).
// To compute h[] in O(N log N) time, do the following:
//   1. Compute F and G (pass d = 1 as the argument).
//   2. Get H by element-wise multiplying F and G
//   3. Get h by taking the inverse FFT (pass d = -1)

#define MAX_N (1 << 3)     // must be power of 2

typedef complex<double> T;
typedef vector<T> VT;
const double PI = 4*atan(1);

void FFT(VT &a, bool invert) {
    size_t n = a.size();

    for (int i=1, j=0; i<n; ++i) {
        int bit = n >> 1;
        for (; j>=bit; bit>>=1)
            j -= bit;
        j += bit;
        if (i < j)
            swap(a[i], a[j]);
    }

    for (int len=2; len<=n; len<<=1) {
        double ang = 2*PI/len * (invert ? -1 : 1);
        T wlen (cos(ang), sin(ang));
        for (int i=0; i<n; i+=len) {
            T w(1);
            for (int j=0; j<len/2; ++j) {
                T u = a[i+j], v = a[i+j+len/2] * w;
                a[i+j] = u + v;
                a[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }

    if(invert)
        for (int i=0; i<n; ++i)
            a[i] /= n;
}
// END

#include <iostream>
#include <iomanip>
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )

int main(void)
{
  cout << "If rows come in identical pairs, then everything works." << endl;
  cout << setprecision(8) << fixed;
  cout << "Test FFT:" << endl;

  const size_t n = MAX_N;
  T _a[n] = {0, 1, T(1,3), T(0,5), 1, 0, 2, 0};
  T _b[n] = {1, T(0,-2), T(0,1), 3, -1, -3, 1, -2};
  VT a(_a, _a+n), b(_b,_b+n);
  VT A = a, B = b;
  FFT(A,false);
  FFT(B,false);
  
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
  FFT(ab,true);
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
