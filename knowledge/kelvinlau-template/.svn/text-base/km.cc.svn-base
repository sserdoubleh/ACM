#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int N = 105, inf = 0x3f3f3f3f;

int w[N][N], match[N], slack[N], lx[N], ly[N];
bool vx[N], vy[N];

bool find(int n, int u) {
  vx[u] = 1;
  for (int v = 0; v < n; v++) if (!vy[v]) {
    if (lx[u] + ly[v] == w[u][v]) {
      vy[v] = 1;
      if (match[v] == -1 || find(n, match[v])) {
        match[v] = u;
        return 1;
      }
    } else slack[v] = min(slack[v], lx[u] + ly[v] - w[u][v]);
  }
  return 0;
}

int max_match(int n) {
  for (int i = 0; i < n; i++) {
    lx[i] = *max_element(w[i], w[i] + n);
    ly[i] = 0;
  }
  memset(match, -1, sizeof(match));
  
  for (int u = 0; u < n; u++) {
    memset(slack, 0x3f, sizeof(slack));
    while (1) {
      memset(vx, 0, sizeof(vx));
      memset(vy, 0, sizeof(vy));
      if (find(n, u)) break;
      
      int d = inf;
      for (int j = 0; j < n; j++)
        if (!vy[j]) d = min(d, slack[j]);
      
      for (int i = 0; i < n; i++)
        if (vx[i]) lx[i] -= d;
      for (int j = 0; j < n; j++)
        if (vy[j]) ly[j] += d; else slack[j] -= d;
    }
  }
  
  int res = 0;
  for (int j = 0; j < n; j++)
    res += w[match[j]][j];
  return res;
}

int min_match(int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      w[i][j] *= -1;
  
  return -max_match(n);
}

int main() {
  int n;
  scanf("%d", &n);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      scanf("%d", &w[i][j]);
  
  printf("%d\n", max_match(n));

  return 0;
}

