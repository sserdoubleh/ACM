#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int inf = 10000000;
const int N = 105;
int low[N][N], net[N][N];

int max_flow(int s, int t, int n) {
  static int q[N], pv[N];
  int a, b, i, d, u, v, res;
  
  res = 0;
  while (1) {
    a = b = 0;
    q[b++] = s;
    memset(pv, -1, sizeof(pv));
    pv[s] = s;
    while (a < b && pv[t] == -1) {
      u = q[a++];
      for (v = 0; v < n; v++)
        if (net[u][v] && pv[v] == -1) {
          pv[v] = u;
          q[b++] = v;
        }
    }
    if (pv[t] == -1) break;

    d = inf;
    for (v = t; v != s; v = u) {
      u = pv[v];
      d = min(d, net[u][v]);
    }

    res += d;
    for (v = t; v != s; v = u) {
      u = pv[v];
      net[u][v] -= d;
      net[v][u] += d;
    }
  }

  return res;
}

int limited_min_flow(int s, int t, int n) {
  int x, y, i, j, a, b, res;
  
  x = n;
  y = n + 1;
  for (i = 0; i < n; i++)
  for (j = 0; j < n; j++) {
    net[x][j] += low[i][j];
    net[i][y] += low[i][j];
    net[i][j] -= low[i][j];
  }
  
  res = 0;
  for (i = 0; i < n; i++)
    res += net[s][i] + low[s][i];  // be aware of overflow
  
  a = net[s][t];
  b = net[t][s]; net[t][s] = inf;
  max_flow(x, y, n + 2);

  for (i = 0; i < n; i++)
    if (net[x][i]) return -1;

  net[s][t] = a;
  net[t][s] = b;
  
  max_flow(t, s, n);
  
  for (i = 0; i < n; i++)
    res -= net[s][i];
  
  return res;
}
