#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int N = 30010, inf = 1000000000;

int n, val[N], sum[N], pnt[N];
int m, vout[N<<1], next[N<<1], first[N];
int seq[N], pos[N];
int path[N][2];

void add_edge(int u, int v) {
  next[m] = first[u];
  vout[m] = v;
  first[u] = m++;
}

void build(int u) {
  int i, v;

  sum[u] = 1;
  for (i = first[u]; i > -1; i = next[i]) {
    v = vout[i];
    if (sum[v] == 0) {
      build(v);
      pnt[v] = u;
      sum[u] += sum[v];
    }
  }
}

void dfs(int u) {
  int i, v, hv = -1;

  seq[m] = u;
  pos[u] = m++;

  for (i = first[u]; i > -1; i = next[i]) {
    v = vout[i];
    if (pnt[v] == u)
      if (hv == -1 || sum[v] > sum[hv])
        hv = v;
  }

  if (hv == -1) {
    path[u][1] = u;
    return;
  }

  path[hv][0] = path[u][0];
  dfs(hv);
  path[u][1] = path[hv][1];
  
  for (i = first[u]; i > -1; i = next[i]) {
    v = vout[i];
    if (v != hv && pnt[v] == u) {
      path[v][0] = v;
      dfs(v);
    }
  }  
}

#define id  ((u + v) | (u < v))
#define mid ((u + v) >> 1)
#define lc  ((u + mid) | (u < mid))
#define rc  ((mid + 1 + v) | (mid + 1 < v))

int tf[N<<1], tg[N<<1];

void tbuild(int u, int v) {
  if (u == v) {
    tf[id] = val[seq[v]];
    tg[id] = val[seq[v]];
    return;
  }

  tbuild(u, mid);
  tbuild(mid + 1, v);

  tf[id] = tf[lc] + tf[rc];
  tg[id] = max(tg[lc], tg[rc]);
}

int ta, tb, tx;

int tmax(int u, int v) {
  if (ta <= u && v <= tb) return tg[id];
  if (tb < u || v < ta) return -inf;
  return max(tmax(u, mid), tmax(mid + 1, v));
}

int tsum(int u, int v) {
  if (ta <= u && v <= tb) return tf[id];
  if (tb < u || v < ta) return 0;
  return tsum(u, mid) + tsum(mid + 1, v);
}

void tupdate(int u, int v) {
  if (u == v) {
    tf[id] = val[seq[v]];
    tg[id] = val[seq[v]];
    return;
  }

  if (tx <= mid)
    tupdate(u, mid);
  else
    tupdate(mid + 1, v);

  tf[id] = tf[lc] + tf[rc];
  tg[id] = max(tg[lc], tg[rc]);
}

int findmax(int u, int v, int& lca) {
  int res, x;

  res = -inf;
  while (pos[u] < pos[v]) {
    x = path[v][0];
    if (x == v) {
      res = max(res, val[v]);
      v = pnt[v];
    } else {
      if (pos[x] <= pos[u])
        x = seq[pos[u] + 1];
      ta = pos[x];
      tb = pos[v];
      res = max(res, tmax(pos[path[v][0]], pos[path[v][1]]));
      v = pnt[x];
    }
  }
  
  lca = v;
  res = max(res, val[lca]);
  return res;
}

int findsum(int u, int v, int& lca) {
  int res, x;

  res = 0;
  while (pos[u] < pos[v]) {
    x = path[v][0];
    if (x == v) {
      res += val[v];
      v = pnt[v];
    } else {
      if (pos[x] <= pos[u])
        x = seq[pos[u] + 1];
      ta = pos[x];
      tb = pos[v];
      res += tsum(pos[path[v][0]], pos[path[v][1]]);
      v = pnt[x];
    }
  }

  lca = v;
  res += val[lca];
  return res;
}

int main() {
  int i, u, v, q, lca, ans;
  char cmd[10];
  
  while (scanf("%d", &n) != EOF) {
    m = 0;
    fill(first, first + n, -1);
    for (i = 0; i < n - 1; i++) {
      scanf("%d %d", &u, &v);
      u--; v--;
      add_edge(u, v);
      add_edge(v, u);
    }
    for (i = 0; i < n; i++)
      scanf("%d", &val[i]);

    pnt[0] = -1;
    fill(sum, sum + n, 0);
    build(0);

    m = 0;
    path[0][0] = 0;
    dfs(0);
    
    for (i = 0; i < n; i++) {
      u = path[i][0];
      v = path[i][1];
      if (u == i && u != v)
        tbuild(pos[u], pos[v]);
    }
    
    scanf("%d", &q);
    while (q--) {
      scanf("%s %d %d", cmd, &u, &v);
      if (cmd[1] == 'M') {
        u--; v--;
        if (pos[u] > pos[v]) swap(u, v);
        ans = findmax(u, v, lca);
        ans = max(ans, findmax(lca, u, lca));
        printf("%d\n", ans);
      } else if (cmd[1] == 'S') {
        u--; v--;
        if (pos[u] > pos[v]) swap(u, v);
        ans = findsum(u, v, lca);
        ans += findsum(lca, u, lca);
        ans -= val[lca];
        printf("%d\n", ans);
      } else if (cmd[1] == 'H') {
        u--;
        val[u] = v;
        tx = pos[u];
        tupdate(pos[path[u][0]], pos[path[u][1]]);
      }
    }
  }

  return 0;
}
