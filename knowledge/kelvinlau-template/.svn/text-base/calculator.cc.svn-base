#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <algorithm>
using namespace std;

const int INTMAX = 0x7fffffff;
const int INTMIN = 0x80000000;
typedef long long ll;

int x, ok;
struct Token { 
  int t, val;
} lookahead;

inline bool fit(ll x) { 
  return INTMIN <= x && x <= INTMAX; 
}

Token next_token() {
  Token tok;
  ll val;

  while (isspace(x)) 
    x = getchar();

  if (x == EOF) {
    tok.t = EOF;
    return tok;
  }

  if (strchr("()+-*/%^", x)) {
    tok.t = x;
    x = getchar();
    return tok;
  }
  
  for (val = 0; isdigit(x); x = getchar()) {
    val = val * 10 + x - '0';
    ok &= fit(val);
  }
  tok.t = 'n';
  tok.val = (int)val;
  return tok;
}

inline void shift() { 
  lookahead = next_token(); 
}

void init() {
  x = getchar(); 
  shift();
}

int match(int t) {
  if (lookahead.t == t) {
    shift();
    return 1;
  }
  return 0;
}

int exp();
int term();
int factor();
int unit();

int exp() {
  ll ans = term();
  while (1) {
    if (match('+')) {
      ans += term();
    } else if (match('-')) {
      ans -= term();
    } else break;
    
    ok &= fit(ans);
  }
  return (int)ans;
}

int term() {
  ll t, ans = factor();
  while (1) {
    if (match('*')) {
      ans *= factor();
    } else if (match('/')) {
      if (t = factor())
        ans /= t;
      else
        ok = 0;
    } else if (match('%')) {
      if (t = factor())
        ans %= t;
      else
        ok = 0;
    } else break;
    ok &= fit(ans);
  }
  return (int)ans;
}

int pow(int x, int y) {
  ll r = 1;
  if (x == 0 && y == 0 || y < 0) return ok = 0;
  if (x == 0 || x == 1) return x;
  if (x == -1) y %= 2;
  
  while (y--) {
    r *= x;
    if (!fit(r))
      return ok = 0;
  }
  return (int)r;
}

int factor() {
  int ans = unit();
  if (match('^'))
    ans = pow(ans, factor());
  return ans;
}

int unit() {
  ll ans;
  int sign;

  sign = 1;
  while (match('-'))
    sign *= -1;

  if (lookahead.t == 'n') {
    ans = sign * lookahead.val;
    shift();
    ok &= fit(ans);
    return ans;
  }

  if (match('(')) {
    ans = sign * exp();
    ok &= fit(ans);
    if (match(')'))
      return ans;
  }

  return ok = 0;
}

int main() {
  int val;

  init();
  while (lookahead.t != EOF) {
    ok = 1;
    val = exp();
    if (ok)
      printf("%d\n", val);
    else
      puts("ERROR!");
  }
  return 0;
}
