#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

void manacher(char a[], int n, int rad[]) {
  int i, j, k;
  rad[0] = rad[n] = 0;
  for (i = 1, j = 0; i < n; i += k, j = max(0, j - k)) {
    while (i > j && i + j < n && a[i - j - 1] == a[i + j]) j++;
    for (rad[i] = j, k = 1; k <= rad[i] && rad[i - k] != rad[i] - k; k++)
      rad[i + k] = min(rad[i - k], rad[i] - k);
  }
}

const int N = 500010;
int n;
char a[N];
int rad[N];

int main() {
  scanf("%d %s", &n, a);
  manacher(a, n, rad);
  return 0;
}
