/* f[i] = a[i], i < m; 
 * f[n] = b[0] * f[n - m] + ... + b[m - 1] * f[n - 1]; 
 * given a[], b[], m, n; find f[n]
 * O(M ^ 2 log N) */
#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

typedef long long ll;
const int M = 1010;
const ll R = 1000000003;

int n, m;
ll a[M], b[M], c[M + M][M], v[M], t[M + M];

void f(int n, ll v[]) {
  if (n < m) {
    memset(v, 0, sizeof(ll) * m);
    v[n] = 1;
    return;
  }

  f(n / 2, v);
  memset(t, 0, sizeof(ll) * (m + m));
  for (int i = 0; i < m; i++)
    for (int j = 0; j < m; j++) {
      t[i + j] += v[i] * v[j];
      t[i + j] %= R;
    }
  for (int i = m; i <= m + m - 2; i++)
    for (int j = 0; j < m; j++) {
      t[j] += c[i][j] * t[i];
      t[j] %= R;
    }
  memcpy(v, t, sizeof(ll) * m);

  if (n % 2) {
    t[0] = 0;
    memcpy(t + 1, v, sizeof(ll) * m);
    for (int j = 0; j < m; j++) {
      t[j] += t[m] * c[m][j];
      t[j] %= R;
    }
    memcpy(v, t, sizeof(ll) * m);
  }
}

int main() {
  scanf("%d %d", &m, &n);
  for (int i = 0; i < m; i++)
    scanf("%lld", &a[i]);
  for (int i = 0; i < m; i++)
    scanf("%lld", &b[i]);

  for (int i = 0; i < m; i++)
    for (int j = 0; j < m; j++)
      c[i][j] = i == j;
  for (int j = 0; j < m; j++)
    c[m][j] = b[j];
  for (int i = m + 1; i <= m + m - 2; i++) {
    c[i][0] = c[i - 1][m - 1] * c[m][0] % R;
    for (int j = 1; j < m; j++)
      c[i][j] = (c[i - 1][j - 1] + c[i - 1][m - 1] * c[m][j]) % R;
  }

  f(n, v);

  ll res = 0;
  for (int i = 0; i < m; i++) {
    res += a[i] * v[i];
    res %= R;
  }
  printf("%lld\n", res);

  return 0;
}
