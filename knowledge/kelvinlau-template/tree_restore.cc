/* Restore the tree from a distance matrix of every pair of leaves 
 * O(L ^ 3) */
#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 200 + 10;

int l, n;
int d[N + N][N + N];
int pnt[N + N], wt[N + N];

void init() {
  memset(pnt, -1, sizeof(pnt));
}

void add(int u, int v, int w) {
  pnt[v] = u;
  wt[v] = w;
}

int check_edge(int x, int v, int &e1, int &e2) {
  int u = pnt[v];
  int w = wt[v];
  int el = 0, er = 0;
  for (int j = 0; j < x; j++) {
    if (d[j][u] < d[j][v]) {
      if (el == 0) {
        el = d[x][j] - d[u][j];
        if (el <= 0) return 0;
      } else {
        if (el != d[x][j] - d[u][j]) return 0;
      }
    } else {
      if (er == 0) {
        er = d[x][j] - d[v][j];
        if (er <= 0) return 0;
      } else {
        if (er != d[x][j] - d[v][j]) return 0;
      }
    }
  }
  if (el + er < w) return 0;
  if ((el + er - w) % 2) return 0;
  e2 = (el + er - w) / 2;
  if (el - e2 >= w || er - e2 >= w) return 0;
  e1 = er - e2;
  return 1;
}

int check_node(int x, int u) {
  int delta = d[x][0] - d[u][0];
  if (delta <= 0) return 0;
  for (int j = 1; j < x; j++) {
    if (d[x][j] - d[u][j] != delta)
      return 0;
  }
  return delta;
}

void find1(int v, int x) {
  int u = pnt[v];
  int w = wt[v];
  for (int i = 0; i < n; i = (i + 1 != x ? i + 1 : l))
    d[v][i] = d[i][v] = d[u][i] + w;
  d[v][v] = 0;
}

void find2(int v, int c, int x) {
  int u = pnt[v];
  int w = wt[v];
  int w1 = w;
  int w2 = wt[c];
  for (int i = 0; i < n; i = (i + 1 != x ? i + 1 : l))
    d[v][i] = d[i][v] = (d[i][u] < d[i][c] ? d[i][u] + w1 : d[i][c] + w2);
  d[v][v] = 0;
}

void extend_node(int x, int u, int e) {
  add(u, x, e);
  find1(x, x);
}

void extend_edge(int x, int v, int e1, int e2) {
  int u = pnt[v];
  int w = wt[v];
  if (e2 == 0) {
    add(x, v, e1);
    add(u, x, w - e1);
    find2(x, v, x);
  } else {
    add(n, v, e1);
    add(u, n, w - e1);
    add(n, x, e2);
    find2(n, v, x);
    n++;
    find1(x, x);
  }
}

void output(int v) {
  int u = pnt[v];
  int w = wt[v];
  while (w > 1) {
    printf("%d %d\n", n + 1, v + 1);
    v = n++;
    w--;
  }
  printf("%d %d\n", u + 1, v + 1);
}

int main() {
  while (scanf("%d", &l) != EOF) {
    for (int i = 0; i < l; i++)
      for (int j = 0; j < l; j++)
        scanf("%d", &d[i][j]);

    init();
    add(0, 1, d[0][1]);

    n = l;
    for (int x = 2; x < l; x++) {
      int flag = 0;
      for (int i = l; i < n; i++) {
        int e = check_node(x, i);
        if (e > 0) {
          extend_node(x, i, e);
          flag = 1;
          break;
        }
      }
      if (flag) continue;

      for (int i = 1; i < n; i++) {
        if (i == x) i = l;
        if (i == n) break;
        int e1, e2;
        if (check_edge(x, i, e1, e2)) {
          extend_edge(x, i, e1, e2);
          flag = 1;
          break;
        }
      }

      if (!flag) {
        n = -1;
        break;
      }
    }

    if (n >= 0) {
      int num = 1;
      for (int i = 1; i < n; i++)
        num += wt[i];
      printf("%d\n", num);
      int n1 = n;
      for (int i = 1; i < n1; i++)
        output(i);
    } else puts("-1");
    puts("");
  }
  return 0;
}
