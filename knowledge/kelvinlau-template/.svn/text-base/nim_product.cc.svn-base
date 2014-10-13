#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

int cache[20][20];

int mulx(int, int);

int mul(int x, int y) {
  int sum = 0;
  for (int i = 0; (1 << i) <= x; i++) if (x & (1 << i))
    for (int j = 0; (1 << j) <= y; j++) if (y & (1 << j))
      sum ^= mulx(i, j);
  return sum;
}

int mulx(int i, int j) {
  if (i > j)
    swap(i, j);

  if (i == 0)
    return 1 << j;

  if (cache[i][j] != -1)
    return cache[i][j];

  int x = 1 << i;
  int y = 1 << j;

  int k;
  for (k = 30; !((1 << k) & i); k--);
  int z = 1 << (1 << k);

  if (x == z) {
    if ((i & j) == 0)
      return x * y;
    else
      return mul(x * 3 / 2, y / x);
  }
  return mul(mul(y, z), x / z);
}

void init() {
  memset(cache, -1, sizeof cache);
}
