#include <cmath>
#include <algorithm>
using namespace std;
// BEGIN
// Short function for comparing floating point numbers.

const double EPS_ABS    = 1e-10;  // for values near 0.0. Keep small.
const double EPS_REL    = 1e-8;   // for values NOT near 0.0. Balance.

bool feq( double a, double b ) {
	double d = fabs(b-a);
	if( d <= EPS_ABS ) return true;
	if( d <= max(fabs(a),fabs(b))*EPS_REL ) return true;
	return false;
}

bool flt( double a, double b ) {
	return !feq(a,b) && a < b;
}
// END

#ifdef TEST_FLOAT_COMPARE
#include <iostream>
int main() {
	if(  feq(1.0/1e6, 0.0) )     cout << "feq wrong near 0" << endl;
	if( !feq(0.0,     0.0) )     cout << "feq incorrectly decides 0=0" << endl;
	if( !feq(1.0/1e6, 1.0/1e6) ) cout << "feq incorrectly decides 1.0/1e6 == 1.0/1e6" << endl;
}
#endif // TEST_FLOAT_COMPARE
