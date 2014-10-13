#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 300000 + 10;

struct node {
  node *l, *r, *p, *q;
  int z, f, g, w;
} nodes[N];

inline node *up(node *x) {
  node *l = x->l;
  node *r = x->r;
  x->g = max((l ? max(l->w, l->g) + l->f : 0), 
             (r ? max(r->w, r->g) + r->f : 0));
  return x;
}

inline node *down(node *x) {
  if (x->z) {
    x->z = 0;
    swap(x->l, x->r);
    if (x->l) x->l->z ^= 1;
    if (x->r) x->r->z ^= 1;
  }
  if (x->f) {
    if (x->l) x->l->f += x->f;
    if (x->r) x->r->f += x->f;
    x->w += x->f;
    x->g += x->f;
    x->f = 0;
  }
  return x;
}

inline void lr(node *x) {
  node *y = x->r;
  node *b = y->l;
  if (x->r = b) 
    b->p = x;
  y->p = x->p;
  if (x->p) {
    if (x->p->l == x)
      x->p->l = y;
    else
      x->p->r = y;
  }
  y->l = x;
  x->p = y;
  swap(x->q, y->q);
  up(x);
  up(y);
}

inline void rr(node *x) {
  node *y = x->l;
  node *b = y->r;
  if (x->l = b) 
    b->p = x;
  y->p = x->p;
  if (x->p) {
    if (x->p->l == x)
      x->p->l = y;
    else
      x->p->r = y;
  }
  y->r = x;
  x->p = y;
  swap(x->q, y->q);
  up(x);
  up(y);
}

void splay(node *x) {
  node *p;
  while (p = x->p) {
    down(p); 
    down(x);
    if (p->l == x) 
      rr(p); 
    else 
      lr(p);
  }
  down(x);
}

void access(node *x) {
  node *u = x;
  node *v = NULL;
  do {
    splay(u);
    if (u->r) {
      u->r->q = u;
      u->r->p = NULL;
    }
    u->r = v;
    if (v) {
      v->p = u;
      v->q = NULL;
    }
    up(u);
    v = u;
    u = u->q;
  } while (u);

  splay(x);
}

/* rotate the tree, let x be the new root */
void rotate(node *x) {
  access(x);
  x->z ^= 1;
}

/* cut out x and its subtree */
void cut(node *x) {
  access(x);
  if (x->l) {
    x->l->p = NULL;
    x->l = NULL;
    up(x);
  }
}

/* add an edge u -> v, v must be a root */
void join(node *u, node *v) {
  access(u);
  access(v);
  v->l = u;
  u->p = v;
  up(v);
}

/* root of x */
node *root(node *x) {  
  access(x);
  while (down(x)->l) x = x->l;
  return x;
}

/* parent of x */
node *parent(node *x) {
  access(x);
  x = x->l;
  if (!x) return NULL;
  while (down(x)->r) x = x->r;
  return x;
}

/* weight += d for nodes on path from x to y */
void add(node *x, node *y, int d) {
  rotate(x);
  access(y);
  y->f += d;
}

/* max weight of nodes on path from x to y */
int query(node *x, node *y) {
  rotate(x);
  access(y);
  return y->f + max(y->w, y->g);
}
