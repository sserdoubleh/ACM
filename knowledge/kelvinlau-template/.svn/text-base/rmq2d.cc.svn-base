#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 200 + 10;
const int K = 9;

int a[N][N];
int rmq[N][N][K][K], wide[N];

int find(int x1, int y1, int x2, int y2) {
  int x, y, res;

  x = wide[x2 - x1 + 1];
  y = wide[y2 - y1 + 1];
  x2 = x2 + 1 - (1 << x);
  y2 = y2 + 1 - (1 << y);

  res = -1;
  res = max(res, rmq[x1][y1][x][y]);
  res = max(res, rmq[x2][y1][x][y]);
  res = max(res, rmq[x1][y2][x][y]);
  res = max(res, rmq[x2][y2][x][y]);
  return res;
}

int main() {
  int n, m, i, j, x, y;

  wide[1] = 0;
  for (i = 2; i < N; i++)
    wide[i] = wide[i / 2] + 1;

  for (i = 1; i <= n; i++)
    for (j = 1; j <= m; j++)
      scanf("%d", &a[i][j]);

  for (x = 0; (1 << x) <= n; x++)
    for (y = 0; (1 << y) <= m; y++)
      for (i = 1; i + (1 << x) <= n + 1; i++)
        for (j = 1; j + (1 << y) <= m + 1; j++)
          if (x == 0 && y == 0)
            rmq[i][j][x][y] = a[i][j];
          else if (y == 0)
            rmq[i][j][x][y] = max(rmq[i][j][x - 1][y], rmq[i + (1 << (x - 1))][j][x - 1][y]);
          else
            rmq[i][j][x][y] = max(rmq[i][j][x][y - 1], rmq[i][j + (1 << (y - 1))][x][y - 1]);

  return 0;
}
