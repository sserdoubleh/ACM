#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int N = 100010;
typedef long long ll;

int en, hd[N];
struct edge { int v, w, p; } ed[N + N];
int dist[N], h[N], pnt[N][15], dmax, q[N], s, t;

void add(int u, int v, int w) {
  ed[en] = (edge) { v, w, hd[u] }; // XXX g++ only!!
  hd[u] = en++;
}

void init(int n) {
  en = 0;
  memset(hd, -1, sizeof(int) * n);
}

int jump(int v, int d) {
  int i;
  
  for (i = 0; d; i++) {
    if (d & 1) v = pnt[v][i];
    d >>= 1;
  }
  return v;
}

int lca(int u, int v) {
  int i, d, x, y;
  
  if (h[u] > h[v]) swap(u, v);

  v = jump(v, h[v] - h[u]);
  if (u == v) return v;

  for (i = dmax; i >= 0; i--) {
    x = pnt[u][i];
    y = pnt[v][i];
    if (x != y) {
      u = x;
      v = y;
    }
  }

  return pnt[u][0];
}

int main() {
  int n, i, j, k, u, v, w, z, p, m;

  while (scanf("%d", &n), n) {
    init(n);
    for (i = 1; i < n; i++) {
      scanf("%d %d %d", &u, &v, &w);
      add(u, v, w);
      add(v, u, w);
    }

    memset(pnt, -1, sizeof(pnt));
    memset(h, -1, sizeof(h));
    dist[0] = h[0] = s = t = 0;
    q[t++] = 0;
    while (s < t) {
      u = q[s++];
      for (z = hd[u], p = -1; z > -1; z = ed[z].p) {
        v = ed[z].v;
        w = ed[z].w;
        if (h[v] == -1) {
          dist[v] = dist[u] + w;
          h[v] = h[u] + 1;
          pnt[v][0] = u;
          q[t++] = v;
          p = z;
        } else {
          (p == -1 ? hd[u] : ed[p].p) = ed[z].p;
        }
      }
    }

    dmax = 0;
    for (j = 1; (1 << j) < n; j++) {
      for (i = 0; i < n; i++) {
        k = pnt[i][j - 1];
        pnt[i][j] = (k > -1 ? pnt[k][j - 1] : -1);
        if (pnt[i][j] != -1) dmax = j;
      }
      if (dmax < j) break;
    }

    scanf("%d", &m);
    while (m--) {
      scanf("%d %d", &u, &v);
      k = lca(u, v);
      printf("%d\n", dist[u] + dist[v] - 2 * dist[k]);
    }
    puts("");
  }

  return 0;
}

