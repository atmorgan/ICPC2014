#include <vector>
#include <iostream>

using namespace std;

#define FOR(v,l,u) for( int v = l; v < u; ++v )

typedef signed long long int LL;
typedef LL T;
typedef vector<T> VT;

// BEGIN
// T is a type with +/- operations and identity element '0'.

// Least significant bit of a. Used throughout.
int LSB( int a ) { return a ^ (a & (a-1)); }

// To use it, instantiate it as `BIT(n)' where n is the size of the underlying
// array. The BIT then assumes a value of 0 for every element. Update each
// index individually (with `add') to use a different set of values.
//
// Note that it is assumed that the underlying array has size a power of 2!
// This mostly just simplifies the implementation without any loss in speed.
// Just use the closest power of 2 larger than the max input size. Even if some test
// cases do not test this high, initialization is extremely quick.
//
// The comments below make reference to an array `arry'. This is the underlying
// array. (A is the data stored in the actual tree.)
struct BIT {
	int N;
	VT A;
	BIT( int n ) : N(n), A(N+1,0) {} // n must be a power of 2
	// add v to arry[idx]
	void add( int idx, T v ) {
		for( int i = idx+1; i <= N; i += LSB(i) ) A[i] += v;
	}
	// get sum( arry[0..idx] )
	T sum( int idx ) {
		T ret = 0;
		for( int i = idx+1; i > 0; i -= LSB(i) ) ret += A[i];
		return ret;
	}
	// get sum( arry[l..r] )
	T sum_range( int l, int r ) { return sum(r) - sum(l-1); }
	// Find largest r so that sum( arry[0..r] ) <= thresh
	// This assumes arry[i] >= 0 for all i > 0, for monotonicity.
	// This takes advantage of the specific structure of LSB() to simplify the
	// binary search.
	int largest_at_most( T thresh ) {
		int r = 0, del = N;
		while( del && r <= N ) {
			int q = r + del;
			if( A[q] <= thresh ) {
				r = q;
				thresh -= A[q];
			}
			del /= 2;
		}
		return r-1;
	}
};

// A `range-add'/`index query' BIT
struct BIT_flip {
	BIT A;
	BIT_flip( int n ) : A(n) {}
	// add v to arry[l,r]
	void add( int l, int r, T v ) {
		A.add(l,v);
		A.add(r+1,-v);
	}
	// get arry[idx]
	T query( int idx ) {
		return A.sum(idx);
	}
};

// A `range-add'/`range-query' data structure that uses BITs.
struct BIT_super {
	int N;
	BIT_flip m, b; // linear coefficient, constant coefficient
	BIT_super( int n ) : N(n), m(n), b(n) {}
	// add v to arry[l..r]
	void add( int l, int r, T v ) {
        m.add(l,r,v);         // add slope on active interval
		b.add(l,N,l*(-v));    // subtract contribution from pre-interval
		b.add(r+1,N,(r+1)*v);       // add total contribution to after-interval
	}
	// get sum( arry[0..r] )
	T query( int r ) {
		++r;
        return m.query(r)*r + b.query(r);
	}
	// get sum( arry[l..r] )
	T query_range( int l, int r ) {
		return query(r) - query(l-1);
	}
};

// A 2-dimensional specialization of BITd. (see below)
// What took `nlogn' before now takes `nlog^2(n)'.
struct BIT2 {
	int N1;
	int N2;
	vector<BIT> A;
	BIT2( int n1, int n2 ) : N1(n1), N2(n2) {
		A.resize(N1+1, BIT(n2));
	}
	// add v to arry[x][y]
	void add( int x, int y, T v ) {
        for( int i = x+1; i <= N1; i += LSB(i) ) A[i].add(y,v);
    }
	// get sum( arry[0..x][0..y] ).
	T sum( int x, int y ) {
        T ret = 0;
		for( int i = x+1; i > 0; i -= LSB(i) ) ret += A[i].sum(y);
		return ret;
	}
	// get sum( arry[xL..xH)[yL..yH) ).
	T sum_range( int xL, int yL, int xH, int yH ) {
		return sum(xH,yH) + sum(xL-1,yL-1) - sum(xH,yL-1) - sum(xL-1,yH);
	}
};

// A d-dimensional binary indexed tree
// What took `nlogn' before now takes `nlog^d(n)'.
//
// To construct it, set dims to be the vector of dimensions, and pass
// d <- dims.size().
typedef vector<int> VI;
struct BITd {
	int N;
	int D;
	vector<BITd> A;
	T V;
	BITd( const VI &dims, int d ) : N(dims[d-1]), D(d) {
		if( D == 0 ) V = 0;
		else         A.resize( N+1, BITd( dims, D-1 ) );
	}
	void add( const VI &idx, T v ) {
		if( D == 0 ) V += v;
		for( int i = idx[D-1]+1; i <= N; i += LSB(i) ) A[i].add(idx,v);
	}
	T sum( const VI &idx ) {
		if( D == 0 ) return V;
		T ret = 0;
		for( int i = idx[D-1]+1; i > 0; i -= LSB(i) ) ret += A[i].sum(idx);
		return ret;
	}
	T sum_range( VI lo, VI hi ) {
		FOR(i,0,D) --lo[i];
        // In higher dimensions, we have to use inclusion-exclusion
		int BD = ((int)1) << D;
		T ret = 0;
		FOR(S,0,BD) {
			int sign = 1;
			VI q(lo);
			FOR(b,0,BD) if( (S >> b) & 1 ) {
				q[b] = hi[b];
				sign *= -1;
			}
			ret += sign * sum(q);
		}
		return ret;
	}
};

// END

#include <iostream>

void test_BIT_correct() {
    bool success = true;
    // only two test suites...
	{
		const int N  = (1U << 10); // ~1e3
		const int NN = N*N;        // ~1e6
		BIT tr(NN); // BIT on NN elts
		// Basic tests
		FOR(i,0,NN) tr.add( i, ((T)56)*i*(i+3) );
		VT A(NN+1); // prefix sums, for checking
		A[0] = 0; FOR(i,1,tr.N+1) A[i] = A[i-1] + ((T)56)*(i-1)*(i+2);
		FOR(q,0,NN+1) {
			T result = tr.sum(q-1);
			if( result != A[q] ) {
				success = false;
				cerr << "Error in BIT::add/sum(" << q << "): Expected: " << A[q] << ", Actual: " << result << endl;
			}
		}
		FOR(qL,0,N) FOR(qR,qL,qL+N) {
			T result = tr.sum_range(qL,qR-1);
			if( result != A[qR] - A[qL] ) {
				success = false;
				cerr << "Error in BIT::sum_range(" << qL << "," << qR-1 << "): ";
				cerr << "Expected: " << A[qR] - A[qL] << ", Actual: " << result << endl;
			}
		}
	}
	{
		const int N = (1U << 7);
		BIT tr(N);
		VT A(N,0);
		tr.add( 6, 14 );   A[6]   += 14;
		tr.add( 28, 52 );  A[28]  += 52;
		tr.add( 24, 2 );   A[24]  += 2;
		tr.add( 99, 0 );   A[99]  += 0;
		tr.add( 99, 1 );   A[99]  += 1;
		tr.add( 99, 3 );   A[99]  += 3;
		tr.add( 100, 4 );  A[100] += 4;
		tr.add( 91,4 );    A[91]  += 4;
		VT AS(A.size()+1);
		AS[0] = 0;
		FOR(i,1,AS.size()) AS[i] = AS[i-1] + A[i-1];
		int UB = AS.back();
		FOR(q,0,UB) {
			int result = tr.largest_at_most(q)+1;
			int exp = 0;
			while( exp+1 < AS.size() && AS[exp+1] <= (T)q ) ++exp;
			if( result != exp ) {
				success = false;
				cerr << "Error in BIT::largest_at_most(" << q << "): ";
				cerr << "Expected: " << exp << ", Actual: " << result << endl;
			}
		}
	}
    if (success) {
        cerr << "BIT correct!" << endl;
    }
}

void test_BIT_flip_correct() {
	bool success = true;
	{
		const int N = (1U << 4);
		BIT_flip tr(N);
		VT A(N,0);
		tr.add(3, 8, 16);  FOR(i,3,9)   A[i] += 16;
		tr.add(1, 3, 2 );  FOR(i,1,4)   A[i] += 2;
		tr.add(11,10,99);  FOR(i,11,11) A[i] += 99;
		tr.add(0, 8, 12);  FOR(i,0,9)   A[i] += 12;
		FOR(q,0,N) {
			T result = tr.query( q );
			T exp = A[q];
			if( result != exp ) {
				success = false;
				cerr << "Error in BIT_flip::add/query(" << q << "): ";
				cerr << "Expected: " << exp << ", Actual: " << result << endl;
			}
		}
	}
	if( success ) {
		cerr << "BIT_flip correct!" << endl;
	}
}

void test_BIT_super_correct() {
	bool success = true;
	{
		const int N = (1U << 4);
		BIT_super tr(N);
		VT A(N,0);
		FOR(i,0,N) FOR(j,i,N) {
			T v = (27*i*(i+3)*(j+7)) % 93;
			tr.add(i,j-1,v);
			FOR(k,i,j) A[k] += v;
		}
		VT AS(A.size()+1);
		AS[0] = 0; FOR(i,1,AS.size()) AS[i] = AS[i-1] + A[i-1];
		FOR(qL,0,N) FOR(qR,qL,N) {
			T result = tr.query_range(qL,qR-1);
			T exp = AS[qR] - AS[qL];
			if( result != exp ) {
				success = false;
				cerr << "Error in BIT_super::add/query(" << qL << "," << qR-1 << "): ";
				cerr << "Expected: " << exp << ", Actual: " << result << endl;
			}
		}
	}
	if( success ) {
		cerr << "BIT_super correct!" << endl;
	}
}

void test_BIT2_correct() {
	typedef vector<VT> VVT;
	bool success = true;
	{
		const int N = (1U << 5);
		BIT2 tr(N,N);
		VVT A(N,VT(N,0));
		FOR(i,0,N) FOR(j,0,N) {
			T v = (27*i*(i+3)*(j+7)) % 93;
			tr.add(i,j,v);
			A[i][j] = v;
		}
		FOR(qi1,0,N) FOR(qi2,qi1,N) FOR(qj1,0,N) FOR(qj2,qj1,N) {
			T result = tr.sum_range(qi1,qj1, qi2,qj2);
			T exp = 0;
			// dumb ways are the best ways to test
			FOR(ki,qi1,qi2+1) FOR(kj,qj1,qj2+1) exp += A[ki][kj];
			if( result != exp ) {
				success = false;
				cerr << "Error in BIT2::add/query(" << qi1 << "," << qj1 << "  ->  " << qi2 << "," << qj2 << "): ";
				cerr << "Expected: " << exp << ", Actual: " << result << endl;
            }
		}
	}
	if( success ) {
		cerr << "BIT2 correct!" << endl;
	}
}

void test_BITd_correct() {
	typedef vector<VT> VVT;
	bool success = true;
	{
		const int N1 = (1U << 5), N2 = (1U << 4);
		VI dimn(2); dimn[0] = N1; dimn[1] = N2;
		BITd tr( dimn, dimn.size() );
		VVT A(N1,VT(N2,0));
		FOR(i,0,N1) FOR(j,0,N2) {
			T v = (27*i*(i+3)*(j+7)) % 93;
			VI pos(2); pos[0] = i; pos[1] = j;
			tr.add(pos,v);
			A[i][j] = v;
		}
		FOR(qi1,0,N1) FOR(qi2,qi1,N1) FOR(qj1,0,N2) FOR(qj2,qj1,N2) {
			VI pos1(2), pos2(2);
			pos1[0] = qi1; pos1[1] = qj1;
			pos2[0] = qi2; pos2[1] = qj2;
			T result = tr.sum_range(pos1,pos2);
			T exp = 0;
			// dumb ways are the best ways to test
			FOR(ki,qi1,qi2+1) FOR(kj,qj1,qj2+1) exp += A[ki][kj];
			if( result != exp ) {
				success = false;
				cerr << "Error in BITd::add/query(" << qi1 << "," << qj1 << "  ->  " << qi2 << "," << qj2 << "): ";
				cerr << "Expected: " << exp << ", Actual: " << result << endl;
                return;
            }
		}
	}
	if( success ) {
		cerr << "BITd correct!" << endl;
	}
}

int main() {
    test_BIT_correct();
    test_BIT_flip_correct();
    test_BIT_super_correct();
    test_BIT2_correct();
    test_BITd_correct();
    return 0;
}
