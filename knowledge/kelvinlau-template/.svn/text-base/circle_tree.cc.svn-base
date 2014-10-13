/* Find the 'containing-forest' of N non-touching circles
 * O(N log N) */
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <set>
using namespace std;

struct circle {
  int x, y, r;
};

struct node {
  int t;
  circle *c;
};

struct query {
  int x, i, t;
};

const int N = 50010;
int n, m;
circle c[N];
query q[N << 1];
int parent[N];

inline long long sqr(long long x) {
  return x * x;
}

bool contains(circle *a, circle *b) {
  return sqr(a->x - b->x) + sqr(a->y - b->y) < sqr(a->r - b->r);
}

bool operator<(const query &a, const query &b) {
  return a.x < b.x || a.x == b.x && a.t < b.t;
}

bool operator<(node a, node b) {
  if (a.c == b.c)
    return a.t < b.t;

  int z = 0;
  if (a.c->r < b.c->r) {
    z = 1;
    swap(a, b);
  }

  if (!contains(a.c, b.c))
    return z ^ (a.c->y < b.c->y);

  return z ^ (a.t < 0);
}

int main() {
  while (scanf("%d", &n) != EOF) {
    for (int i = 0; i < n; i++)
      scanf("%d %d %d", &c[i].x, &c[i].y, &c[i].r);

    m = 0;
    for (int i = 0; i < n; i++) {
      q[m].x = c[i].x - c[i].r;
      q[m].i = i;
      q[m++].t = -1;
      q[m].x = c[i].x + c[i].r;
      q[m].i = i;
      q[m++].t = +1;
    }
    sort(q, q + m);

    memset(parent, -1, sizeof parent);

    node t;
    set<node> s;
    set<node>::iterator it;
    for (int j = 0, i; j < m; j++) {
      t.c = &c[i = q[j].i];
      if (q[j].t == -1) {
        t.t = -1;
        it = s.insert(t).first;
        t.t = +1;
        s.insert(t);
        if (it != s.begin()) {
          --it;
          if (it->t == -1) {
            parent[i] = it->c - c;
          } else {
            parent[i] = parent[it->c - c];
          }
        }
      } else {
        t.t = +1;
        s.erase(s.find(t));
        t.t = -1;
        s.erase(s.find(t));
      }
    }
  }
  return 0;
}
