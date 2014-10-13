#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 9; 
const int B = 3; 
const int R = N * N * N + 5; 
const int C = N * N * 4 + 5;
const int Z = R * 4 + C + 5;

struct node {
  int x, y;
  node *l, *r, *u, *d;
};

node nodes[Z], *next, *root, *row[R], *col[C];
int size[C];
int ans[N][N];

void init(int r, int c) {
  next = nodes;
  memset(row, 0, sizeof row);
  memset(size, 0, sizeof size);

  root = next++;
  root->l = root->r = root;
  for (int y = 0; y < c; y++) {
    node *p = next++;
    p->x = -1, p->y = y;
    p->r = root, p->l = root->l;
    p->r->l = p->l->r = p;
    col[y] = p->u = p->d = p;
  }
}

/* BETTER add from top to bottom, from left to right */
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

void cover(int c) {
  node *x = col[c], *y, *z;

  x->l->r = x->r;
  x->r->l = x->l;

  for (y = x->d; y != x; y = y->d)
    for (z = y->r; z != y; z = z->r) {
      z->u->d = z->d;
      z->d->u = z->u;
      size[z->y]--;
    }
}

void uncover(int c) {
  node *x = col[c], *y, *z;

  for (y = x->u; y != x; y = y->u)
    for (z = y->l; z != y; z = z->l) {
      z->u->d = z;
      z->d->u = z;
      size[z->y]++;
    }

  x->l->r = x;
  x->r->l = x;
}

int dfs(int dep) {
  node *x, *y, *z = NULL;

  for (x = root->r; x != root; x = x->r)
    if (!z || size[x->y] < size[z->y]) z = x;
  if (!z) return 1;

  cover(z->y);
  for (x = z->u; x != z; x = x->u) {
    int r = x->x;
    ans[r / N / N][r / N % N] = r % N;

    for (y = x->r; y != x; y = y->r)
      cover(y->y);

    if (dfs(dep + 1)) return 1;

    for (y = x->l; y != x; y = y->l)
      uncover(y->y);
  }
  uncover(z->y);

  return 0;
}

int main() {
  init(N * N * N, 4 * N * N);

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      for (int d = 0; d < N; d++) {
        int b = i / B * B + j / B;
        add(i * N * N + j * N + d, 0 * N * N + i * N + j);
        add(i * N * N + j * N + d, 1 * N * N + i * N + d);
        add(i * N * N + j * N + d, 2 * N * N + j * N + d);
        add(i * N * N + j * N + d, 3 * N * N + b * N + d);
      }

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      scanf("%1d", &ans[i][j]);
      if (--ans[i][j] != -1) {
        int x = i * N * N + j * N + ans[i][j];
        node *z = row[x];
        do {
          cover(z->y);
        } while ((z = z->r) != row[x]);
      }
    }

  if (dfs(0)) {
    for (int i = 0; i < N; i++, puts(""))
      for (int j = 0; j < N; j++)
        printf("%c", '1' + ans[i][j]);
  } else
    puts("no solution!");

  return 0;
}
