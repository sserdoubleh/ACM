#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

typedef long long ll;

const ll R = 1000000007;
const int N = 1000 + 10;

int n, q;
ll fs[N][N], xs[N], ys[N];

inline ll modular(ll x, ll y) {
  return (x % y + y) % y;
}

ll exgcd(ll m, ll n, ll &a, ll &b) { 
  if (!n) return a = 1, b = 0, m;
  ll d = exgcd(n, m % n, b, a);
  b -= m / n * a;
  return d;
}

ll invert(ll x, ll m)  { 
  ll a, b;
  exgcd(x, m, a, b);
  return modular(a, m);
}

ll f(ll x) {
  ll y = ys[0];
  ll p = 1;
  for (int i = 1; i < n; i++) {
    p = modular(p * (x - xs[i - 1]), R);
    y = modular(y + p * fs[0][i], R);
  }
  return y;
}

int main() {

  scanf("%d %d", &n, &q);
  for (int i = 0; i < n; i++)
    scanf("%lld %lld", &xs[i], &ys[i]);

  for (int i = 0; i < n; i++)
    fs[i][i] = ys[i];

  for (int d = 1; d < n; d++)
    for (int i = 0, j = d; j < n; i++, j++)
      fs[i][j] = modular(fs[i + 1][j] - fs[i][j - 1], R) * invert(d, R) % R;

  while (q--) {
    ll x; scanf("%lld", &x);
    printf("%lld\n", f(x));
  }

  return 0;
}
