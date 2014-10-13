#include <stdio.h>
using namespace std;

typedef long long ll;

int euler(int a, int p) {
  ll ret = 1, s = a, k = (p - 1) / 2;
  while (k) {
    if (k & 1) ret = (ret * s) % p;
    s = (s * s) % p;
    k = (k >> 1);
  }
  return ret != 1 ? 0 : 2;
}    


int cal(int p, int n, int d) {
  int pn = 1;
  for (int i = 0; i < n; i++) pn *= p;
  d %= pn;
  if (d == 0) {
    int k = 1;
    for (int i = 0; i < n / 2; i++) k *= p;
    return k;
  }
  if (p == 2) {
    int r, b = 0, pr, pb;
    while (d % p == 0) {
      d /= p;
      b++;
    }
    if (b % 2 != 0) return 0;
    r = b / 2;
    pr = 1;
    for (int i = 0; i < r; i++) pr *= p;
    n -= b;
    if (n < 2) return 1 * pr;
    if (n == 2) {
      if (d % 4 == 1) return 2 * pr;
    }
    if (n > 2) {
      if (d % 8 == 1) return 4 * pr;
    }
    return 0;
  }
  if (p != 2) {
    int r, b = 0, pr, pb;
    while (d % p == 0) {
      d /= p;
      b++;
    }
    if (b % 2 != 0) return 0;
    r = b / 2;
    pr = 1;
    for (int i = 0; i < r; i++) pr *= p;
    return pr * euler(d, p);
  }
  return 0;
}

/* number of x such that x ^ 2 = d (% m) */
int solve(int m, int d) {
  int ret = 1;
  for (int i = 2; i * i <= m; i++) if (m % i == 0) {
    int j = 0;
    int q = 1;
    while (m % i == 0) {
      m /= i;
      j++;
      q *= i;
    }
    ret *= cal(i, j, d);
  }
  if (m > 1)
    ret *= cal(m, 1, d);
  return ret;
}
    
int main() {
  int m, d;
  while (scanf("%d %d", &m, &d) != EOF) {
    printf("%d\n", solve(m, d));
  }
  return 0;
}
