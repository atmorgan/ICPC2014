#include <vector>
using namespace std;

// BEGIN
// Throughout all following code, it's assumed that inputs are nonnegative.
// However, a signed type is used for two purposes:
// 1. -1 is used as an error code sometimes.
// 2. Some of these (egcd) actually have negative return values.

typedef signed long long int T;
typedef vector<T> VT;

// basic gcd
T gcd( T a, T b ) {
	if( a < 0 ) return gcd(-a,b);
	if( b < 0 ) return gcd(a,-b);
	T c;
	while( b ) { c = a % b; a = b; b = c; }
	return a;
}

// basic lcm
T lcm( T a, T b ) {
	if( a < 0 ) return lcm(-a,b);
	if( b < 0 ) return lcm(a,-b);
	return a/gcd(a,b)*b;
}

// returns gcd(a,b), and additionally finds x,y such that d = ax + by (Bezout's identity)
T egcd( T a, T b, T &x, T &y ) {
	if( a < 0 ) {
		T r = egcd(-a,b,x,y);
		x *= -1;
		return r;
	}
	if( b < 0 ) {
		T r = egcd(a,-b,x,y);
		y *= -1;
		return r;
	}
	T u = y = 0,   v = x = 1;
	while( b ) {
		T q = a/b,     r = a % b;
		  a = b,       b = r;
		T m = u,       n = v;
		  u = x - q*u, v = y - q*v;
		  x = m,       y = n;
	}
	return a;
}

// Compute b so that ab = 1 (mod n).
// Returns n if gcd(a,n) != 1, since no such b exists.
T modinv( T a, T n ) {
	T x, y, g = egcd( a, n, x, y );
	if( g != 1 ) return -1;
	if( x < 0 ) x += (y+a-1)/a*n;
	x %= n;
	return x;
}

// Find all solutions to ax = b (mod n),
// and push them onto S.
// Returns the number of solutions.
// Solutions exist iff gcd(a,n) divides b.
// If solutions exist, then there are exactly gcd(a,n) of them.
size_t modsolve( T a, T b, T n, VT &S ) {
	T g = gcd(a,n);
	if( (b % g) == 0 ) {
		T x = modinv( a/g, n/g );
		x = (x * b/g) % (n/g);
		for( T k = 0; k < g; k++ )
			S.push_back( (x + k*(n/g)) % n );
		return (size_t)g;
	}
	return 0;
}

// Chinese remainder theorem, simple version.
// Given a, b, n, m, find z which simultaneously satisfies
//     z = a (mod m)  and  z = b (mod n).
// This z, when it exists, is unique mod lcm(n,m).
// If such z does not exist, then return -1.
// z exists iff a == b (mod gcd(m,n))
T CRT( T a, T m, T b, T n ) {
	T s, t, g = egcd(m, n, s, t);
	T l = m/g*n, r = a % g;
	if( (b % g) != r ) return -1;
	if( g == 1 ) {
		while( s < 0 ) s += l;
		while( t < 0 ) t += l;
		s %= l; t %= l;
		T r1 = (s * b)   % l, r2 = (t * a)   % l;
		  r1 = (r1 * m)  % l, r2 = (r2 * n)  % l;
		return (r1 + r2) % l;
	}
	else {
		return g*CRT(a/g, m/g, b/g, n/g) + r;
	}
}

// Chinese remainder theorem, extended version.
// Given a[K] and n[K], find z so that, for every i,
//     z = a[i] (mod n[i])
// The solution is unique mod lcm( n[i] ) when it exists.
// The existence criteria is just the extended version of what it is above.
T CRT_ext( const VT &a, const VT &n ) {
	T ret = a[0], l = n[0];
	for( size_t i = 1; i < a.size(); i++ ) {
		ret = CRT( ret, l,  a[i], n[i]);
		l = lcm( l, n[i] );
		if( ret == -1 ) return -1;
	}
	return ret;
}

// Compute x and y so that ax + by = c.
// The solution, when it exists, is unique up to the transformation
//     x -> x + kb/g
//     y -> y - ka/g
// for integers k, where g = gcd(a,b).
// The solution exists iff gcd(a,b) divides c.
// The return value is true iff the solution exists.
bool linear_diophantine( T a, T b, T c, T &x, T &y ) {
	T s,t, g = egcd(a,b,s,t);
	if( (c % g) != 0 )
		return false;
	x = c/g*s; y = c/g*t;
	return true;
}
// END

#include <iostream>

void test_gcd() {
	cerr << "test gcd" << endl;
	if( gcd( 4, 7) != 1 ) cerr << "gcd(  4,  7 ) != 1" << endl;
	if( gcd( 0, 7) != 7 ) cerr << "gcd(  0,  7 ) != 1" << endl;
	if( gcd(14, 7) != 7 ) cerr << "gcd( 14,  7 ) != 1" << endl;
	if( gcd(14,21) != 7 ) cerr << "gcd( 14, 21 ) != 1" << endl;
	if( gcd(-7, 7) != 7 ) cerr << "gcd( -7,  7 ) != 1" << endl;
	if( gcd(-7,-7) != 7 ) cerr << "gcd( -7, -7 ) != 1" << endl;
}
void test_lcm() {
	cerr << "test lcm" << endl;
	for( T a = 1; a < 1000; ++a )
	for( T b = 1; b < 1000; ++b ) {
		if( gcd(a,b)*lcm(a,b) != a*b ) {
			cerr << "lcm*gcd != product:  " << a << "  " << b << endl;
		}
	}
}
void test_egcd() {
	cerr << "test egcd" << endl;
	for( T a = 0; a < 4000; ++a )
	for( T b = 0; b < 4000; ++b ) {
		T s, t, g = egcd(a,b,s,t);
		if( gcd(a,b) != g ) {
			cerr << "gcd != egcd:  " << a << "  " << b << endl;
		}
		if( s*a + b*t != g ) {
			cerr << "egcd s*a + t*b = g fail:  " << a << " " << b << endl;
		}
	}
}
void test_modinv() {
	cerr << "test modinv" << endl;
	for( T a = 0; a < 2000; ++a )
	for( T n = 2; n < 2000; ++n ) {
		T ai = modinv(a, n);
		if( gcd(a,n) != 1 && ai != -1 ) {
			cerr << "modinv returned -1 when it shouldn't have:  " << a << " mod " << n << endl;
		}
		if( gcd(a,n) == 1 ) {
			if( (a * ai) % n != 1 ) {
				cerr << "modinv computed the wrong inverse:  " << a << " mod " << n << endl;
			}
			if( ai >= n ) {
				cerr << "modinv computed a too-large inverse:  " << a << " mod " << n << endl;
			}
		}
	}
}
void test_modsolve() {
	cerr << "test modsolve" << endl;
	for( T n = 1; n < 250; ++n )
	for( T a = 0; a < n; ++a )
	for( T b = 0; b < n; ++b ) {
		VT sol;
		size_t r = modsolve( a, b, n, sol );
		if( r != sol.size() ) {
			cerr << "modsolve counted solutions wrong:  " << a << "x = " << b << " mod " << n << endl;
		}
		T g = gcd(a,n);
		if( (b % g) != 0 && r != 0 ) {
			cerr << "modsolve says there are solutions when there aren't:  " << a << "x = " << b << " mod " << n << endl;
		}
		if( (b % g) == 0 && r != (size_t)g ) {
			cerr << "modsolve didn't make gcd(a,n) solutions:  " << a << "x = " << b << " mod " << n << endl;
		}
		for( size_t i = 0; i < r; ++i ) {
			if( ((a*sol[i]) % n) != (b % n) ) {
				cerr << "modsolve gave a bad solution:  " << a << "x = " << b << " mod " << n << " (" << i << ")" << endl;
			}
			if( sol[i] >= n ) {
				cerr << "modsolve gave a too-large solution:  " << a << "x = " << b << " mod " << n << " (" << i << ")" << endl;
			}
		}
	}
}
void test_CRT() {
	cerr << "test CRT" << endl;
	for( T m = 1; m < 100; ++m )
	for( T n = 1; n < 100; ++n )
	for( T a = 0; a < m; ++a )
	for( T b = 0; b < n; ++b ) {
		T z = CRT(a,m, b,n);
		T g = gcd(m,n);
		if( (a % g) != (b % g) && z != -1 ) {
			cerr << "CRT gave an impossible solution:  z = " << a << " mod " << m << " and z = " << b << " mod " << n << endl;
		}
		if( (a % g) == (b % g) ) {
			if( (z % m) != a || (z % n) != b ) {
				cerr << "CRT gave a bad solution:  z = " << a << " mod " << m << " and z = " << b << " mod " << n << endl;
			}
		}
	}
}
void test_CRT_ext() {
	cerr << "test CRT_ext" << endl;
	{
		const T solution = 155;
		const T _n[] = { 9, 4, 55, 77, 10, 166 };
		VT n( _n, _n+6 );
		VT a( n.size() );
		for( size_t i = 0; i < n.size(); ++i )
			a[i] = solution % n[i];
		T r = CRT_ext( a, n );
		if( solution != r ) {
			cerr << "CRT_ext gave the wrong solution (test #1)" << endl;
		}
	}
	{
		const T _n[] = { 2, 6 };
		VT n( _n, _n+2 ), a( n.size() );
		a[0] = 0;
		a[1] = 1;
		T r = CRT_ext( a, n );
		if( r != -1 ) {
			cerr << "CRT_ext did not indicate failure (test #2)" << endl;
		}
	}
}
void test_linear_diophantine() {
	cerr << "test linear_diophantine" << endl;
	{
		const T a = 2, b = 3, x = -8, y = 19;
		const T c = a*x + b*y;
		T _x, _y; _x = _y = 0;
		bool r = linear_diophantine(a,b,c, _x,_y);
		if( !r ) {
			cerr << "linear_diophantine returned no solution (test #1)" << endl;
		}
		if( _x*a + _y*b != c ) {
			cerr << "linear_diophantine returned a wrong solution (test #1)" << endl;
		}
	}
	{
		const T a = 2, b = 8, c = 7;
		T _x, _y; _x = _y = 0;
		bool r = linear_diophantine(a,b,c, _x,_y);
		if( r ) {
			cerr << "linear_diophantine returned existence of solution (test #2)" << endl;
		}
	}
	{
		const T a = 6, b = 8, c = 4;
		T _x, _y; _x = _y = 0;
		bool r = linear_diophantine(a,b,c, _x,_y);
		if( !r ) {
			cerr << "linear_diophantine returned no solution (test #3)" << endl;
		}
		if( a*_x + b*_y != c ) {
			cerr << "linear_diophantine returned a wrong solution (test #3)" << endl;
		}
	}
}

int main() {
	test_gcd();
	test_lcm();
	test_egcd();
	test_modinv();
	test_modsolve();
	test_CRT();
	test_CRT_ext();
	test_linear_diophantine();
	return 0;
}

