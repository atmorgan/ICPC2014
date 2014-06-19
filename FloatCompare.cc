#include <cmath>
using namespace std;
// BEGIN
// Short function for comparing floating point numbers.

const MIN_NORMAL = 1e-100; // close enough
const EPS_ABS    = 1e-10;  // for values near 0.0.
const EPS_REL    = 1e-8;   // for values NOT near 0.0.

bool ApproxEq( double a, double b ) {
	double d = fabs(b-a);
	if( d <= EPS_ABS ) return true;
	if( d <= max(fabs(a),fabs(b))*EPS_REL ) return true;
	return false;
}
// END
