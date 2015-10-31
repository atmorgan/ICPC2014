#ifndef RATIONAL_CC
#define RATIONAL_CC
#include "Algebra.cc"

// BEGIN
// Rational struct. Uses lcm to keep in simplified form.
// Simply replace "double" (or "int") with "rat" and use
// contructor to initialize constants. Rat class handles
// everything else.
//
// Written in such a way as to avoid overflow if possible.

struct rat {
    T n, d;

    rat(T n, T d) {
        T k = gcd(n, d);
        this->n = n/k;
        this->d = d/k;
    }

    rat(T n) : n(n), d(1) {}
};

rat operator + (const rat &a, const rat &b) {
    T new_d = lcm(a.d, b.d); // if overflow occurs, this may be 0
                             // causing floating point exception
    T a_scale = new_d / a.d;
    T b_scale = new_d / b.d;
    
    return rat(a.n*a_scale + b.n*b_scale, new_d);
}

rat operator * (const T s, const rat &a) {
    return rat(a.n * s, a.d);
}

rat operator * (const rat &a, const T s) {
    return s*a;
}

rat operator - (const rat &a, const rat &b) {
    return a + (-1 * b);
}

rat operator * (const rat &a, const rat &b) {
    return rat(a.n*b.n, a.d*b.d); 
}

rat operator / (const rat &a, const rat &b) {
    return a * rat(b.d, b.n);
}

// END

#ifdef BUILD_TEST_RATIONAL
int main() {
    //TODO: implement tests
    //This code has successfully been used and accepted by UVa Online Judge
    //Dice Throwing
    return 0;
}
#endif // BUILD_TEST_RATIONAL
#endif // RATIONAL_CC
