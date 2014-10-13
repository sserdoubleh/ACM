#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <math.h>
using namespace std;

typedef long long ll;

ll gcd(ll x, ll y) { 
  return !y ? x : gcd(y, x % y);
}

ll modular(ll a, ll b) { 
  return (a % b + b) % b; 
}

/** m * a + n * b == gcd(m, n) */
ll exgcd(ll m, ll n, ll &a, ll &b) { 
  if (!n)
    return a = 1, b = 0, m;

  ll d = exgcd(n, m % n, b, a);
  b -= m / n * a;
  return d;
}

/** x * y % m == 1 */
ll invert(ll x, ll m)  { 
  ll a, b;
  exgcd(x, m, a, b);
  return modular(a, m);
}

/** x % m == a && x % n == b */
ll modular_system(ll m, ll a, ll n, ll b) { 
  ll g, k, l;

  g = exgcd(m, n, k, l);
  if ((a - b) % g) return -1;
  k *= (b - a) / g;
  k = modular(k, n / g);
  return modular(k * m + a, m / g * n);
}

/** x % m[i] == r[i] */
ll modular_system_array(ll m[], ll r[], int k) {
  ll M = m[0], R = r[0];
  for (int i = 1; R != -1 && i < k; i++) {
    R = modular_system(M, R, m[i], r[i]);
    M = M / gcd(M, m[i]) * m[i];
  }
  return R;
}

/** a * x % m == b */
ll modular_equation(ll a, ll m, ll b) { 
  return modular_system(m, b, a, 0) / a % m;
}

/** calculate r = x ^ y % m */
ll modular_pow(ll x, ll y, ll m) {
  ll r = 1 % m;
  for (; y; y >>= 1, x = x * x % m)
    if (y & 1) r = r * x % m;
  return r;
}

/** a ^ x % m == b */
ll modular_log(ll a, ll b, ll m) {
  typedef pair<ll, ll> pp;
  static pp table[10006];

  ll s = (ll)ceil(sqrt(m));
  for (ll j = 0, p = 1; j < s; j++, p = p * a % m)
    table[j] = pp(p, j);
  stable_sort(table, table + s);

  ll c = invert(modular_pow(a, s, m), m); // c = a ^ (-m)

  for (ll i = 0; i < m; i++, b = b * c % m) {
    pp *p = lower_bound(table, table + s, pp(b, -1));
    if (p < table + s && p->first == b) return i * s + p->second;
  }
  return -1;
}
