#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 50;
const int R = N; 
const int C = 64
const int Z = R * C;

struct node {
  int x, y;
  node *l, *r, *u, *d;
};

node nodes[Z], *next, *root, *row[R], *col[C];
int size[C];
bool mark[C];

void init(int r, int c) {
  next = nodes;
  memset(row, 0, sizeof row);
  memset(size, 0, sizeof size);

  root = next++;
  root->l = root->r = root;
  for (int y = 0; y < c; y++) {
    node *p = next++;
    p->x = -1, p->y = y;
    p->r = root;
    p->l = root->l;
    p->r->l = p->l->r = p;
    col[y] = p->u = p->d = p;
  }
}

/* MUST add from top to bottom, from left to right */
node *add(int x, int y) {
  node *p = next++;

  p->x = x, p->y = y;
  size[y]++;

  if (!row[x]) {
    row[x] = p->l = p->r = p;
  } else {
    p->r = row[x];
    p->l = row[x]->l;
    p->r->l = p->l->r = p;
  }

  p->d = col[y];
  p->u = col[y]->u;
  p->u->d = p->d->u = p;

  return p;
}

void cover(node *x) {
  for (node *y = x->d; y != x; y = y->d) {
    y->l->r = y->r;
    y->r->l = y->l;
    size[x->y]--;
  }
}

void uncover(node *x) {
  for (node *y = x->u; y != x; y = y->u) {
    y->l->r = y->r->l = y;
    size[x->y]++;
  }
}

int h() {
  int res = 0;
  node *x, *y, *z;

  memset(mark, 0, sizeof mark);
  for (x = root->l; x != root; x = x->l) if (!mark[x->y]) {
    mark[x->y] = 1;
    res++;
    for (y = x->u; y != x; y = y->u)
      for (z = y->r; z != y; z = z->r)
        mark[z->y] = 1;
  }
  return res;
}

int dfs(int dep) {
  node *x, *y, *z = NULL;

  if (dep < h()) return 0;

  for (x = root->r; x != root; x = x->r)
    if (!z || size[x->y] < size[z->y]) z = x;
  if (!z) return 1;

  if (!dep) return 0;

  for (x = z->u; x != z; x = x->u) {
    cover(x);
    for (y = x->r; y != x; y = y->r)
      cover(y);

    if (dfs(dep - 1)) return 1;

    for (y = x->l; y != x; y = y->l)
      uncover(y);
    uncover(x);
  }

  return 0;
}
