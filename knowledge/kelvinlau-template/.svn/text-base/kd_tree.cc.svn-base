/* MIPT Range Query 
 * surport 3 types of operations:
 *   add x, y
 *   delete x, y
 *   count [x1, x2] * [y1, y2] */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <set>
using namespace std;

struct point {
  int x, y;
};

bool operator<(point p, point q) {
  return p.x < q.x || p.x == q.x && p.y < q.y;
}

bool operator==(point p, point q) {
  return p.x == q.x && p.y == q.y;
}

bool xcmp(point *p, point *q) {
  return p->x < q->x;
}

bool ycmp(point *p, point *q) {
  return p->y < q->y;
}

struct node {
  point *p;
  node *l, *r;
  int s, x1, x2, y1, y2;
};

const int N = 100005;
const int INF = 2000000000;

point *xl[N], *yl[N], *tl[N];
node nodes[N * 4], *next;

node *build(point **xl, point **yl, int n, int d) {
  if (!n) return NULL;

  node *u = next++;
  u->x1 = xl[0]->x;
  u->x2 = xl[n - 1]->x;
  u->y1 = yl[0]->y;
  u->y2 = yl[n - 1]->y;

  if (n == 1) {
    u->l = u->r = NULL;
    u->p = xl[0];
    u->s = 0; // XXX size set to 0 here
    return u;
  }

  int mid = n / 2;
  if (!d) {
    if (rand() % 2)
      while (mid > 0 && xl[mid - 1]->x == xl[mid]->x) mid--;
    else if (mid > 0)
      while (mid < n && xl[mid - 1]->x == xl[mid]->x) mid++;
  } else {
    if (rand() % 2)
      while (mid > 0 && yl[mid - 1]->y == yl[mid]->y) mid--;
    else if (mid > 0)
      while (mid < n && yl[mid - 1]->y == yl[mid]->y) mid++;
  }

  int t1 = 0, t2 = mid;
  if (!d) {
    int lim = mid < n ? xl[mid]->x : INF;
    for (int i = 0; i < n; i++)
      if (yl[i]->x < lim)
        tl[t1++] = yl[i];
      else
        tl[t2++] = yl[i];
    for (int i = 0; i < n; i++)
      yl[i] = tl[i];
  } else {
    int lim = mid < n ? yl[mid]->y : INF;
    for (int i = 0; i < n; i++)
      if (xl[i]->y < lim)
        tl[t1++] = xl[i];
      else
        tl[t2++] = xl[i];
    for (int i = 0; i < n; i++)
      xl[i] = tl[i];
  }

  u->p = NULL;
  u->l = build(xl, yl, mid, !d);
  u->r = build(xl + mid, yl + mid, n - mid, !d);
  u->s = (u->l ? u->l->s : 0) + (u->r ? u->r->s : 0);
  return u;
}

/* build a kd tree from n points
 * NO duplicated points allowed */
node *kd_tree(point *p, int n) {
  for (int i = 0; i < n; i++)
    xl[i] = yl[i] = p + i;
  sort(xl, xl + n, xcmp);
  sort(yl, yl + n, ycmp);
  next = nodes;
  return build(xl, yl, n, 0);
}

inline int leaf(node *x) {
  return !!x->p;
}

inline int contained(node *u, int x1, int x2, int y1, int y2) {
  return x1 <= u->x1 && u->x2 <= x2 && y1 <= u->y1 && u->y2 <= y2;
}

inline int intersected(node *u, int x1, int x2, int y1, int y2) {
  return max(u->x1, x1) <= min(u->x2, x2) && max(u->y1, y1) <= min(u->y2, y2);
}

inline int inside(node *u, int x, int y) {
  return u->x1 <= x && x <= u->x2 && u->y1 <= y && y <= u->y2;
}

/* set or unset a point */
void change(node *u, int x, int y, int d) {
  while (1) {
    u->s += d;
    if (leaf(u)) 
      break;
    if (u->l && inside(u->l, x, y))
      u = u->l;
    else
      u = u->r;
  }
}

/* number of points inside [x1, x2] * [y1, y2] */
int query(node *u, int x1, int x2, int y1, int y2) {
  if (contained(u, x1, x2, y1, y2)) return u->s;

  int ret = 0;
  if (u->l && intersected(u->l, x1, x2, y1, y2))
    ret += query(u->l, x1, x2, y1, y2);
  if (u->r && intersected(u->r, x1, x2, y1, y2))
    ret += query(u->r, x1, x2, y1, y2);
  return ret;
}

struct query_t {
  char t;
  point p;
  int x1, y1, x2, y2;
};

int n, m;
query_t q[N];
point p[N];

int main() {
  char cmd[10];

  n = m = 0;
  while (scanf("%s", cmd) != EOF) {
    switch (q[m].t = cmd[0]) {
      case 'A':
        scanf("%d %d", &q[m].p.x, &q[m].p.y);
        p[n++] = q[m].p;
        break;
      case 'D':
        scanf("%d %d", &q[m].p.x, &q[m].p.y);
        break;
      case 'C':
        scanf("%d %d %d %d", &q[m].x1, &q[m].y1, &q[m].x2, &q[m].y2);
        break;
    }
    m++;
  }

  sort(p, p + n);
  n = unique(p, p + n) - p;
  node *root = kd_tree(p, n);

  set<point> btree;
  set<point>::iterator it;

  for (int i = 0; i < m; i++) {
    query_t *qt = &q[i];
    switch (qt->t) {
      case 'A':
        if (btree.find(qt->p) == btree.end()) {
          btree.insert(qt->p);
          change(root, qt->p.x, qt->p.y, +1);
          puts("ADDED");
        } else {
          puts("ALREADY EXISTS");
        }
        break;
      case 'D':
        if ((it = btree.find(qt->p)) != btree.end()) {
          btree.erase(it);
          change(root, qt->p.x, qt->p.y, -1);
          puts("DELETED");
        } else {
          puts("NOT FOUND");
        }
        break;
      case 'C':
        printf("%d\n", query(root, qt->x1, qt->x2, qt->y1, qt->y2));
        break;
    }
  }

  return 0;
}
