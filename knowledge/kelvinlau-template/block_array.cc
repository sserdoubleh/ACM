#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

struct Line {
  int y, x1, x2, t;
};

bool cmp(Line a, Line b) {
  return a.y < b.y;
}

typedef long long ll;
const int N = 10005, G = 145;

int n, m, k, cover;
int dx[N<<1], dd[N<<1];
int cov[N<<1], f[G], g[G][N], w;
Line line[N<<1];

void update(int j, int t) {
  int x = cov[j];
  g[j/w][x] -= dd[j];
  g[j/w][x+t] += dd[j];
  cov[j] += t;
}

int main() {
  int i, j, a, b, c, d, t, x1, x2, y1, y2;
  ll len, area;

  while (scanf("%d", &n), n) {
    m = k = 0;
    for (i = 0; i < n; i++) {
      scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
      line[m].y = y1;
      line[m].x1 = x1;
      line[m].x2 = x2;
      line[m++].t = +1;
      
      line[m].y = y2;
      line[m].x1 = x1;
      line[m].x2 = x2;
      line[m++].t = -1;

      dx[k++] = x1;
      dx[k++] = x2;
    }
    scanf("%d", &cover);
  
    sort(dx, dx + k);
    k = unique(dx, dx + k) - dx;
    for (w = 1; w * w < k; w++);

    for (i = 0; i < k - 1; i++)
      dd[i] = dx[i + 1] - dx[i];

    memset(cov, 0, sizeof(cov));
    memset(f, 0, sizeof(f));
    
    for (j = 0; j * w < k - 1; j++) {
      a = j * w;
      b = min(a + w, k - 1);
      g[j][0] = dx[b] - dx[a];
      
      for (i = 1; i <= cover; i++)
        g[j][i] = 0;
    }
    
    sort(line, line + m, cmp);
    
    area = 0;
    for (i = 0; i < m - 1; i++) {
      x1 = line[i].x1;
      x2 = line[i].x2;
      t = line[i].t;
      
      a = lower_bound(dx, dx + k, x1) - dx;
      d = lower_bound(dx, dx + k, x2) - dx;
      b = (a + w - 1) / w * w;
      c = (d == k - 1 ? d : d / w * w);

      if (b >= c) {
        for (j = a; j < d; j++)
          update(j, t);
      } else {
        for (j = a; j < b; j++)
          update(j, t);

        for (j = c; j < d; j++)
          update(j, t);

        for (j = b / w; j * w < c; j++)
          f[j] += t;
      }

      len = 0;
      for (j = 0; j * w < k - 1; j++)
        if (cover >= f[j]) len += g[j][cover - f[j]];
      area += len * (line[i+1].y - line[i].y);
    }
    printf("%lld\n", area);
  }
  
  return 0;
}
