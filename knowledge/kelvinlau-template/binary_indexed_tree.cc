#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 20010;
const int B = 17;

int c[N], n;

void add(int x, int d) {
  for (x++; x <= n; x += x & -x)
    c[x] += d;
}

int sum(int x) {
  int ret = 0;
  for (x++; x; x -= x & -x)
    ret += c[x];
  return ret;
}

int kth(int k) {
  int ret = 0;
  for (int i = B; i >= 0; i--) {
    ret += 1 << i;
    if (ret > n || c[ret] > k) ret -= 1 << i;
    else k -= c[ret];
  }
  return ret;
}

void init() {
  memset(c, 0, sizeof(c));
}
