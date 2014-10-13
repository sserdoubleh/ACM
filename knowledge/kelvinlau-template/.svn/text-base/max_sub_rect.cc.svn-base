#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int N = 3005;
int n, m;
bool a[N][N];
int lc[N], rc[N], tc[N], ll[N], rr[N];

int main() {
  int i, j, l, u, d, r, ans, tt;

  ans = 0;
  memset(tc, 0, sizeof(tc));
  memset(ll, 0x3f, sizeof(ll));
  memset(rr, 0x3f, sizeof(rr));
  for (i = 1; i <= n; i++) {
    lc[0] = rc[m+1] = 0;
    for (j = m; j >= 1; j--)
      rc[j] = (a[i][j] ? rc[j+1]+1 : 0);
    
    for (j = 1; j <= m; j++) {
      if (!a[i][j]) {
        tc[j] = lc[j] = 0;
        ll[j] = rr[j] = 0x3f3f3f3f;
      } else {
        tc[j]++;
        lc[j] = lc[j-1] + 1;
        ll[j] = min(ll[j], lc[j]);
        rr[j] = min(rr[j], rc[j]);
        
        l = j - ll[j] + 1;
        r = j + rr[j] - 1;
        u = i - tc[j] + 1;
        d = i;
        tt = (r - l + 1) * (d - u + 1);
        ans = max(ans, tt);
      }
    }
  }

  printf("%d\n", ans);

  return 0;
}
