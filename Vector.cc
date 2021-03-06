#ifndef VECTOR_CC
#define VECTOR_CC
#include "FloatCompare.cc"
#include <cmath>
using namespace std;
// BEGIN
// A simple library used elsewhere in the notebook.
// Provides basic vector/point operations.
typedef double T;

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
	return feq(a.x,b.x) && feq(a.y,b.y);
}
bool operator != ( const Pt &a, const Pt &b ) { return !(a == b); }

T dot( const Pt &a, const Pt &b )   { return a.x*b.x + a.y*b.y; }
T cross( const Pt &a, const Pt &b ) { return a.x*b.y - a.y*b.x; }
T norm2( const Pt &a )              { return a.x*a.x + a.y*a.y; } // dot(a,a)
T norm( const Pt &a )               { return sqrt(a.x*a.x + a.y*a.y); }
T dist2( const Pt &a, const Pt &b ) { // dot(a-b,a-b)
	T dx = a.x - b.x, dy = a.y - b.y;
	return dx*dx + dy*dy;
}
T dist( const Pt &a, const Pt &b )  { // sqrt(dot(a-b,a-b))
	T dx = a.x - b.x, dy = a.y - b.y;
	return sqrt(dx*dx + dy*dy);
}

bool lex_cmp_xy( const Pt &lhs, const Pt &rhs ) {
	if( !feq(lhs.x,rhs.x) ) return lhs.x < rhs.x;
	if( !feq(lhs.y,rhs.y) ) return lhs.y < rhs.y;
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
	if( Pt(5,0) != Pt(15.0/3,0) )          cerr << "Pt == too inexact" << endl;
	if( Pt(5,0) == Pt(50001.0/10000.0,0) ) cerr << "Pt == too exact" << endl;
	if( !feq(0,dot(a,b)) )                 cerr << "dot is incorrect (#1)" << endl;
	if( !feq(dot(b,b),2.0) )               cerr << "dot is incorrect (#2)" << endl;
	if( !feq(dot(b,c),0) )                 cerr << "dot is incorrect (#3)" << endl;
	if( !feq(cross(a,b),0) )               cerr << "cross is incorrect (#1)" << endl;
	if( !feq(cross(b,b),0) )               cerr << "cross is incorrect (#2)" << endl;
	if( !feq(cross(b,c),2.0) )             cerr << "cross is incorrect (#3)" << endl;
	if( !feq(norm2(b),2.0) )               cerr << "norm2 is incorrect (#1)" << endl;
	if( !feq(norm2(a),0) )                 cerr << "norm2 is incorrect (#2)" << endl;
	if( !feq(norm(b),sqrt(2.0)) )          cerr << "norm is incorrect (#1)" << endl;
	if( !feq(norm(a),0) )                  cerr << "norm is incorrect (#2)" << endl;
	if( !feq(dist2(a,b),2.0) )             cerr << "dist2 is incorrect (#1)" << endl;
	if( !feq(dist2(b,b),0) )               cerr << "dist2 is incorrect (#2)" << endl;
	if( !feq(dist(a,b),sqrt(2.0)) )        cerr << "dist is incorrect (#1)" << endl;
	if( !feq(dist(b,b),0) )                cerr << "dist is incorrect (#2)" << endl;
}

int main() {
	test_vector_correct();
	return 0;
}
#endif // BUILD_TEST_VECTOR
#endif // VECTOR_CC
