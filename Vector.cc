#ifndef VECTOR_CC
#define VECTOR_CC
#include <cmath>
using namespace std;
// BEGIN
// A simple library used elsewhere in the notebook.
// Provides basic vector/point operations.
typedef double T;
const T EPS = 1e-8;

struct Pt {
	T x, y;
	Pt() {}
	Pt( T x, T y ) : x(x), y(y) {}
	Pt( const Pt &h ) : x(h.x), y(h.y) {}
};

Pt operator + ( const Pt &a, const Pt &b ) { return Pt(a.x+b.x, a.y+b.y); }
Pt operator - ( const Pt &a, const Pt &b ) { return Pt(a.x-b.x, a.y-b.y); }
Pt operator * ( const T s, const Pt &a )   { return Pt(s*a.x, s*a.y); }
Pt operator * ( const Pt &a, const T s )   { return s*a; }
Pt operator / ( const Pt &a, const T s )   { return Pt(a.x/s,a.y/s); }
// Note the kind of division that occurs when using integer types.
// Use rationals if you want this to work right.
bool operator == ( const Pt &a, const Pt &b ) {
	return fabs(a.x-b.x) <= EPS && fabs(a.y-b.y) <= EPS;
}
bool operator != ( const Pt &a, const Pt &b ) { return !(a == b); }

T dot( const Pt &a, const Pt &b )   { return a.x*b.x + a.y*b.y; }
T cross( const Pt &a, const Pt &b ) { return a.x*b.y - a.y*b.x; }
T norm2( const Pt &a )              { return dot(a,a); }
T norm( const Pt &a )               { return sqrt(norm2(a)); }
T dist2( const Pt &a, const Pt &b ) { return norm2(a-b); }
T dist( const Pt &a, const Pt &b )  { return norm(a-b); }

bool lex_cmp_xy( const Pt &lhs, const Pt &rhs ) {
	if( fabs(lhs.x-rhs.x) > EPS ) return lhs.x < rhs.x;
	if( fabs(lhs.y-rhs.y) > EPS ) return lhs.y < rhs.y;
	return false;
}
// END

#ifdef BUILD_TEST_VECTOR
#include <iostream>

void test_vector_correct() {
	cerr << "test vector operations" << endl;
	Pt a(0,0), b(1,1), c(-1,1);
	if( a+b != Pt( 1, 1) )                 cerr << "Pt + Pt fails" << endl;
	if( a-b != Pt(-1,-1) )                 cerr << "Pt - Pt fails" << endl;
	if( 5.0*b != Pt(5,5) )                 cerr << "scalar * Pt fails" << endl;
	if( b*5.0 != Pt(5,5) )                 cerr << "Pt * scalar fails" << endl;
	if( b/5.0 != Pt(0.2,0.2) )             cerr << "Pt / scalar fails" << endl;
	if( a != Pt(EPS/2,EPS/2) )             cerr << "Pt == Pt too exact" << endl;
	if( a == Pt(2*EPS,2*EPS) )             cerr << "Pt == Pt too inexact" << endl;
	if( fabs(dot(a,b)) >= EPS )            cerr << "dot is incorrect (#1)" << endl;
	if( fabs(dot(b,b)-2.0) >= EPS )        cerr << "dot is incorrect (#2)" << endl;
	if( fabs(dot(b,c)) >= EPS )            cerr << "dot is incorrect (#3)" << endl;
	if( fabs(cross(a,b)) >= EPS )          cerr << "cross is incorrect (#1)" << endl;
	if( fabs(cross(b,b)) >= EPS )          cerr << "cross is incorrect (#2)" << endl;
	if( fabs(cross(b,c)-2.0) >= EPS )      cerr << "cross is incorrect (#3)" << endl;
	if( fabs(norm2(b)-2.0) >= EPS )        cerr << "norm2 is incorrect (#1)" << endl;
	if( fabs(norm2(a)) >= EPS )            cerr << "norm2 is incorrect (#2)" << endl;
	if( fabs(norm(b)-sqrt(2.0)) >= EPS )   cerr << "norm is incorrect (#1)" << endl;
	if( fabs(norm(a)) >= EPS )             cerr << "norm is incorrect (#2)" << endl;
	if( fabs(dist2(a,b)-2.0) >= EPS )      cerr << "dist2 is incorrect (#1)" << endl;
	if( fabs(dist2(b,b)) >= EPS )          cerr << "dist2 is incorrect (#2)" << endl;
	if( fabs(dist(a,b)-sqrt(2.0)) >= EPS ) cerr << "dist is incorrect (#1)" << endl;
	if( fabs(dist(b,b)) >= EPS )           cerr << "dist is incorrect (#2)" << endl;
}

int main() {
	test_vector_correct();
	return 0;
}
#endif // BUILD_TEST_VECTOR
#endif // VECTOR_CC
