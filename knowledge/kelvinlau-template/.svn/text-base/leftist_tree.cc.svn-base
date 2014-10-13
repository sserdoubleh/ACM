#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 100010;

typedef int T;

struct node {
  T v;
  int d;
  node *l, *r;
} nodes[N], *next;

void init() {
  next = nodes;
}

node *node_new(T v) {
  node *x = next++;
  x->l = x->r = NULL;
  x->d = 0;
  x->v = v;
  return x;
}

inline int dist(node *x) {
  return x ? x->d : -1;
}

node *merge(node *x, node *y) {
  if (!x) return y;
  if (!y) return x;
  if (y->v > x->v) swap(x, y);
  x->r = merge(x->r, y);
  if (dist(x->l) < dist(x->r))
    swap(x->l, x->r);
  x->d = dist(x->r) + 1;
  return x;
}

node *pop(node *x) {
  node *y = merge(x->l, x->r);
  x->l = x->r = NULL;
  x->d = 0;
  return y;
}
