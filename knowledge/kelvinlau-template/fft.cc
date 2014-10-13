#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <algorithm>

typedef long long ll;

int is_power_of_two(int n) {
  return !(n & (n - 1));
}

int next_power_of_two(int n) {
  int p = 1;
  while (p < n)
    p <<= 1;
  return p;
}

int modular_power(int x, int y, int R) {
  int r = 1;
  while (y) {
    if (y & 1)
      r = (ll)r * x % R;
    x = (ll)x * x % R;
    y >>= 1;
  }
  return r;
}

int inv(int x, int R) {
  return modular_power(x, R - 2, R);
}

char to_char(int d) {
  if (d < 10)
    return '0' + d;
  else if (d < 36)
    return 'a' + d - 10;
}

int to_digit(char c) {
  if (isdigit(c))
    return c - '0';
  else if (islower(c))
    return c - 'a' + 10;
  else if (isupper(c))
    return c - 'A' + 10;
  else
    return 0;
}

void eval(const int p[], int n, int x, char bignum[]) {
  int carry = 0;
  for (int i = 0; i < n; i++) {
    carry += p[i];
    bignum[i] = to_char(carry % 10);
    carry /= 10;
  }
  int l = n;
  while (l && bignum[l - 1] == '0') l--;
  if (l) {
    std::reverse(bignum, bignum + l);
    bignum[l] = 0;
  } else {
    bignum[1] = 0;
  }
}

void to_vector(const char bignum[], int v[], int n) {
  int l = strlen(bignum);
  for (int i = 0, j = l - 1; i < n; i++, j--)
    v[i] = j >= 0 ? to_digit(bignum[j]) : 0;
}

void mul(const int a[], const int b[], int c[], int n, int R) {
  for (int i = 0; i < n; i++)
    c[i] = (ll)a[i] * b[i] % R;
}

int is_prime(int n) {
  if (n % 2 == 0)
    return 0;
  for (int p = 3; p * p <= n; p += 2)
    if (n % p == 0)
      return 0;
  return 1;
}

int is_primitive_root(int R, int n, int w) {
  int p = 1;
  for (int k = 1; k < n; k++) {
    p = (ll)p * w % R;
    if (p == 1)
      return 0;
  }
  p = (ll)p * w % R;
  return p == 1;
}

int udiv(int x, int y) {
  return (x + y - 1) / y;
}

void find_modular_and_omega(int n, int lb, int &R, int &w) {
  int m = udiv(lb - 1, n);
  R = m * n + 1;
  while (1) {
    if (is_prime(R) && R >= lb) {
      for (w = 2; w < R; w++)
        if (is_primitive_root(R, R - 1, w)) {
          w = modular_power(w, m, R);
          return;
        }
    }
    R += n;
    m++;
  }
}

void __fft(const int v[], int R, int ws, int n, int s, int y[]) {
  if (n == 1) {
    y[0] = v[0];
  } else {
    __fft(v,     R, (ll)ws * ws % R, n / 2, 2 * s, y);
    __fft(v + s, R, (ll)ws * ws % R, n / 2, 2 * s, y + n / 2);
    ll wsk = 1;
    for (int k1 = 0, k2 = n / 2; k1 < n / 2; k1++, k2++) {
      int t1 = y[k1];
      int t2 = y[k2];
      y[k1] = (t1 + wsk * t2) % R;
      y[k2] = ((t1 - wsk * t2) % R + R) % R;
      wsk = wsk * ws % R;
    }
  }
}

void fft(const int v[], int n, int R, int w, int y[]) {
  assert(is_power_of_two(n));
  __fft(v, R, w, n, 1, y);
}

void ifft(const int v[], int n, int R, int w, int y[]) {
  assert(is_power_of_two(n));
  w = inv(w, R);
  __fft(v, R, w, n, 1, y);
  int n1 = inv(n, R);
  for (int i = 0; i < n; i++)
    y[i] = (ll)y[i] * n1 % R;
}

const int N = 1 << 17;
const int BASE = 10;

int n, R, w;
char A[N + 1];
char B[N + 1];
char C[N + 1];
int a[N], fa[N];
int b[N], fb[N];
int c[N], fc[N];

int main() {
  scanf("%s %s", &A, &B);
  n = next_power_of_two(std::max(strlen(A), strlen(B)) * 2);
  to_vector(A, a, n);
  to_vector(B, b, n);
  find_modular_and_omega(n, (BASE - 1) * (BASE - 1) * n, R, w);
  assert(is_primitive_root(R, n, w));
  fft(a, n, R, w, fa);
  fft(b, n, R, w, fb);
  mul(fa, fb, fc, n, R);
  ifft(fc, n, R, w, c);
  eval(c, n, BASE, C);
  printf("%s\n", C);
  return 0;
}
