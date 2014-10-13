#include <stdio.h>
#include <string.h>
using namespace std;

const int N = 100 + 10;

bool adj[N][N], vt[N];
int lnk[N];

int find(int m, int x) {
  for (int y = 0; y < m; y++) if (adj[x][y] && !vt[y]) {
    vt[y] = 1;
    if (lnk[y] == -1 || find(m, lnk[y])) {
      lnk[y] = x;
      return 1;
    }
  }
  return 0;
}

int max_match(int n, int m) {
  memset(lnk, -1, sizeof(lnk));
  int res = 0;
  for (int x = 0; x < n; x++) {
    memset(vt, 0, sizeof(vt));
    if (find(m, x)) res++;
  }
  return res;
}
