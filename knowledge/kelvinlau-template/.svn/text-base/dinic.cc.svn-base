#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 1010, M = 100010, inf = 0x3f3f3f3f;
struct edge { int x, y, c, p; } ed[M];
int ne, hd[N], cur[N], ps[N], dep[N];

void add(int x, int y, int c) {
  ed[ne] = (edge) { x, y, c, hd[x] }; hd[x] = ne++; // XXX g++ only!!
  ed[ne] = (edge) { y, x, 0, hd[y] }; hd[y] = ne++; // XXX g++ only!!
}

void init() {
  ne = 0;
  memset(hd, -1, sizeof(hd));
}

int flow(int n, int s, int t) {
  int i, j, k, f, r, top, tr, res = 0;
  while (1) {
    memset(dep, -1, n * sizeof(int));
    for (f = dep[ps[0] = s] = 0, r = 1; f != r; )
    for (i = ps[f++], j = hd[i]; j != -1; j = ed[j].p)
      if (ed[j].c && dep[k = ed[j].y] == -1) {
        dep[k] = dep[i] + 1; ps[r++] = k;
        if (k == t) { f = r; break; }
      }
    if (dep[t] == -1) break;
    
    memcpy(cur, hd, n * sizeof(int));
    for (i = s, top = 0; ; ) {
      if (i == t) {
        for (k = 0, tr = inf; k < top; k++)
          if (ed[ps[k]].c < tr)
            tr = ed[ps[f = k]].c;
        for (k = 0; k < top; k++)
          ed[ps[k]].c -= tr, ed[ps[k]^1].c += tr;
        res += tr; i = ed[ps[top = f]].x;
      }
      for (j = cur[i]; cur[i] != -1; j = cur[i] = ed[cur[i]].p)
        if (ed[j].c && dep[i] + 1 == dep[ed[j].y]) break;
      if (cur[i] != -1) {
        ps[top++] = cur[i];
        i = ed[cur[i]].y;
      } else {
        if (top == 0) break;
        dep[i] = -1; i = ed[ps[--top]].x;
      }
    }
  }
  return res;
}
