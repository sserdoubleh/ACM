/* suffix automata
 * a dfa accepts all suffixes of some strings */
#include <stdio.h>
#include <string.h>

const int L = 1 << 17;
const int K = 26;
const char BASE = 'a';

struct node {
  int len, term;
  node *next[K], *fail;
} nodes[L << 1], *next_node, *start;

node *node_new(int len) {
  node *x = next_node++;
  memset(x, 0x00, sizeof *x);
  x->len = len;
  return x;
}

node *node_clone(node *y, int len) {
  node *x = next_node++;
  *x = *y;
  x->len = len;
  return x;
}

/* init sam, make a start-state
 * returns start-state */
void sam_init() {
  next_node = nodes;
  start = node_new(0);
}

/* extend a char from state end
 * returns new furthest accept-state */
node *sam_extend(node *end, int c) {
  node *x = node_new(end->len + 1), *p;
  for (p = end; p && !p->next[c]; p = p->fail)
    p->next[c] = x;
  if (p) {
    node *q = p->next[c];
    if (p->len + 1 < q->len) {
      node *q1 = node_clone(q, p->len + 1);
      x->fail = q->fail = q1;
      for (; p && p->next[c] == q; p = p->fail)
        p->next[c] = q1;
    } else {
      x->fail = q;
    }
  } else {
    x->fail = start;
  }
  return x;
}

/* add a string to sam */
void sam_add(const char *str) {
  node *x = start;
  for (const char *p = str; *p; p++) {
    int c = *p - BASE;
    if (x->next[c] && x->next[c]->len == x->len + 1)
      x = x->next[c];
    else
      x = sam_extend(x, c);
  }
  while (x != start) {
    x->term = 1;
    x = x->fail;
  }
}

/* judge whether suffix is a suffix of one of the string */
int sam_run(const char *suffix) {
  node *x = start;
  for (const char *p = suffix; *p; p++) {
    int c = *p - BASE;
    x = x->next[c];
    if (!x)
      return 0;
  }
  return x->term;
}

/* display sam */
void sam_display() {
  for (node *x = nodes, *y; x < next_node; x++) {
    for (int c = 0; c < K; c++)
      if (y = x->next[c])
        printf("%d :%c -> %d\n", x - nodes, BASE + c, y - nodes);
    printf("%d fail -> %d\n", x - nodes, x->fail - nodes);
    if (x->term)
      printf("%d terminal\n", x - nodes);
    puts("");
  }
}

int main() {
  static char str[L];
  int n;
  
  sam_init();
  scanf("%d", &n);
  while (n--) {
    scanf("%s", str);
    sam_add(str);
  }

  while (scanf("%s", str) == 1)
    printf("%s: %s\n", str, sam_run(str) ? "accept" : "fail");

  return 0;
}
