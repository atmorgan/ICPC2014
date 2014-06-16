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
	int ret = 0;
	for( size_t i = 0; i < p.size(); i++ ) {
		size_t z = (i + 1) % p.size();
		if( p[i].y <= q.y ) {                 // must go up to pass q
			if( p[z].y > q.y )                  // passing
				if( isLeft( p[i], p[z], q ) > 0 ) // q is to the left of edge
					++ret;
		}
		else {                                // must go down to pass q
			if( p[z].y < q.y )                  // passing
				if( isLeft( p[z], p[i], q ) > 0 ) // q is to the left of edge
					--ret;
		}
	}
	return (size_t)(ret < 0 ? -ret : ret);
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
// REMOVE_REDUNDANT does what it sounds like it does.
#define REMOVE_REDUNDANT
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
#ifdef REMOVE_REDUNDANT
	if( Z.size() <= 2 ) return;
	dn.clear();
	dn.push_back( Z[0] );
	dn.push_back( Z[1] );
	for( size_t i = 2; i < Z.size(); i++ ) {
		if( PointOnSegment(dn[dn.size()-2],Z[i],dn.back()) )
			dn.pop_back();
		dn.push_back(Z[i]);
	}
	if( dn.size() >= 3 && PointOnSegment(dn.back(),dn[1],dn[0]) ) {
		dn[0] = dn.back();
		dn.pop_back();
	}
	Z = dn;
#endif
}
// END
// Poly-poly intersection?
// Triangulating? (decomposing poly into triangles)

#ifdef BUILD_TEST_POLYGON
// TODO: Implement tests!!!
int main() { return 0; }
#endif // BUILD_TEST_POLYGON
#endif // POLYGON_CC
