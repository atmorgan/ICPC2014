#include <cmath>
#include <algorithm>
using namespace std;
// BEGIN
// Short function for comparing floating point numbers.

const double EPS_ABS    = 1e-10;  // for values near 0.0. Keep small.
const double EPS_REL    = 1e-8;   // for values NOT near 0.0. Balance.

bool ApproxEq( double a, double b ) {
	double d = fabs(b-a);
	if( d <= EPS_ABS ) return true;
	if( d <= max(fabs(a),fabs(b))*EPS_REL ) return true;
	return false;
}

bool ApproxLt( double a, double b ) {
	return !ApproxEq(a,b) && a < b;
}
// END

#ifdef TEST_FLOAT_COMPARE
#include <iostream>
int main() {
	if(  ApproxEq(1.0/1e6, 0.0) )     cout << "ApproxEq wrong near 0" << endl;
	if( !ApproxEq(0.0,     0.0) )     cout << "ApproxEq incorrectly decides 0=0" << endl;
	if( !ApproxEq(1.0/1e6, 1.0/1e6) ) cout << "ApproxEq incorrectly decides 1.0/1e6 == 1.0/1e6" << endl;
}
#endif // TEST_FLOAT_COMPARE
