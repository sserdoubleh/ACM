/* XXX This code is very buggy, don't use it! */
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
using namespace std;

typedef vector<int> VI;
typedef vector< pair<int, int> > VII;
#define pb(a) push_back(a)

inline int longMod(int a, int b, int c) {
  return (long long)a *b % c;
}

int pm(int a, int b, int m) {
  int r = 1;
  for (; b; b >>= 1, a = longMod(a, a, m))
    if (b & 1)
      r = longMod(r, a, m);
  return r;
}

int ext_gcd(int a, int b, int &x, int &y) {
  int r;
  if (!b) {
    x = 1, y = 0;
    return a;
  }
  int d = ext_gcd(b, a % b, y, x);
  y = y - a / b * x;
  return d;
}

// solve x^2 == r (mod p)
// p is an odd prime
// and r^((p-1)/1) == 1 (mod p)
// r1 and r2 is the result
void getSqr(int r, int p, int &r1, int &r2) {
  int S = 0, Q = p - 1;
  while (!(Q & 1)) {
    Q >>= 1;
    S++;
  }
  if (S == 1) {
    r1 = pm(r, (p + 1) >> 2, p);
    r2 = p - r1;
    return;
  }
  int i, j, c, R, dt, t, M, z = 1;
  do {
    z = rand() % p;
  } while (pm(z, (p - 1) >> 1, p) != p - 1);
  c = pm(z, Q, p);
  R = pm(r, (Q + 1) >> 1, p);
  t = pm(r, Q, p);
  M = S;
  while (t != 1) {
    for (i = 1, dt = longMod(t, t, p); dt != 1; i++)
      dt = longMod(dt, dt, p);
    for (j = M - i - 1; j > 0; j--)
      c = longMod(c, c, p);
    R = longMod(R, c, p);
    c = longMod(c, c, p);
    t = longMod(t, c, p);
    M = i;
  }
  r1 = R;
  r2 = p - R;
}

// know the last digit of the result
// get the whole result of
// x^2 == r (mod p^c);
// p is odd prime
int getDigit(int r, int p, int c, int last) {
  int ans = last, w = p;
  int i, pp, a, b;
  for (pp = 1, i = 1; i <= c; i++)
    pp *= p;
  int x, y;
  for (i = 2; i <= c; i++) {
    a = (last << 1) % p;
    b = (r + pp - longMod(last, last, pp)) % pp / w % p;
    ext_gcd(a, p, x, y);
    a = (p + longMod(b, x, p)) % p;
    last = last + w * a;
    w *= p;
  }
  return last;
}

// Solve x^2 == r (mod p^c)
// p is odd prime
// c>=1,r>0
void getPowerSol(int r, int p, int c, int &r1, int &r2) {
  int last;
  int i, j;
  getSqr(r, p, r1, r2);
  r1 = getDigit(r, p, c, r1);
  r2 = getDigit(r, p, c, r2);
}

// Solve x^2 = r (mod 2^c)
// r>0
void getPowerSol2(int r, int c, VI &sol) {
  sol.clear();
  if (c == 1) {
    sol.pb(r & 1);
    return;
  }
  if (c == 0) {
    sol.pb(0);
    sol.pb(1);
    return;
  }
  if (c == 2) {
    int t = r & 3;
    if (t == 3 || t == 2)
      return;
    else if (t == 1) {
      sol.pb(1);
      sol.pb(3);
    } else {
      sol.pb(0);
      sol.pb(2);
    }
    return;
  }
  int pp = (1 << c) - 1;
  r &= pp;
  if (r == 0) {
    r = 1 << ((c + 1) >> 1);
    sol.pb(0);
    while (r < pp) {
      sol.pb(r);
      r <<= 1;
    }
    return;
  }
  if (r & 1) {
    if ((r & 7) != 1) {
      return;
    }
    int i, x, d, f;
    x = 1, f = 4;
    for (i = 4; i <= c; i++) {
      x = x + ((((r - x * x) >> (i - 1)) & 1) << (i - 2));
      f <<= 1;
    }
    sol.pb(x);
    sol.pb(-x);
    sol.pb(-x - f);
    sol.pb(x + f);
    for (i = 0; i < 4; i++)
      sol[i] &= pp;
  } else {
    int fac = 1;
    while (!(r & 1) && c > 3) {
      if (r & 3) {
        return;
      }
      r >>= 2, fac <<= 1, c -= 2;
    }
    getPowerSol2(r, c, sol);
    for (int i = 0; i < sol.size(); i++) {
      sol[i] *= fac;
      sol[i] &= pp;
    }
  }
}

void divide(int n, VII &ans) {
  ans.clear();
  int i, j = 0;
  for (i = 2; i <= n / i; i++) {
    if (n % i == 0) {
      ans.pb(make_pair(i, 0));
      while (n % i == 0) {
        n /= i;
        ans[j].second++;
      }
      j++;
    }
  }
  if (n > 1)
    ans.pb(make_pair(n, 1));
}

// Using Chinese Remainder Theorem
// (m1,m2)=1
void updateRemain(const VI &org, int m1, int r2, int m2, VI &ans) {
  int x, y, M = m1 * m2, r1, R;
  ext_gcd(m1, m2, x, y);
  for (int i = 0; i < org.size(); i++) {
    r1 = org[i];
    R = (longMod(r2 * x, m1, M) + longMod(r1 * y, m2, M)) % M;
    ans.pb(R);
  }
}

void solve(int r, int n, VI &res) {
  res.clear();
  VII fac;
  r = (r % n + n) % n;
  static VI rs[2];
  rs[0].clear(), rs[1].clear();
  rs[0].pb(0);
  int i, d = 0, p, j, pp, curM = 1, r1, r2;
  if (n == 1) {
    res.pb(0);
    return;
  }
  divide(n, fac);
  static VI tmp;
  for (i = 0; i < fac.size(); i++) {
    tmp.clear();
    p = 1;
    for (j = 0; j < fac[i].second; j++)
      p *= fac[i].first;
    if (fac[i].first == 2)
      getPowerSol2(r, fac[0].second, tmp);
    else {
      if (pm(r, (fac[i].first - 1) >> 1, fac[i].first) != 1)
        return;
      getPowerSol(r, fac[i].first, fac[i].second, r1, r2);
      tmp.pb(r1);
      tmp.pb(r2);
    }
    for (j = 0; j < tmp.size(); j++)
      updateRemain(rs[d], curM, tmp[j], p, rs[d ^ 1]);
    rs[d].clear();
    curM *= p;
    d ^= 1;
  }
  res.assign(rs[d].begin(), rs[d].end());
  for (i = 0; i < res.size(); i++)
    res[i] = (res[i] % n + n) % n;
}

int main() {
  VI res;
  solve(2, 24, res);
  return 0;
}
