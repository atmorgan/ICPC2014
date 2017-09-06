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

// These generalize to higher-dimensional polyhedra, provided you represent
// them as a collection of facets
// Just replace "cross" with the suitable determinant, and adjust any scaling
// factors.
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
		if( feq(p[i].y, q.y) )  state[i] =  0; // break ties later
		else if( p[i].y < q.y ) state[i] = -1; // we'll use nearest
		else                    state[i] =  1; // neighbor (either)
	FOR(i,1,p.size())   if( state[i] == 0 ) state[i] = state[i-1];
	if( state[0] == 0 ) state[0] = state.back();
	FOR(i,1,p.size())   if( state[i] == 0 ) state[i] = state[i-1];
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
// Convex hull via monotone chain algorithm.
// This *will* modify the given VP. To save your points, do
// {   VP hull(p.begin(),p.end());    ConvexHull(hull);   }
// This *will* keep redundant points on the polygon border.
// To ignore those, change the isLeft's < and > to <= and >=.
void ConvexHull( VP &Z ) {
	sort( Z.begin(), Z.end(), lex_cmp_xy);
	Z.resize( unique(Z.begin(),Z.end()) - Z.begin() );
	if( Z.size() < 2 ) return;
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
// Implementation of Sutherland-Hodgman algorithm:
// https://en.wikipedia.org/wiki/Sutherland-Hodgman_algorithm
// Computes the intersection of polygon subject and polygon clip.
// Polygons points must be given in clockwise order. Clip must be convex.
// May return repeated points, especially if intersection is a single point or line segment.
// If no intersection occurs, will return an empty vector.
// Undefined behavior if intersection consists of multiple polygons.
VP ConvexClipPolygon( const VP &subject, const VP &clip ) {
    VP output = subject;
    for (size_t i = 0; i < clip.size(); ++i) {
        size_t ip1 = (i+1)%clip.size();
        Pt EdgeStart = clip[i];
        Pt EdgeEnd = clip[ip1];
        VP input = output;
        output.clear();
        Pt S = input.back();
        for (size_t j = 0; j < input.size(); ++j) {
            Pt E = input[j];
            if (isLeft(EdgeStart, EdgeEnd, E) <= 0) {
                if (isLeft(EdgeStart, EdgeEnd, S) > 0) {
                    output.push_back(ComputeLineIntersection(EdgeStart, EdgeEnd,  S, E));
                }
                output.push_back(E);
            }
            else if (isLeft(EdgeStart, EdgeEnd, S) <= 0) {
                output.push_back(ComputeLineIntersection(EdgeStart, EdgeEnd,  S, E));
            }
            S = E;
        }
    }
    return output;
}

// END
// Poly-poly intersection?
// Triangulating? (decomposing poly into triangles)

#include <iostream>

void print_points(VP &points) {
    if (points.size() > 0) {
        cerr << "(" << points[0].x << " " << points[0].y << ")";
        for (int i = 1; i < points.size(); ++i) {
            cerr << ", (" << points[i].x << " " << points[i].y << ")";
        }
        cerr << endl;
    }
    else {
        cerr << "<empty>" << endl;
    }
}

bool testcase_ConvexClipPolygon(VP subject, VP clip, VP expected) {
    bool success = true;    
    VP result = ConvexClipPolygon(subject, clip);
    if (result.size() != expected.size()) {
        success = false;
    }
    else {
        for (size_t i = 0; i < expected.size(); ++i) {
            if (!feq(result[i].x, expected[i].x) || !feq(result[i].y, expected[i].y)) {
                success = false;
            }
        }
    }
    if (!success) {
        cerr << "ConvexClipPolygon incorrect" << endl;
        cerr << "Expected: ";
        print_points(expected);
        cerr << "Actual: ";
        print_points(result);
        return false;
    }
    return true;
}

void test_ConvexClipPolygon_correct() {
    bool success = true;
    {
        VP subject;
        subject.push_back(Pt(9, 1));
        subject.push_back(Pt(5, -4));
        subject.push_back(Pt(3, -2));
        subject.push_back(Pt(3, 1));
        subject.push_back(Pt(5, 3));

        VP clip;
        clip.push_back(Pt(0, 0));
        clip.push_back(Pt(1, 4));
        clip.push_back(Pt(5, 0));

        VP expected;
        expected.push_back(Pt(5, 0));
        expected.push_back(Pt(3, 0));
        expected.push_back(Pt(3, 1));
        expected.push_back(Pt(3.5, 1.5));

        if (!testcase_ConvexClipPolygon(subject, clip, expected)) {
            success = false;
        }
    }
    {
        VP subject;
        subject.push_back(Pt(0, 0));
        subject.push_back(Pt(0, 2));
        subject.push_back(Pt(2, 0));

        VP clip;
        clip.push_back(Pt(2, 2));
        clip.push_back(Pt(1, 3));
        clip.push_back(Pt(3, 4));
        clip.push_back(Pt(3, 2));

        VP expected;

        if (!testcase_ConvexClipPolygon(subject, clip, expected)) {
            success = false;
        }
    }
    {
        VP subject;
        subject.push_back(Pt(0, 0));
        subject.push_back(Pt(0, 2));
        subject.push_back(Pt(2, 0));

        VP clip;
        clip.push_back(Pt(2, 0));
        clip.push_back(Pt(1, 3));
        clip.push_back(Pt(3, 4));
        clip.push_back(Pt(3, 2));

        VP expected;
        expected.push_back(Pt(2, 0));
        expected.push_back(Pt(2, 0));
        expected.push_back(Pt(2, 0));

        if (!testcase_ConvexClipPolygon(subject, clip, expected)) {
            success = false;
        }
    }
    {
        VP subject;
        subject.push_back(Pt(2, 0));
        subject.push_back(Pt(0, 5));
        subject.push_back(Pt(1, 5));
        subject.push_back(Pt(2.5, 1));
        subject.push_back(Pt(4, 3));
        subject.push_back(Pt(5, 3));
        subject.push_back(Pt(6.5, 1));
        subject.push_back(Pt(8, 5));
        subject.push_back(Pt(9, 5));
        subject.push_back(Pt(7, 0));
        subject.push_back(Pt(6, 0));
        subject.push_back(Pt(4.5, 2));
        subject.push_back(Pt(3, 0));

        VP clip;
        clip.push_back(Pt(0, 2));
        clip.push_back(Pt(1, 4));
        clip.push_back(Pt(4, 5));
        clip.push_back(Pt(9, 1));
        clip.push_back(Pt(4, 0));

        VP expected;
        expected.push_back(Pt(1.5, 1.25));
        expected.push_back(Pt(2.0/3, 10.0/3));
        expected.push_back(Pt(1, 4));
        expected.push_back(Pt(4.0/3, 37.0/9));
        expected.push_back(Pt(2.5, 1));
        expected.push_back(Pt(4, 3));
        expected.push_back(Pt(5, 3));
        expected.push_back(Pt(6.5, 1));
        expected.push_back(Pt(92.0/13, 33.0/13));
        expected.push_back(Pt(257.0/33, 65.0/33));
        expected.push_back(Pt(167.0/23, 15.0/23));
        expected.push_back(Pt(132.0/23, 8.0/23));
        expected.push_back(Pt(4.5, 2));
        expected.push_back(Pt(36.0/11, 4.0/11));

        if (!testcase_ConvexClipPolygon(subject, clip, expected)) {
            success = false;
        }
    }
    {
        VP subject;
        subject.push_back(Pt(0, 0));
        subject.push_back(Pt(0, 3));
        subject.push_back(Pt(3, 3));
        subject.push_back(Pt(3, 0));

        VP clip;
        clip.push_back(Pt(3, 2));
        clip.push_back(Pt(3, 5));
        clip.push_back(Pt(5, 5));
        clip.push_back(Pt(5, 2));

        VP expected;
        expected.push_back(Pt(3, 2));
        expected.push_back(Pt(3, 3));
        expected.push_back(Pt(3, 3));
        expected.push_back(Pt(3, 2));

        if (!testcase_ConvexClipPolygon(subject, clip, expected)) {
            success = false;
        }
    }
    {
        VP subject;
        subject.push_back(Pt(0, 0));
        subject.push_back(Pt(0, 3));
        subject.push_back(Pt(3, 3));
        subject.push_back(Pt(3, 0));

        VP clip;
        clip.push_back(Pt(1, 1));
        clip.push_back(Pt(1, 2));
        clip.push_back(Pt(2, 2));
        clip.push_back(Pt(2, 1));

        VP expected;
        expected.push_back(Pt(2, 1));
        expected.push_back(Pt(1, 1));
        expected.push_back(Pt(1, 2));
        expected.push_back(Pt(2, 2));

        if (!testcase_ConvexClipPolygon(subject, clip, expected)) {
            success = false;
        }
    }

    if (success) {
        cerr << "ConvexClipPolygon correct!" << endl;
    }
}

#ifdef BUILD_TEST_POLYGON
// TODO: Implement tests!!!
// TODO: arbitrary polygon intersection, orientation test for inside convex polygon (see SCUD busters)
int main() { 
    // ConvexClipPolygon has gotten Accepted for UVa Online Judge problem "Polygons"
    test_ConvexClipPolygon_correct();
    return 0; 
}
#endif // BUILD_TEST_POLYGON
#endif // POLYGON_CC
