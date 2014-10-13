#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int MAXLEN = 110, BASE = 10000;

struct bignum {
  int d[MAXLEN / 4], n, s;
  bignum(int x = 0) {
    if (x < 0) s = 1, x = -x; else s = 0;
    for (n = 0; x; x /= BASE)
      d[n++] = x % BASE;
  }
  bignum(const char *str) {
    if (isdigit(*str)) s = 0;
    else s = (*str++ == '-');

    int k = strlen(str);
    n = (k + 3) / 4;
    fill(d, d + n, 0);
    for (int i = n - 1, j = 0; j < k; j++) {
      d[i] = d[i] * 10 + str[j] - '0';
      if ((j + 1 - k) % 4 == 0) i--;
    }
    while (n > 0 && d[n - 1] == 0) n--;
  }
  int read() {
    char str[MAXLEN];
    if (scanf("%s", str) == EOF) return 0;
    *this = bignum(str);
    return 1;
  }
  bignum(const bignum& x) {
    n = x.n;
    s = x.s;
    copy(x.d, x.d + n, d);
  }
  static bignum exp(int len) {
    bignum C;
    C.n = len + 1;
    fill(C.d, C.d + len, 0);
    C.d[len] = 1;
    return C;
  }
  int length() {
    if (!n) return 0;
    int res = 4 * n - 4;
    for (int x = d[n - 1]; x; x /= 10) res++;
    return res;
  }
  void println() const {
    if (s && n)
      printf("-");
    if (n > 0) {
      printf("%d", d[n-1]);
      for (int i = n - 2; i >= 0; i--)
        printf("%04d", d[i]);
      printf("\n");
    } else puts("0");
  }
};

bignum operator-(bignum A) {
  A.s ^= 1;
  return A;
}

int cmp(const bignum& A, const bignum& B) {
  if (!A.n && !B.n) return 0;
  if (!A.n) return B.s ? +1 : -1;
  if (!B.n) return A.s ? -1 : +1;
  if (A.s ^ B.s) return A.s ? -1 : +1;
  if (A.s && B.s) return cmp(-B, -A);
  if (A.n != B.n) return A.n - B.n;

  for (int i = A.n - 1; i >= 0; i--) {
    if (A.d[i] < B.d[i]) return -1;
    if (A.d[i] > B.d[i]) return +1;
  }
  return 0;
}

bool operator< (const bignum& A, const bignum& B) { return cmp(A, B) <  0; }
bool operator> (const bignum& A, const bignum& B) { return cmp(A, B) >  0; }
bool operator==(const bignum& A, const bignum& B) { return cmp(A, B) == 0; }
bool operator!=(const bignum& A, const bignum& B) { return cmp(A, B) != 0; }
bool operator<=(const bignum& A, const bignum& B) { return cmp(A, B) <= 0; }
bool operator>=(const bignum& A, const bignum& B) { return cmp(A, B) >= 0; }

bignum operator+(const bignum&, const bignum&);
bignum operator-(const bignum&, const bignum&);
bignum operator*(const bignum&, const bignum&);
bignum operator/(const bignum&, const bignum&);
bignum operator%(const bignum&, const bignum&);

bignum& operator+=(bignum& a, const bignum& b) { return a = a + b; }
bignum& operator-=(bignum& a, const bignum& b) { return a = a - b; }
bignum& operator*=(bignum& a, const bignum& b) { return a = a * b; }
bignum& operator/=(bignum& a, const bignum& b) { return a = a / b; }
bignum& operator%=(bignum& a, const bignum& b) { return a = a % b; }

bignum operator+(const bignum& A, const bignum& B) {
  if (A.s && B.s) return -((-A) + (-B));
  if (A.s) return B - (-A);
  if (B.s) return A - (-B);

  bignum C; 
  C.n = max(A.n, B.n);
  int carry = 0;
  for (int i = 0; i < C.n; i++) {
    carry += (i < A.n ? A.d[i] : 0) + (i < B.n ? B.d[i] : 0);
    C.d[i] = carry % BASE;
    carry /= BASE;
  }
  if (carry) C.d[C.n++] = carry;
  return C;
}

bignum operator-(const bignum& A, const bignum& B) {
  if (A.s && B.s) return -((-A) - (-B));
  if (A.s) return -((-A) + B);
  if (B.s) return A + (-B);
  if (A < B) return -(B - A);

  bignum C; C.n = max(A.n, B.n);
  int carry = 0;
  for (int i = 0; i < C.n; i++) {
    carry += (i < A.n ? A.d[i] : 0) - (i < B.n ? B.d[i] : 0);
    C.d[i] = (carry + BASE) % BASE;
    carry = carry < 0 ? -1 : 0;
  }
  while (C.n > 0 && C.d[C.n - 1] == 0) C.n--;
  return C;
}

bignum operator*(const bignum& A, const bignum& B) {
  bignum C; C.n = A.n + B.n; C.s = A.s ^ B.s;
  fill(C.d, C.d + C.n, 0);
  for (int i = 0; i < A.n; i++)
    for (int j = 0; j < B.n; j++) {
      C.d[i + j] += A.d[i] * B.d[j];
      C.d[i + j + 1] += C.d[i + j] / BASE;
      C.d[i + j] %= BASE;
    }
  while (C.n > 0 && C.d[C.n - 1] == 0) C.n--;
  return C;
}

void div(bignum A, bignum B, bignum &Q, bignum &R) {
  bignum q, p;
  int s = A.s ^ B.s;
  A.s = B.s = 0;
  R = A;
  
  while (R >= B) {
    q = bignum::exp(max(R.n - B.n - 1, 0));
    p = q * B;
    if (p * BASE <= R) { p *= BASE; q *= BASE; }
    
    int u = 1;
    int v = BASE;
    while (u + 1 < v) {
      int k = (u + v) / 2;
      if (p * k <= R) u = k; else v = k;
    }
    
    Q += q * u;
    R -= p * u;
  }
  R.s = Q.s = s;
}

bignum operator/(const bignum& A, const bignum& B) {
  bignum Q, R;
  div(A, B, Q, R);
  return Q;
}

bignum operator%(const bignum& A, const bignum& B) {
  bignum Q, R;
  div(A, B, Q, R);
  return R;
}
