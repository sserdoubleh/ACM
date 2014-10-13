#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int M = 1000010;

int fp[M], prime[M], pn;

/* O(M) */
void init() {
  pn = 0;
  for (int i = 2; i < M; i++) {
    if (!fp[i]) fp[i] = prime[pn++] = i;
    for (int j = 0; prime[j] * i < M; j++) {
      fp[prime[j] * i] = prime[j];
      if (i % prime[j] == 0) break;
    }
  }
}

/* assumes that n < M */
void factorize1(int n, int p[], int k[], int &m) {
  for (m = 0; n > 1; m++) {
    p[m] = fp[n], k[m] = 0;
    while (n % p[m] == 0) 
      n /= p[m], k[m]++;
  }
}

/* assumes that n < M * M */
void factorize(int n, int p[], int k[], int &m) {
  m = 0;
  for (int i = 0; n >= M && prime[i] * prime[i] <= n; i++)
    if (n % prime[i] == 0) {
      p[m] = prime[i], k[m] = 0;
      while (n % p[m] == 0)
        n /= p[m], k[m]++;
      m++;
    }
  if (n < M)
    for (; n > 1; m++) {
      p[m] = fp[n], k[m] = 0;
      while (n % p[m] == 0) 
        n /= p[m], k[m]++;
    }
  if (n > 1)
    p[m] = n, k[m++] = 1;
}

void find_divisors(int p[], int k[], int m, int d[], int &dn) {
  dn = 0; d[dn++] = 1;
  for (int i = 0; i < m; i++) {
    for (int j = 0, z = dn; j < k[i] * z; j++, dn++)
      d[dn] = d[dn - z] * p[i];
  }
  sort(d, d + dn);
}
