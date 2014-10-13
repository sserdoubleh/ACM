#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long ll;

ll modular_multiply(ll x, ll y, ll m) {
  if (x < y) swap(x, y);

  ll r = 0;
  while (y) {
    if (y & 0x1)
      r = (r + x) % m;
    x = (x << 1) % m;
    y >>= 1;
  }
  return r;
}

ll modular_power(ll x, ll y, ll m) {
  ll r = 1;
  while (y) {
    if (y & 0x1)
      r = modular_multiply(r, x, m);
    x = modular_multiply(x, x, m);
    y >>= 1;
  }
  return r;
}

int fermat_check(ll n, int k = 10) {
  if (n <= 3)
    return n > 1;

  while (k--) {
    ll a = rand() % (n - 3) + 2;
    if (modular_power(a, n - 1, n) != 1)
      return 0;
  }
  return 1;
}

int miller_rabin(ll n, int k = 10) {
  ll d, x;
  int s, r;

  if (n <= 3)
    return n > 1;

  if (n % 2 == 0)
    return 0;

  d = n - 1;
  s = 0;
  while (!(d & 1)) {
    d >>= 1;
    s++;
  }

  while (k--) {
    x = modular_power(rand() % (n - 3) + 2, d, n);
    if (x == 1 || x == n - 1)
      continue;

    for (r = 1; r < s; r++) {
      x = modular_multiply(x, x, n);
      if (x == 1) return 0;
      if (x == n - 1) break;
    }
    if (r == s)
      return 0;
  }
  return 1;
}

ll gcd(ll x, ll y) {
  return !y ? x : gcd(y, x % y);
}

ll f(ll x, ll m, ll c) {
  return (modular_multiply(x, x, m) + c) % m;
}

ll pollard_rho(ll n) {
  if (!(n & 1))
    return 2;

  while (1) {
    ll x = rand() % n;
    ll y = x;
    ll c = rand() % n;
    ll d = 1;
    while (1) {
      x = f(x, n, c);
      y = f(f(y, n, c), n, c);
      d = gcd(y > x ? y - x : x - y, n);
      if (d == n) break;
      if (d > 1) return d;
    }
  }
}

void factorize(ll n, vector<ll> &r) {
  if (miller_rabin(n)) {
    if (n != 1)
      r.push_back(n);
  } else {
    ll d = pollard_rho(n);
    factorize(d, r);
    factorize(n / d, r);
  }
}

void find_divisors(ll n, vector<ll> &divisors) {
  vector<ll> factors;
  factorize(n, factors);
  sort(factors.begin(), factors.end());
  
  divisors.clear(); divisors.push_back(1);
  for (int i = 0, j = 0; i < factors.size(); i = j) {
    while (j < factors.size() && factors[i] == factors[j]) j++;

    ll p = factors[i];
    int k = j - i;
    int o = divisors.size();
    for (int z = 0; z < k * o; z++)
      divisors.push_back(divisors[divisors.size() - o] * p);
  }
}
