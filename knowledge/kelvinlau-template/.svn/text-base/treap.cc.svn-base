#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <ctype.h>
using namespace std;

typedef int T;

const int N = 1000010;

struct node {
  T d; 
  int t, s;
  node *p, *l, *r;
} nodes[N], *next_alloc;

void init() {
  next_alloc = nodes;
}

node *node_new(T d) {
  node *x = next_alloc++;
  x->d = d;
  x->l = x->r = NULL;
  x->t = rand();
  x->s = 1;
  return x;
}

inline int size(node *x) {
  return x ? x->s : 0;
}

node *root(node *x) {
  while (x->p) x = x->p;
  return x;
}

void add_size(node *x, int d) {
  for ( ; x; x = x->p)
    x->s += d;
}

void rotate(node *x) {
  node *y = x->p;
  node *g = y->p;

  x->p = g;
  if (g) (g->l == y ? g->l : g->r) = x;
  y->p = x;

  if (x == y->l) {
    y->l = x->r;
    if (x->r) x->r->p = y;
    x->r = y;
  } else {
    y->r = x->l;
    if (x->l) x->l->p = y;
    x->l = y;
  }

  y->s = size(y->l) + size(y->r) + 1;
  x->s = size(x->l) + size(x->r) + 1;
}

void adjust(node *x) {
  while (x->p && x->t < x->p->t)
    rotate(x);
}

/* return new root
 * duplicated elements are not allowed */
node *_insert(node *p, node *x) {
  if (!p)
    return x;

  while (1) {
    if (x->d < p->d) {
      if (!p->l) {
        add_size(x->p = p, 1);
        adjust(p->l = x);
        break;
      }
      p = p->l;
    } else if (x->d > p->d) {
      if (!p->r) {
        add_size(x->p = p, 1);
        adjust(p->r = x);
        break;
      }
      p = p->r;
    } else break;
  }
  return root(p);
}

/* return new root */
node *insert(node *p, T d) {
  return _insert(p, node_new(d));
}

node *kth(node *p, int k) {
  while (p) {
    if (k == size(p->l)) 
      return p;
    if (k < size(p->l)) 
      p = p->l;
    else 
      k -= size(p->l) + 1, p = p->r;
  }
  return NULL;
}

/* # nodes with value < d */
int count(node *x, T d) {
  int res = 0;
  while (x) {
    if (d <= x->d)
      x = x->l;
    else
      res += size(x->l) + 1, x = x->r;
  }
  return res;
}

node *find(node *p, T d) {
  while (p) {
    if (d == p->d) return p;
    p = (d < p->d ? p->l : p->r);
  }
  return NULL;
}

node *lower_bound(node *x, T d) {
  node *y;
  if (!x)
    return NULL;
  if (d <= x->d && (y = lower_bound(x->l, d))) 
    return y;
  if (d <= x->d)
    return x;
  return lower_bound(x->r, d);
}

node *upper_bound(node *x, T d) {
  node *y;
  if (!x)
    return NULL;
  if (d < x->d && (y = upper_bound(x->l, d))) 
    return y;
  if (d < x->d)
    return x;
  return upper_bound(x->r, d);
}

node *next(node *x) {
  if (x->r) {
    x = x->r;
    while (x->l) x = x->l;
    return x;
  } else {
    while (x->p && x->p->r == x)
      x = x->p;
    return x->p;
  }
}

node *prev(node *x) {
  if (x->l) {
    x = x->l;
    while (x->r) x = x->r;
    return x;
  } else {
    while (x->p && x->p->l == x)
      x = x->p;
    return x->p;
  }
}

/* return new root */
node *erase(node *x) {
  while (x->l || x->r) {
    if (x->l && (!x->r || x->l->t < x->r->t))
      rotate(x->l);
    else
      rotate(x->r);
  }

  node *y = x->p;
  if (y) (y->l == x ? y->l : y->r) = NULL, add_size(y, -1);
  x->p = NULL;
  return y ? root(y) : NULL;
}

node *__merge(node *x, node *y) {
  node *l = y->l, *r = y->r;
  y->l = y->r = NULL, y->s = 1;

  if (r) x = __merge(x, r);
  x = _insert(x, y);
  if (l) x = __merge(x, l);
  return x;
}

/* return new root */
node *merge(node *x, node *y) {
  if (size(x) < size(y)) swap(x, y);
  if (!y) return x;
  return __merge(x, y);
}

void dfs(node *x) {
  putchar('(');
  if (x) {
    if (x->l) { dfs(x->l); putchar(' '); }
    printf("%d", x->d);
    if (x->r) { putchar(' '); dfs(x->r); }
  }
  putchar(')');
}
