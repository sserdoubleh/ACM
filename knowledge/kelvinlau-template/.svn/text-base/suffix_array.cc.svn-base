#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

char *_str;

bool cmp(int x, int y) { 
  return _str[x] < _str[y]; 
}

inline int sv(int n, int *rk, int k) { return k < n ? rk[k] + 1 : 0; }

void suffix_array(char *str, int *sa, int *ht, int *rk, int *h) {
  int i, j, t, k, n = strlen(str);
  
  for (i = 0; i < n; i++) sa[i] = i;
  _str = str; sort(sa, sa + n, cmp);
  
  for (i = j = 0; i < n; i = j)
    while (j < n && str[sa[i]] == str[sa[j]])
      rk[sa[j++]] = i;

  for (k = 1; k <= n; k <<= 1) {
    for (i = 0; i <= n; i++) h[i] = 0;
    for (i = 0; i < n; i++) h[sv(n, rk, i + k)]++;
    for (i = 1; i <= n; i++) h[i] += h[i - 1];
    for (i = n - 1; i >= 0; i--) ht[--h[sv(n, rk, i + k)]] = i;

    for (i = 0; i < n; i++) h[i] = 0;
    for (i = 0; i < n; i++) h[rk[sa[i]]]++;
    for (i = 1; i < n; i++) h[i] += h[i - 1];
    for (i = n - 1; i >= 0; i--) sa[--h[rk[ht[i]]]] = ht[i];

    for (i = j = 0; i < n; i = j)
      while (j < n && rk[sa[i]] == rk[sa[j]] && sv(n, rk, sa[i] + k) == sv(n, rk, sa[j] + k))
        ht[sa[j++]] = i;
    swap(ht, rk);
  }

  for (i = 0; i < n; i++) {
    if (rk[i] == n - 1) {
      h[i] = 0;
      continue;
    }
    k = sa[rk[i] + 1];
    t = (!i || h[i - 1] <= 1) ? 0 : h[i - 1] - 1;
    for (; str[i + t] == str[k + t]; t++);
    h[i] = t;
  }
  for (i = 0; i < n; i++)
    ht[i] = h[sa[i]];
}

const int N = 100010;

char str[N];
int sa[N], ht[N], rk[N], h[N];

int main() {
  scanf("%s", str);
  suffix_array(str, sa, ht, rk, h);
  
  for (int i = 0; str[i]; i++)
    printf("%d %s\n", ht[i], &str[sa[i]]);
  
  return 0;
}
