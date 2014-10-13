/* MIPT Range Query 
 * surport 3 types of operations:
 *   add x, y
 *   delete x, y
 *   count [x1, x2] * [y1, y2] */
#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

/*********** point ***********/

struct point {
  int x, y;
};

bool operator<(point p, point q) {
  return p.x < q.x || p.x == q.x && p.y < q.y;
}

bool operator==(point p, point q) {
  return p.x == q.x && p.y == q.y;
}

bool xcmp(point *a, point *b) {
  return a->x < b->x || a->x == b->x && a->y < b->y; 
}

bool ycmp(point *a, point *b) {
  return a->y < b->y || a->y == b->y && a->x < b->x; 
}

/*********** binary indexed tree ***********/

void ta_init(int *ta, int n) {
  memset(ta, 0, sizeof(*ta) * n);
}

void ta_add(int *ta, int n, int k, int d) {
  while (k <= n) {
    ta[k] += d;
    k += k & -k;
  }
}

int ta_sum(int *ta, int n, int k) {
  int res = 0;
  while (k) {
    res += ta[k];
    k -= k & -k;
  }
  return res;
}

/*********** range tree ***********/

struct node {
  int size;
  int x1, x2;
  node *l, *r;
  int *la, *lb;
  int *ta;
};

struct range_tree {
  node *root;
  int size;
  point **yl;
};

const int N = 100010;
const int M = N * 20; // N log N
const int INF = 2010000000;

node nodes[N << 1], *next;
point *xs[N], *ys[N], *yt[N];
int links[M << 1], *ln;
int ts[M], *tn;

node *__build(point **xl, point **yl, point **yt, int n) {
  int i, d, na, nb;
  node *p;
  point **ya, **yb;

  p = next++;
  p->x1 = xl[0]->x;
  p->x2 = xl[n - 1]->x;
  p->size = n;
  p->ta = tn; tn += n + 1;
  ta_init(p->ta, n + 1);

  if (n > 1) {
    d = n / 2;
    ya = yt;
    yb = yt + d;
    na = d;
    nb = n - d;
    p->la = ln; ln += n + 1;
    p->lb = ln; ln += n + 1;
    p->la[n] = na;
    p->lb[n] = nb;
    for (i = n - 1; i >= 0; i--) {
      if (xcmp(yl[i], xl[d]))
        ya[--na] = yl[i];
      else
        yb[--nb] = yl[i];
      p->la[i] = na;
      p->lb[i] = nb;
    }
    p->l = __build(xl, yt, yl, d);
    p->r = __build(xl + d, yt + d, yl + d, n - d);
  } else {
    p->l = p->r = NULL;
  }

  return p;
}

/* NOTE: no duplicated points are allowed 
 *       only one range_tree can be maintained at a time */
range_tree *range_tree_build(point *p, int n) {
  static range_tree __rt;
  range_tree *rt = &__rt;

  for (int i = 0; i < n; i++)
    xs[i] = ys[i] = &p[i];
  sort(xs, xs + n, xcmp);
  sort(ys, ys + n, ycmp);
  ln = links;
  tn = ts;
  next = nodes;
  rt->root = n ? __build(xs, ys, yt, n) : NULL;

  for (int i = 0; i < n; i++)
    ys[i] = &p[i];
  sort(ys, ys + n, ycmp);
  rt->yl = ys;
  rt->size = n;

  return rt;
}

int __query(node *p, int x1, int x2, int lb, int ub) {
  if (!p || x2 < p->x1 || p->x2 < x1 || lb >= ub)
    return 0;

  if (x1 <= p->x1 && p->x2 <= x2)
    return ta_sum(p->ta, p->size, ub) - ta_sum(p->ta, p->size, lb);

  return __query(p->l, x1, x2, p->la[lb], p->la[ub]) + 
         __query(p->r, x1, x2, p->lb[lb], p->lb[ub]);
}

int range_tree_query(range_tree *rt, int x1, int x2, int y1, int y2) {
  int lb, ub;
  point a, b;
  node *root = rt->root;

  if (!root) return 0;

  a.x = -INF; a.y = y1; 
  b.x = +INF; b.y = y2;
  lb = lower_bound(rt->yl, rt->yl + rt->size, &a, ycmp) - rt->yl;
  ub = upper_bound(rt->yl, rt->yl + rt->size, &b, ycmp) - rt->yl;
  return __query(root, x1, x2, lb, ub);
}

int range_tree_add(range_tree *rt, int x, int y, int d) {
  int i;
  point a;
  node *p = rt->root;

  if (!p) return 0;

  a.x = x; a.y = y;
  i = lower_bound(rt->yl, rt->yl + rt->size, &a, ycmp) - rt->yl;
  if (i == rt->size || rt->yl[i]->x != x || rt->yl[i]->y != y)
    return 0;
  if ((ta_sum(p->ta, p->size, i) < ta_sum(p->ta, p->size, i + 1)) ^ (d < 0))
    return 0;

  while (p) {
    ta_add(p->ta, p->size, i + 1, d);
    if (p->size <= 1) break;
    if (p->la[i] != p->la[i + 1])
      i = p->la[i], p = p->l;
    else
      i = p->lb[i], p = p->r;
  }
  return 1;
}

/*********** main ***********/

struct query_t {
  char t;
  point p;
  int x1, y1, x2, y2;
};

int main() {
  char cmd[10];
  int n, m;
  static query_t qs[N];
  static point p[N];

  n = m = 0;
  while (scanf("%s", cmd) != EOF) {
    switch (qs[m].t = cmd[0]) {
      case 'A':
        scanf("%d %d", &qs[m].p.x, &qs[m].p.y);
        p[n++] = qs[m].p;
        break;
      case 'D':
        scanf("%d %d", &qs[m].p.x, &qs[m].p.y);
        break;
      case 'C':
        scanf("%d %d %d %d", &qs[m].x1, &qs[m].y1, &qs[m].x2, &qs[m].y2);
        break;
    }
    m++;
  }

  sort(p, p + n);
  n = unique(p, p + n) - p;
  range_tree *rt = range_tree_build(p, n);

  for (int i = 0; i < m; i++) {
    query_t *q = &qs[i];
    switch (q->t) {
      case 'A':
        if (range_tree_add(rt, q->p.x, q->p.y, +1)) {
          puts("ADDED");
        } else {
          puts("ALREADY EXISTS");
        }
        break;
      case 'D':
        if (range_tree_add(rt, q->p.x, q->p.y, -1)) {
          puts("DELETED");
        } else {
          puts("NOT FOUND");
        }
        break;
      case 'C':
        printf("%d\n", range_tree_query(rt, q->x1, q->x2, q->y1, q->y2));
        break;
    }
  }

  return 0;
}
