using namespace std;
// BEGIN
// Combination.
#define mp make_pair
#define pb push_back
#define lmax 2147483647 //32 bit max signed!
#define lmin -2147483647 //32 bit min
#define ulmax 4294967295 //32 bit unsigned max(2^32 - 1)!
#define mx (1<<20)
#define md 1000000009
int f[mx+10]; // count factorials before use!!
/* This function calculates (a^b)%MOD */
long long pow(int a, int b, int MOD) {
    long long x = 1, y = a; 
    while (b > 0) {
        if(b%2 == 1) {
            x = (x*y);
            if (x>MOD) x %= MOD;
        }
        y = (y*y);
        if (y>MOD) y%=MOD; 
        b /= 2;
    }
    return x;
}

/*  Modular Multiplicative Inverse
    Using Euler's Theorem
    a^(phi(m)) = 1 (mod m)
    a^(-1) = a^(m-2) (mod m) */
long long InverseEuler(int n, int MOD){
    return pow(n, MOD-2, MOD);
}
 
long long C(int n, int r, int MOD) {
	if (r > n)
		return 0;
    return (f[n]*((InverseEuler(f[r], MOD) * InverseEuler(f[n-r], MOD)) % MOD)) % MOD;
}
int pw[21];
int main(int argc, char *argv[]) {
    f[0] = pw[0] = 1;
    for (int i = 1; i <= mx; i++)
    	f[i] = (1ll*f[i-1]*i)%md;
    for (int i = 1; i <= 20; i++)
    	pw[i] = (1<<i);
    int k;
    scanf("%d", &k);
    for (int i = 1; i <= (1<<k); i++) {
    	long long a = (1ll*2*C(i-1, pw[k-1]-1, md))%md;
    	long long ff = (1ll*f[pw[k-1]]*f[pw[k-1]])%md;
    	long long b = ( 1ll*((1ll*a)%md)*ff)%md;
    	cout<<b<<endl;
     }

    return 0;
}
// END
