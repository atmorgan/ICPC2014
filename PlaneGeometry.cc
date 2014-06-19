#ifndef PLANE_GEOMETRY_CC
#define PLANE_GEOMETRY_CC
#include "Vector.cc"
#include <vector>
#include <cmath>
using namespace std;
// BEGIN
// Some routines for basic plane geometry.
// Depends on Vector.cc.
typedef vector<Pt> VP;

int isLeft( Pt a, Pt b,  Pt c ) {
	T z = cross(b-a,c-a);
	if( ApproxEq(z,0) ) return  0; // c is on the line ab
	else if( z > 0 )    return  1; // c is left of the line ab
	else                return -1; // c is right of the line ab
}
Pt RotateCCW90( Pt p ) { return Pt(-p.y,p.x); }
Pt RotateCW90( Pt p )  { return Pt(p.y,-p.x); }
Pt RotateCCW( Pt p, T t ) { // This only makes sense for T=double
	return Pt(p.x*cos(t)-p.y*sin(t), p.x*sin(t)+p.y*cos(t));
}
// Project the point c onto line ab
// This assumes a != b, so check that first.
Pt ProjectPointLine( const Pt &a, const Pt &b,  const Pt &c ) {
	return a + (b-a) * dot(b-a,c-a)/norm2(b-a);
}
// "Project" the point c onto segment ab
// Nicely paired with dist: dist(c, ProjectPointSegment(a,b,c))
Pt ProjectPointSegment( Pt a, Pt b,  Pt c ) {
	T r = dist2(a,b);
	if( ApproxEq(r,0) ) return a;
	r = dot(c-a, b-a)/r;
	if( r < 0 ) return a;
	if( r > 1 ) return b;
	return a + (b-a)*r;
}
// Compute the distance between point (x,y,z) and plane ax+by+cz=d
T DistancePointPlane( T x, T y, T z,   T a, T b, T c, T d ) {
	return fabs(a*x+b*y+c*z-d)/sqrt(a*a+b*b+c*c);
}
// Decide if lines ab and cd are parallel.
// If a=b or c=d, then this will return true.
bool LinesParallel( Pt a, Pt b, Pt c, Pt d ) {
	return ApproxEq( cross(b-a,c-d), 0 );
}
// Decide if lines ab and cd are the same line
// If a=b and c=d, then this will return true.
// If a=b xor c=d, (wlog a=b), then this is true iff a is on cd.
bool LinesColinear( Pt a, Pt b, Pt c, Pt d ) {
	return LinesParallel(a,b, c,d)
	    && isLeft(a,b, c) == 0
	    && isLeft(c,d, a) == 0; // to make a=b, c=d cases symmetric
}
// Determine if the segment ab intersects with segment cd
// Use line-line intersection (below) to find it.
// This *will* do the right thing if a=b, c=d, or both!
bool SegmentsIntersect( Pt a, Pt b,   Pt c, Pt d ) {
	if(  LinesColinear(a,b, c,d)  ) {
		if(  ApproxEq(dist2(a,c),0)   ||   ApproxEq(dist2(a,d),0)
			|| ApproxEq(dist2(b,c),0)   ||   ApproxEq(dist2(b,d),0) ) return true;
		if( dot(a-c,b-c) > 0 && dot(a-d,b-d) > 0 && dot(c-b,d-b) > 0 )
			return false;
		return true;
	}
	if( isLeft(a,b, d) * isLeft(a,b, c) > 0 ) return false;
	if( isLeft(c,d, a) * isLeft(c,d, b) > 0 ) return false;
	return true;
}
// Determine if c is on the segment ab
bool PointOnSegment( Pt a, Pt b,  Pt c )
	{ return SegmentsIntersect(a,b,c,c); }
// Compute the intersection of lines ab and cd.
// ab and cd are assumed to be *NOT* parallel
Pt ComputeLineIntersection( Pt a, Pt b,   Pt c, Pt d ) {
	b=b-a; d=d-c; c=c-a; // translate to a, set b,d to directions
	return a + b*cross(c,d)/cross(b,d); // solve s*b = c + t*d by Cramer
}
// Compute the center of the circle uniquely containing three points.
// It's assume the points are *NOT* colinear, so check that first.
Pt ComputeCircleCenter(Pt a, Pt b, Pt c) {
	b=(b-a)/2;   c=(c-a)/2; // translate to a=origin, shrink to midpoints
	return a+ComputeLineIntersection(b,b+RotateCW90(b),c,c+RotateCW90(c));
}
// Compute intersection of line ab with circle at c with radius r.
// This assumes a!=b.
VP CircleLineIntersection( Pt a, Pt b, Pt c, T r ) {
	VP ret;
	b = b-a;   a = a-c;   // translate c to origin, make b the direction
	T A = dot(b,b);       // Let P(t) = a + t*b, and Px, Py projections
	T B = dot(a,b);       // Solve Px(t)^2 + Py(t)^2 = r^2
	T C = dot(a,a) - r*r; // Get A*t^2 + 2B*t + C = 0
	T D = B*B - A*C;      // 4*D is the discriminant^
	if( ApproxLt(D,0) ) return ret;
	D = sqrt( max((T)0,D) );
	ret.push_back( c+a + b*(-B + D)/A );
	if( ApproxEq(D,0) ) return ret;
	ret.push_back( c+a + b*(-B - D)/A );
	return ret;
}
// Compute intersection of circle at a with radius r
// with circle at b with radius s.
// This assumes the circles are distinct, ie (a,r)!=(b,s)
VP CircleCircleIntersection( Pt a, T r, Pt b, T s ) {
	VP ret;
	T d = dist(a, b);
	if( d > r+s || d+min(r,s) < max(r,s) ) return ret; // empty
	T x = (d*d-s*s+r*r)/(2*d);   // The rest of this is magic.
	T y = sqrt(r*r-x*x);         // (It's actually basic geometry.)
	Pt v = (b-a)/d;
	ret.push_back(a+v*x + RotateCCW90(v)*y);
	if( !ApproxEq(y,0) )
		ret.push_back(a+v*x - RotateCCW90(v)*y);
	return ret;
}
// END

#ifdef BUILD_TEST_PLANE_GEOMETRY
// TODO: Tests!!!
#include<iostream>
int main() {
  bool t1 = PointOnSegment(Pt(3, 4), Pt(3, 7), Pt(3, 19)); // expect false
  bool t2 = PointOnSegment(Pt(3, 19), Pt(9, 19), Pt(3, 19)); // expect true
  bool t3 = PointOnSegment(Pt(5, 19), Pt(9, 19), Pt(8, 19)); // expect true
  bool t4 = PointOnSegment(Pt(3, 4), Pt(9, 12), Pt(6, 8)); // expect true
  bool t5 = PointOnSegment(Pt(-3, -4), Pt(9, 12), Pt(0, 0)); // expect true
  bool t6 = PointOnSegment(Pt(1.1, 2.4), Pt(-7.8, 16.3), Pt(3.9, 3)); // expect false
  bool t7 = PointOnSegment(Pt(1.1, 5.6), Pt(1.4, .8), Pt(1.2, 4)); // expect true
  bool t8 = PointOnSegment(Pt(9, 19), Pt(5, 19), Pt(9.01, 19)); // expect false

  Pt a = Pt(3.73, -5.34); Pt b = Pt(-2.49, 4.33); Pt c = Pt(-2.536, -1.5544); Pt d = Pt(96953.45, 93414.45);
  Pt center = ComputeCircleCenter(a, b, c);
  Pt center2 = ComputeCircleCenter(b, c, d);
  T r = dist(a, center);
  T r2 = dist(b, center2);
  VP circline = CircleLineIntersection(b + .5 * (b-c), c + .2 * (b-c), center, r);
  Pt intersect = ComputeLineIntersection(a, b,   c, d);
  VP circles = CircleCircleIntersection(center, r, center2, r2);

  cout << "t1 expects false: " << t1 << endl;
  cout << "t2 expects true: " << t2 << endl;
  cout << "t3 expects true: " << t3 << endl;
  cout << "t4 expects true: " << t4 << endl;
  cout << "t5 expects true: " << t5 << endl;
  cout << "t6 expects false: " << t6 << endl;
  cout << "t7 expects true: " << t7 << endl;
  cout << "t8 expects false: " << t8 << endl;
  cout << "expects same: " << dist(a, center) << " " << dist(b, center) << " " << dist(c, center) << endl;
  cout << "intersect: " << intersect.x << " " << intersect.y << endl;
  cout << fabs(cross(d - c, intersect - d)) << endl;
  cout << "expects true: " << (b == circline[0] || c == circline[0]) << endl;
  cout << "expects true: " << (b == circles[0] || c == circles[0]) << endl;
  cout << "circles intersection: " << circles[0].x << " " << circles[0].y << "   " << circles[1].x << " " << circles[1].y <<endl;
  cout << "expects true: " << (LinesParallel(a, b, a, intersect) && LinesParallel(c, d, d, intersect)) << endl;
  return 0; 
}
#endif // BUILD_TEST_PLANE_GEOMETRY
#endif // PLANE_GEOMETRY_CC
