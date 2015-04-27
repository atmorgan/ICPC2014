#include <vector>

using namespace std;

#define FOR(v,l,u) for( size_t v = l; v < u; ++v )

typedef signed long long int LL;
typedef LL T;
typedef vector<T> VT;

// BEGIN
// T is a type with +/- operations and identity element '0'.

// Least significant bit of a. Used throughout.
size_t LSB( size_t a ) { return a ^ (a & (a-1)); }

// To use it, instantiate it as `BIT(n)' where n is the size of the underlying
// array. The BIT then assumes a value of 0 for every element. Update each
// index individually (with `add') to use a different set of values.
//
// Note that it is assumed that the underlying array has size a power of 2!
// This mostly just simplifies the implementation without any loss in speed.
//
// The comments below make reference to an array `arry'. This is the underlying
// array. (A is the data stored in the actual tree.)
struct BIT {
	size_t N;
	VT A;
	BIT( size_t n ) : N(n), A(N+1,0) {}
	// add v to arry[idx]
	void add( size_t idx, T v ) {
		for( size_t i = idx+1; i <= N; i += LSB(i) ) A[i] += v;
	}
	// get sum( arry[0..idx) )
	T sum( size_t idx ) {
		T ret = 0;
		for( size_t i = idx; i > 0; i -= LSB(i) ) ret += A[i];
		return ret;
	}
	// get sum( arry[l..r) )
	T sum_range( size_t l, size_t r ) { return sum(r) - sum(l); }
	// Find largest r so that sum( arry[0..r) ) <= thresh
	// This assumes arry[i] >= 0 for all i > 0, for monotonicity.
	// This takes advantage of the specific structure of LSB() to simplify the
	// binary search.
	size_t largest_at_most( T thresh ) {
		size_t r = 0, del = N;
		while( del && r <= N ) {
			size_t q = r + del;
			if( A[q] <= thresh ) {
				r = q;
				thresh -= A[q];
			}
			del /= 2;
		}
		return r;
	}
};

// A `range-add'/`index query' BIT
struct BIT_flip {
	BIT A;
	BIT_flip( size_t n ) : A(n) {}
	// add v to arry[l,r)
	void add( size_t l, size_t r, T v ) {
		A.add(l,v);
		A.add(r,-v);
	}
	// get arry[idx]
	T query( size_t idx ) {
		return A.sum(idx+1);
	}
};

// A `range-add'/`range-query' data structure that uses BITs.
struct BIT_super {
	size_t N;
	BIT_flip m, b; // linear coefficient, constant coefficient
	BIT_super( size_t n ) : N(n), m(n), b(n) {}
	// add v to arry[l..r)
	void add( size_t l, size_t r, T v ) {
		m.add(l,r,v);         // add slope on active interval
		b.add(l,N,l*(-v));    // subtract contribution from pre-interval
		b.add(r,N,r*v);       // add total contribution to after-interval
	}
	// get sum( arry[0..r) )
	T query( size_t r ) {
		return m.query(r)*r + b.query(r);
	}
	// get sum( arry[l..r) )
	T query_range( size_t l, size_t r ) {
		return query(r) - query(l);
	}
};

// A 2-dimensional specialization of BITd. (see below)
// What took `nlogn' before now takes `nlog^2(n)'.
// Feel free to replace `BIT' with BIT_flip or BIT_super
// (just remember to change the interface accordingly)
struct BIT2 {
	size_t N1;
	size_t N2;
	vector<BIT> A;
	BIT2( size_t n1, size_t n2 ) : N1(n1), N2(n2) {
		FOR(i,0,N1) A.push_back( BIT(n2) );
	}
	// add v to arry[x][y]
	void add( size_t x, size_t y, T v ) {
		for( size_t i = x+1; i <= N1; i += LSB(i) ) A[i].add(y,v);
	}
	// get sum( arry[0..x)[0..y) ).
	T sum( size_t x, size_t y ) {
		T ret = 0;
		for( size_t i = x; i > 0; i -= LSB(i) ) ret += A[i].sum(y);
		return ret;
	}
	// get sum( arry[xL..xH)[yL..yH) ).
	T sum_range( size_t xL, size_t yL, size_t xH, size_t yH ) {
		return sum(xH,yH) + sum(xL,yL) - sum(xH,yL) - sum(xL,yH);
	}
};

// A d-dimensional binary indexed tree
// What took `nlogn' before now takes `nlog^d(n)'.
//
// To construct it, set dims to be the vector of dimensions, and pass
// d <- dims.size().
typedef vector<size_t> VI;
struct BITd {
	size_t N;
	size_t D;
	vector<BITd> A;
	T V;
	BITd( const VI &dims, size_t d ) : N(dims[d-1]), D(d) {
		if( D == 0 ) V = 0;
		else         A = vector<BITd>( N+1, BITd( dims, D-1 ) );
	}
	void add( const VI &idx, T v ) {
		if( D == 0 ) V += v;
		for( size_t i = idx[D-1]+1; i <= N; i += LSB(i) ) A[i].add(idx,v);
	}
	T sum( const VI &idx ) {
		if( D == 0 ) return V;
		T ret = 0;
		for( size_t i = idx[D-1]; i > 0; i -= LSB(i) ) ret += A[i].sum(idx);
		return ret;
	}
	T sum_range( const VI &up, const VI &dn ) {
		// In higher dimensions, we have to use inclusion-exclusion
		size_t BD = ((size_t)1) << D;
		T ret = 0;
		FOR(S,0,BD) {
			int sign = 1;
			VI q(up);
			FOR(b,0,BD) if( (S >> b) & 1 ) {
				q[b] = dn[b];
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
		const size_t N  = (1U << 10); // ~1e3
		const size_t NN = N*N;        // ~1e6
		BIT tr(NN); // BIT on NN elts
		// Basic tests
		FOR(i,0,NN) tr.add( i, ((T)56)*i*(i+3) );
		VT A(NN+1); // prefix sums, for checking
		A[0] = 0; FOR(i,1,tr.N+1) A[i] = A[i-1] + ((T)56)*(i-1)*(i+2);
		FOR(q,0,NN+1) {
			T result = tr.sum(q);
			if( result != A[q] ) {
				success = false;
				cerr << "Error in BIT::add/sum(" << q << "): Expected: " << A[q] << ", Actual: " << result << endl;
			}
		}
		FOR(qL,0,N) FOR(qR,qL,qL+N) {
			T result = tr.sum_range(qL,qR);
			if( result != A[qR] - A[qL] ) {
				success = false;
				cerr << "Error in BIT::sum_range(" << qL << "," << qR << "): ";
				cerr << "Expected: " << A[qR] - A[qL] << ", Actual: " << result << endl;
			}
		}
	}
	{
		const size_t N = (1U << 7);
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
		size_t UB = AS.back();
		FOR(q,0,UB) {
			size_t result = tr.largest_at_most(q);
			size_t exp = 0;
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
		const size_t N = (1U << 4);
		BIT_flip tr(N);
		VT A(N,0);
		tr.add(3, 9, 16);  FOR(i,3,9)   A[i] += 16;
		tr.add(1, 4, 2 );  FOR(i,1,4)   A[i] += 2;
		tr.add(11,11,99);  FOR(i,11,11) A[i] += 99;
		tr.add(0, 9, 12);  FOR(i,0,9)   A[i] += 12;
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
		const size_t N = (1U << 4);
		BIT_super tr(N);
		VT A(N,0);
		FOR(i,0,N) FOR(j,i,N) {
			T v = (27*i*(i+3)*(j+7)) % 93;
			tr.add(i,j,v);
			FOR(k,i,j) A[k] += v;
		}
		VT AS(A.size()+1);
		AS[0] = 0; FOR(i,1,AS.size()) AS[i] = AS[i-1] + A[i-1];
		FOR(qL,0,N) FOR(qR,qL,N) {
			T result = tr.query_range(qL,qR);
			T exp = AS[qR] - AS[qL];
			if( result != exp ) {
				success = false;
				cerr << "Error in BIT_super::add/query(" << qL << "," << qR << "): ";
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
		const size_t N = (1U << 5);
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
			FOR(ki,qi1,qi2) FOR(kj,qj1,qj2) exp += A[ki][kj];
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
		const size_t N1 = (1U << 5), N2 = (1U << 4);
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
			FOR(ki,qi1,qi2) FOR(kj,qj1,qj2) exp += A[ki][kj];
			if( result != exp ) {
				success = false;
				cerr << "Error in BITd::add/query(" << qi1 << "," << qj1 << "  ->  " << qi2 << "," << qj2 << "): ";
				cerr << "Expected: " << exp << ", Actual: " << result << endl;
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
