// From http://web.mit.edu/~ecprice/acm/acm07/
// DOES NOT DO DIVISION OR MOD YET

#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <malloc.h>
#include <cmath>
#include <set>
using namespace std;

const int BASE = 10000;

class bigint {
public:
  bigint();
  bigint(int);
  bigint(int*, int);
  bigint(const string &);
  bigint(const bigint &);
  ~bigint();

  void push_back(int x);
  void init();
  void cleanup();
  void wipe();

  int* nums;
  int size;
  int alloc;
};

ostream& operator << (ostream& os, const bigint &a) {
  os << a.nums[a.size-1];
  for (int i = a.size-2; i >= 0; i--) {
    int x = abs(a.nums[i]);
    if (x < 1000) os << 0;
    if (x < 100) os << 0;
    if (x < 10) os << 0;
    os << x;
  }
  return os;
}

void bigint::push_back(int x) {
  if (size == alloc) {
    nums = (int*)realloc(nums, sizeof(int) * (alloc <<= 1));
  }
  nums[size++] = x;
}

void bigint::init() {
  alloc = 16;
  nums = (int*)malloc(sizeof(int) * alloc);
  if (!nums) {
    abort();
  }
  size = 0;
}

void bigint::cleanup() {
  //clean up numbers with abs value >= BASE
  for (int i = 0; i < size - 1; i++) {
    if (abs(nums[i]) >= BASE) {
      nums[i+1] += nums[i] / BASE;
      nums[i] %= BASE;
    }
  }
  //carries in the last digit
  while (abs(nums[size-1]) >= BASE) {
    push_back(nums[size-1] / BASE);
    nums[size-2] %= BASE;
  }

  //clean up sign convention
  //negative numbers must have all elements <= 0,
  //positive have all elements >= 0
  bool neg = 0;
  int at = size - 1;

  while (at >= 0) {
    if (nums[at]) {neg = (nums[at] < 0); break;}
    at--;
  }
  if (at < 0) {
    size = 1;
    return;
  }

  if (neg) {
    for (int i = 0; i <= at; i++) {
      if (nums[i] > 0) {
        nums[i] -= BASE;
        nums[i+1]++;
      }
    }
  } else {
    for (int i = 0; i <= at; i++) {
      if (nums[i] < 0) {
        nums[i] += BASE;
        nums[i+1]--;
      }
    }
  }

  //remove leading zeros, but leave "0"
  while (!nums[size-1] && size > 1) size--;
}

bigint::bigint() {
  init();
  nums[0] = 0;
  size = 1;
}

bigint::bigint(int reserved) {
  int x = 16;
  while (x <= reserved) x <<= 1;
  nums = (int*) malloc(sizeof(int) * x);
  nums[0] = 0;
  size = 1;
  alloc = x;
}

bigint::bigint(int* pop, int num) {
  int x = 1;
  while (x <= num) x *= 2;
  nums = (int*) malloc(sizeof(int) * x);
  memcpy(nums, pop, sizeof(int) * num);
  size = num;
  alloc = x;
}

bigint::bigint(const string &x) {
  init();
  int start = 0;
  bool neg = x[0] == '-';
  string y;

  if (neg) {
    int need = (4 - ((x.size() - 1) & 3)) % 4;
    for (int i = 0; i < need; i++) y += "0";
    y += x.substr(1);

    for (int i = y.size() - 4; i >= 0; i -= 4) {
      int foo=(y[i]-'0')*1000+(y[i+1]-'0')*100+(y[i+2]-'0')*10 +(y[i+3]-'0');
      push_back(-foo);
    }    
  } else {
    int need = (4 - (x.size() & 3)) % 4;
    for (int i = 0; i < need; i++) y += "0";
    y += x;

    for (int i = y.size() - 4; i >= 0; i -= 4) {
      int foo=(y[i]-'0')*1000+(y[i+1]-'0')*100+(y[i+2]-'0')*10 +(y[i+3]-'0');
      push_back(foo);
    }
  }
}

bigint::bigint(const bigint &b) {
  nums = (int*)malloc(sizeof(int) * b.alloc);
  memcpy(nums, b.nums, sizeof(int) * b.size);
  alloc = b.alloc;
  size = b.size;
}

bigint::~bigint() {
  if (nums)
    free(nums);
}

bigint operator + (const bigint &a, const bigint &b) {
  int S = (a.size >? b.size) + 1;
  bigint ret(S);
  ret.size = S;
  memset(ret.nums, 0, S*sizeof(int));
  for (int i = 0; i < a.size; i++) ret.nums[i] += a.nums[i];
  for (int i = 0; i < b.size; i++) ret.nums[i] += b.nums[i];
  ret.cleanup();
  return ret;
}
bigint operator - (const bigint &a, const bigint &b) {
  int S = (a.size >? b.size) + 1;
  bigint ret(S);
  ret.size = S;
  memset(ret.nums, 0, S*sizeof(int));
  for (int i = 0; i < a.size; i++) ret.nums[i] += a.nums[i];
  for (int i = 0; i < b.size; i++) ret.nums[i] -= b.nums[i];
  ret.cleanup();
  return ret;
}

const int KARATSUBA_LIMIT = 100;
bigint operator * (const bigint&, const bigint &);
bigint* mul (bigint *, bigint *);

bigint longmul(const bigint &a, const bigint &b) {
  int S = a.size + b.size;
  bigint ret(S);
  ret.size = S;
  memset(ret.nums, 0, sizeof(int) * S);
  for (int i = 0; i < a.size; i++)
    for (int j = 0; j < b.size; j++) {
      ret.nums[i+j] += a.nums[i] * b.nums[j];
      if (abs(ret.nums[i+j]) >= 2047000000) {
	ret.nums[i+j+1] += ret.nums[i+j] / BASE;
	ret.nums[i+j] %= BASE;
      }
    }
  ret.cleanup();
  return ret;
}

bigint karatsuba(const bigint &a, const bigint &b) {
  int bs = (a.size + 1) >> 1;
  bigint B(a.nums, bs);
  bigint A(a.nums+bs, a.size-bs);
  bigint D(b.nums, bs);
  bigint C(b.nums+bs, b.size-bs);

  bigint AC = A*C, BD = B*D;
  bigint mid = ((A+B)*(C+D)) - (AC + BD);
  int S = 2*bs+AC.size + 2;

  bigint ret(S); ret.size = S;
  memset(ret.nums, 0, S*sizeof(int));

  for (int i = 0; i < BD.size; i++) ret.nums[i] += BD.nums[i];
  for (int i = 0; i < mid.size; i++) ret.nums[i+bs] += mid.nums[i];
  for (int i = 0; i < AC.size; i++) ret.nums[i+2*bs] += AC.nums[i];
  ret.cleanup();
  return ret;
}

bigint karatsuba_prep(const bigint &a, const bigint &b) {
  bigint foo = a, bar = b;
  while (foo.size < bar.size) foo.push_back(0);
  while (foo.size > bar.size) bar.push_back(0);
  return karatsuba(foo,bar);
}

bigint operator * (const bigint &a, const bigint &b) {
  if (a.size <= KARATSUBA_LIMIT || b.size <= KARATSUBA_LIMIT)
    return longmul(a,b);
  else
    return karatsuba_prep(a,b);
}
