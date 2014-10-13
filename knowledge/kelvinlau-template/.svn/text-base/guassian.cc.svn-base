#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <math.h>
using namespace std;

const int N = 100 + 10;

void print(double a[N][N], int n) {
  for (int i = 0; i < n; i++, puts(""))
    for (int j = 0; j <= n; j++)
      printf("%7.3lf", a[i][j]);
  puts("");
}

int guassian(double a[N][N], int n) {  
  int x, y, i, j, bx;
  for (x = y = 0; x < n && y <= n; y++, x++) {
    for (; y <= n; y++) {
      for (bx = -1, i = x; i < n; i++)
        if (a[i][y] != 0 && (bx == -1 || fabs(a[i][y]) > fabs(a[bx][y])))
          bx = i;
      if (bx != -1) break;
    }
    if (y > n) break;
    if (y == n) return 0;
    if (x != bx)
      for (j = y; j <= n; j++)
        swap(a[x][j], a[bx][j]);
    for (j = n; j >= y; j--)
      a[x][j] /= a[x][y];
    for (i = 0; i < n; i++) if (i != x && a[i][y] != 0)
      for (j = n; j >= y; j--)
        a[i][j] -= a[x][j] * a[i][y];
  }
  return 1;
}

int n;
double a[N][N];

int main() {
  scanf("%d", &n);
  for (int i = 0; i < n; i++)
    for (int j = 0; j <= n; j++)
      scanf("%lf", &a[i][j]);
  guassian(a, n);
  return 0;
}
