#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 5010, M = 20010;

int n, m, en, hd[N], root;
struct edge { int v, p, b; } ed[M];
int vt[N], low[N], dep[N], cut[N];

void init() {
  en = 0;
  memset(hd, -1, sizeof hd);
}

void add(int u, int v) {
  ed[en] = (edge) { v, hd[u], 0 }; // XXX g++ only!!
  hd[u] = en++;
}

void dfs(int u, int p, int d) {
  int e = 0, deg = 0, v, z;

  vt[u] = 1;
  low[u] = dep[u] = d;
  for (z = hd[u]; z > -1; z = ed[z].p) {
    if (!vt[v = ed[z].v]) {
      deg++;
      dfs(v, u, d + 1);
      low[u] = min(low[u], low[v]);
      if (low[v] > dep[u])
        ed[z].b = ed[z ^ 1].b = 1;
      cut[u] |= (u == root && deg > 1 || u != root && low[v] >= dep[u]);

    } else if (v != p || e) {
      low[u] = min(low[u], dep[v]);

    } else e = 1;
  }
}

void mark(int u, int c) {
  int v, z;

  vt[u] = c;
  for (z = hd[u]; z > -1; z = ed[z].p) {
    if (!ed[z].b && vt[v = ed[z].v] == -1)
      mark(v, c);
  }
}

int main() {
  int u, v, c;
  
  init();
  scanf("%d %d", &n, &m);
  while (m--) {
    scanf("%d %d", &u, &v);
    add(u, v);
    add(v, u);
  }
  
  memset(vt, 0, sizeof vt);
  memset(cut, 0, sizeof cut);
  dfs(root = 0, -1, 0);
  
  c = 0;
  memset(vt, -1, sizeof vt);
  for (u = 0; u < n; u++)
    if (vt[u] == -1)
      mark(u, c++);
  
  return 0;
}
