#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int M = 10001 * 51 / 2;
const int L = 1000010;
const int K = 26;
const char BASE = 'a';

struct node {
  node *next[K], *fail;
  int word;
};

node a[M], *next, *root;

node *node_new() {
  node *x = next++;
  x->word = 0;
  memset(x->next, 0, sizeof(x->next));
  return x;
}

void init() {
  next = a;
  root = node_new();
}

void ins(char *key) {
  node *x = root;
  for (; *key; key++) {
    int z = *key - BASE;
    if (!x->next[z])
      x->next[z] = node_new();
    x = x->next[z];
  }
  x->word++;
}

void build() {
  static node *q[M];
  static int s, t;

  root->fail = NULL;
  s = t = 0; q[t++] = root;
  while (s < t) {
    node *u = q[s++];
    for (int z = 0; z < K; z++) {
      node *v = u->next[z];
      if (v) {
        node *x = u->fail;
        while (x && !x->next[z]) x = x->fail;
        v->fail = (x ? x->next[z] : root);
        q[t++] = v;
      }
    }
  }
}

node *child(node *x, int z) {
  while (x && !x->next[z]) x = x->fail;
  return x ? x->next[z] : root;
}

int run(char *str) {
  int ans = 0;
  for (node *x = root; *str; str++) {
    int z = *str - BASE;
    while (x && !x->next[z]) x = x->fail;
    if (x) {
      x = x->next[z];
      for (node *y = x; y && y->word != -1; y = y->fail) {
        ans += y->word;
        y->word = -1;
      }
    } else {
      x = root;
    }
  }
  return ans;
}

char str[L];

int main() {
  int T, n;
  
  scanf("%d", &T);
  while (T--) {
    init();
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
      scanf("%s", str);
      ins(str);
    }
    build();
    scanf("%s", str);
    printf("%d\n", run(str));
  }
  return 0;
}
