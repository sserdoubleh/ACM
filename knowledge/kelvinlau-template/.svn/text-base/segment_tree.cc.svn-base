#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int N = 100010;

int f[N << 1];
bool g[N << 1];

inline int id(int u, int v) {
  return (u + v - 1) | (u + 1 < v);
}

inline int md(int u, int v) {
  return (u + v + 1) >> 1;
}

void down(int u, int v) {
  int t = id(u, v), d = md(u, v), l = id(u, d), r = id(d, v);

  if (f[t] != -1) {
    f[l] = f[t];
    f[r] = f[t];
    g[l] = g[r] = 0;
    f[t] = -1;
  }

  if (g[t]) {
    g[l] ^= 1;
    g[r] ^= 1;
    g[t] = 0;
  }
}

void set(int u, int v, int a, int b, int val) {
  int t = id(u, v), d = md(u, v);

  if (b <= u || v <= a) return;
  if (a <= u && v <= b) {
    f[t] = val;
    g[t] = 0;
    return;
  }
  down(u, v);
  set(u, d, a, b, val);
  set(d, v, a, b, val);
}

int get(int u, int v, int a) {
  int t = id(u, v), d = md(u, v);

  if (f[t] != -1) return f[t] ^ g[t];
  return g[t] ^ (a < d ? get(u, d, a) : get(d, v, a));
}

void change(int u, int v, int a, int b) {
  int t = id(u, v), d = md(u, v);

  if (b <= u || v <= a) return;
  if (a <= u && v <= b) {
    g[t] ^= 1;
    return;
  }
  down(u, v);
  change(u, d, a, b);
  change(d, v, a, b);
}

int main() {
  int n, m, q, i, a, b;
  
  while (scanf("%d %d", &n, &m) != EOF) {
    memset(f, 0, sizeof(f));
    memset(g, 0, sizeof(g));
    
    while (m--) {
      scanf("%d", &q);
      switch (q) {
        case 0:
        case 1:
          scanf("%d %d", &a, &b);
          set(0, n, a, b, q);
          break;
        case 2:
          scanf("%d %d", &a, &b);
          change(0, n, a, b);
          break;
        case 3:
          scanf("%d", &a);
          printf("%d\n", get(0, n, a));
          break;
      }
    }
  }

  return 0;
}
