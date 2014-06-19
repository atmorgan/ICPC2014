#ifndef POLYGON_CC
#define POLYGON_CC
#include "Vector.cc"
#include "PlaneGeometry.cc"
#include <cmath>
#include <vector>
#include <algorithm>
// BEGIN
// Basic routines for polygon-related stuff.
// Uses Vector.cc and PlaneGeometry.cc.
// Polygons are just vector<Pt>'s.
#define FOR(v,l,u) for( size_t v = l; v < u; ++v )
typedef vector<Pt> VP;

T ComputeSignedArea( const VP &p ) {
	T area = 0;
	for( size_t i = 0; i < p.size(); i++ ) {
		size_t z = (i + 1) % p.size();
		area += cross( p[i], p[z] );
	}
	return area / 2.0;
}
T ComputeArea( const VP &p ) {
	return fabs(ComputeSignedArea(p));
}
T ComputePerimeter( const VP &p ) {
	T perim = 0.0;
	for( size_t i = 0; i < p.size(); ++i )
		perim += dist(p[i], p[(i+1) % p.size()]);
	return perim;
}
Pt ComputeCentroid( const VP &p ) {
	Pt c(0,0); T scale = 6.0 * ComputeSignedArea(p);
	for( size_t i = 0; i < p.size(); i++ ) {
		size_t j = (i + 1) % p.size();
		c = c + cross(p[i],p[j]) * (p[i]+p[j]);
	}
	return c / scale;
}
bool IsSimple( const VP &p ) {
	for( size_t i = 0; i < p.size(); ++i )
	for( size_t k = i+1; k < p.size(); ++k ) {
		size_t j = (i + 1) % p.size();
		size_t l = (k + 1) % p.size();
		if( i == l || j == k ) continue;
		if( SegmentsIntersect(p[i],p[j], p[k],p[l]) ) 
			return false;
	}
	return true;
}
// Determine the winding number of a point. This is the number of
// times the polygon goes around the given point.
// It is 0 exactly when the point is outside.
// A signed type is used intermediately so that we don't have to
// detect CW versus CCW, but the absolute value is taken in the end.
// If q is *on* the polygon, then the results are not well-defined,
// since it depends on whether q is on an "up" or "down" edge.
size_t WindingNumber( const VP &p, Pt q) {
	int wn = 0;   vector<int> state(p.size()); // state decides up/down
	FOR(i,0,p.size())
		if( ApproxEq(p[i].y, q.y) )  state[i] =  0; // break ties later
		else if( p[i].y < q.y )      state[i] = -1; // we'll use nearest
		else                         state[i] =  1; // neighbor (either)
	FOR(i,1,p.size())   if( state[i] == 0 ) state[i] = state[i-1];
	FOR(i,0,p.size()-1) if( state[i] == 0 ) state[i] = state[i+1];
	FOR(i,0,p.size()) {
		size_t z = (i + 1) % p.size();
		if( state[z] == state[i] ) continue; // only interested in changes
		else if( state[z] == 1 && isLeft(p[i],p[z],q) > 0 ) ++wn;
		else if( state[i] == 1 && isLeft(p[i],p[z],q) < 0 ) --wn;
	}
	return (size_t)(wn < 0 ? -wn : wn);
}
// A complement to the above.
bool PointOnPolygon( const VP &p, Pt q ) {
	for( size_t i = 0; i < p.size(); i++ ) {
		size_t z = (i + 1) % p.size();
		if( PointOnSegment(p[i],p[z],q) )
			return true;
	}
	return false;
}
// Convex hull.
// This *will* modify the given VP. To save your points, do
// {   VP hull(p.begin(),p.end());    ConvexHull(hull);   }
// This *will* keep redundant points on the polygon border.
// To ignore those, change the isLeft's < and > to <= and >=.
void ConvexHull( VP &Z ) {
	sort( Z.begin(), Z.end(), lex_cmp_xy);
	Z.resize( unique(Z.begin(),Z.end()) - Z.begin() );
	VP up, dn;
	for( size_t i = 0; i < Z.size(); i++ ) {
		while(up.size() > 1 && isLeft(up[up.size()-2],up.back(),Z[i]) > 0)
			up.pop_back();
		while(dn.size() > 1 && isLeft(dn[dn.size()-2],dn.back(),Z[i]) < 0)
			dn.pop_back();
		up.push_back(Z[i]);
		dn.push_back(Z[i]);
	}
	Z = dn;
	for( size_t i = up.size() - 2; i >= 1; i-- ) Z.push_back(up[i]);
}
// END
// Poly-poly intersection?
// Triangulating? (decomposing poly into triangles)

#ifdef BUILD_TEST_POLYGON
// TODO: Implement tests!!!
int main() { return 0; }
#endif // BUILD_TEST_POLYGON
#endif // POLYGON_CC
