#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 100000;
const int inf = 800000000;
const int limit = 1000 * N;

int n, n1;
int hd[N + N], en;
struct edge { int v, w, p, a; } ed[N * 6];

void add(int u, int v, int w, int a = 1) {
  ed[en] = (edge) { v, w, hd[u], a }; // XXX g++ only!!
  hd[u] = en++;
}

void add2(int u, int v, int w) {
  add(u, v, w, 1);
  add(v, u, w, 0);
}

void init(int n) {
  en = 0;
  memset(hd, -1, sizeof(int) * n);
}

int q[N + N], s, t;
int size[N + N];
int pnt[N + N];

int m;
struct node {
  int d, f, *hid, *val, *loc, hn, le;
  node *l, *r, *p;
};
node a[N * 4];
node *leaf_loc[N + N];
node *root;

int rk[N + N], rkn;
int heap[3][6 << 20], hn;

inline int id(node *x, int v) {
  return rk[v] - x->le;
}

void heap_up(int *hid, int *loc, int *val, int sz, int x) {
  while (x) {
    int p = (x - 1) >> 1;
    if (val[hid[p]] >= val[hid[x]]) break;

    swap(hid[x], hid[p]);
    loc[hid[x]] = x;
    loc[hid[p]] = p;
    x = p;
  }
}

void heap_down(int *hid, int *loc, int *val, int sz, int x) {
  while (1) {
    int l = x + x + 1;
    int r = x + x + 2;
    int y = x;
    if (l < sz && val[hid[l]] > val[hid[y]]) y = l;
    if (r < sz && val[hid[r]] > val[hid[y]]) y = r;
    if (y == x) break;

    swap(hid[x], hid[y]);
    loc[hid[x]] = x;
    loc[hid[y]] = y;
    x = y;
  }
}

int vt[N + N], tt;

void heap_init(int st, node *x) {
  /* allocate memery */
  x->hid = heap[0] + hn;
  x->loc = heap[1] + hn;
  x->val = heap[2] + hn;
  hn += x->hn;

  /* bfs */
  tt++;
  x->val[id(x, st)] = 0;
  vt[st] = tt;

  s = t = 0;
  q[t++] = st;

  while (s < t) {
    int u = q[s++];
    int i = id(x, u);
    for (int z = hd[u]; z > -1; z = ed[z].p) if (ed[z].a >= 0) {
      int v = ed[z].v;
      int w = ed[z].w;
      int j = id(x, v);
      if (vt[v] < tt) {
        vt[v] = tt;
        x->val[j] = x->val[i] + w;
        q[t++] = v;
      }
    }
  }
  
  /* init heap */
  for (int z = 0; z < t; z++) {
    int u = q[z];
    int i = id(x, u);
    if (u >= n1) x->val[i] -= inf;
    x->loc[i] = x->hid[i] = i;
  }

  /* adjust heap */
  for (int i = t - 1; i >= 0; i--)
    heap_down(x->hid, x->loc, x->val, x->hn, i);
}

void update(node *x) {
  /* calculate f value */
  if (!x->l)
    x->f = x->val[0];
  else {
    x->f = max(x->l->f, x->r->f);
    int ld = x->l->val[x->l->hid[0]];
    int rd = x->r->val[x->r->hid[0]];
    if (abs(ld) <= limit && abs(rd) <= limit)
      x->f = max(x->f, ld + rd + x->d);
  }
}

node *divide(int rt) {
  /* find edge to cut */
  int bz, bv = -1, bu;
  
  s = t = 0;
  q[t++] = rt;

  while (s < t) {
    int u = q[s++];
    for (int z = hd[u]; z > -1; z = ed[z].p) if (ed[z].a == 1) {
      int v = ed[z].v;
      if (bv == -1 || abs(size[rt] - size[v] * 2) < abs(size[rt] - size[bv] * 2))
        bu = u, bz = z, bv = v;
      if (size[v] >= size[rt] / 2) q[t++] = v;
    }
  }

  /* single vertex */
  if (bv == -1) {
    node *x = a + (m++);
    x->l = x->r = NULL;
    x->hn = 1;
    leaf_loc[rt] = x;
    x->le = rk[rt] = rkn++;
    return x;
  }
  
  /* cut it */
  int sz = size[rt];
  ed[bz].a -= 2;
  ed[bz ^ 1].a -= 2;
  pnt[bv] = -1;
  for (int i = bu; i > -1; i = pnt[i])
    size[i] -= size[bv];
  
  /* divide recursively */
  node *x = a + (m++);
  x->d = ed[bz].w;
  x->hn = sz;
  x->l = divide(rt); heap_init(bu, x->l); update(x->l);
  x->r = divide(bv); heap_init(bv, x->r); update(x->r);
  x->l->p = x->r->p = x;
  x->le = x->l->le;

  /* recover the edge */
  ed[bz].a += 2;
  ed[bz ^ 1].a += 2;

  return x;
}

void change(int v) {
  for (node *x = leaf_loc[v]; x != root; x = x->p) {
    int i = id(x, v);
    if (abs(x->val[i]) <= limit) {
      x->val[i] -= inf;
      heap_down(x->hid, x->loc, x->val, x->hn, x->loc[i]);
    } else {
      x->val[i] += inf;
      heap_up(x->hid, x->loc, x->val, x->hn, x->loc[i]);
    }
    update(x);
  }
  update(root);
}

int main() {

  /* input */
  scanf("%d", &n); init(n + n); n1 = n;
  for (int i = 0; i < n - 1; i++) {
    int u, v, w;
    scanf("%d %d %d", &u, &v, &w);
    u--; v--;
    add(u, v, w);
    add(v, u, w);
  }

  /* build directed tree */
  s = t = 0;
  q[t++] = 0;
  size[0] = 1;
  while (s < t) {
    int u = q[s++];
    for (int z = hd[u]; z > -1; z = ed[z].p) {
      int v = ed[z].v;
      if (!size[v]) {
        size[v] = 1;
        q[t++] = v;
      } else ed[z].a = 0;
    }
  }
  
  /* modify the tree to binary tree, so that it can be divided equally */
  while (t) {
    int u = q[--t];

    int dd = 0;
    for (int z = hd[u]; z > -1; z = ed[z].p) if (ed[z].a == 1) dd++;
    
    int deg = 0, lz = -1, p = -1, pw = -1;
    for (int z = hd[u]; z > -1; z = ed[z].p) if (ed[z].a == 1) {
      deg++;
      int v = ed[lz = z].v;
      int w = ed[z].w;
      if (p == -1) {
        p = v;
        pw = w;
      } else if (deg < dd) {
        add2(n, p, p < n1 ? pw : 0);
        add2(n, v, w);
        pnt[p] = pnt[v] = n;
        size[n] = size[p] + size[v] + 1;
        p = n++;
      }
    }
    if (deg > 2) {
      for (int z = hd[u]; z > -1; z = ed[z].p) if (ed[z].a == 1)
        if (z != lz) ed[z].a = ed[z ^ 1].a = -1;
      add2(u, p, 0);
    }
    
    size[u] = 1;
    for (int z = hd[u]; z > -1; z = ed[z].p) if (ed[z].a == 1) {
      int v = ed[z].v;
      size[u] += size[v];
      pnt[v] = u;
    }
  }
  pnt[0] = -1;

  /* divide the binary tree */
  root = divide(0);
  update(root);

  /* query */
  int Q; scanf("%d", &Q);
  while (Q--) {
    char c; scanf(" %c", &c);
    if (c == 'C') {
      int v; scanf("%d", &v); v--;
      change(v);
    } else {
      int ans = root->f;
      if (ans >= 0)
        printf("%d\n", ans);
      else
        puts("They have disappeared.");
    }
  }

  return 0;
}
