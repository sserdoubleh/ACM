#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
using namespace std;

const int N = 20000 + 10;

/* graph */
int n, m;
int d[N];
typedef pair<int, int> edge;
vector<edge> vout[N];

void build_tree(int u) {
  for (int z = 0; z < vout[u].size(); z++) {
    int v = vout[u][z].first;
    int w = vout[u][z].second;
    if (d[v] == -1) {
      d[v] = d[u] + w;
      build_tree(v);
    } else {
      swap(vout[u][z], vout[u].back());
      vout[u].pop_back();
      z--;
    }
  }
}

/* disjoint-set */
int pnt[N];

int root(int x) {
  return pnt[x] = (pnt[x] == x ? x : root(pnt[x]));
}

void merge(int x, int y) {
  x = root(x);
  y = root(y);
  if (rand() % 2)
    pnt[x] = y;
  else
    pnt[y] = x;
}

/* tarjan */
struct Quest {
  int a, b, x;
} quest[N];

int lca[N];
bool visited[N];
vector<int> q[N];

void tarjan(int u) {
  pnt[u] = lca[u] = u;
  for (int z = 0; z < vout[u].size(); z++) {
    int v = vout[u][z].first;
    tarjan(v);
    merge(u, v);
    lca[root(u)] = u;
  }
  visited[u] = 1;
  for (int z = 0; z < q[u].size(); z++) {
    int i = q[u][z];
    int v = quest[i].a != u ? quest[i].a : quest[i].b;
    if (visited[v])
      quest[i].x = lca[root(v)];
  }
}

int main() {
  /* input */
  scanf("%d %d", &n, &m);
  for (int i = 0; i < n; i++)
    vout[i].clear();
  for (int e = 0; e < n - 1; e++) {
    int u, v, w;
    scanf("%d %d %d", &u, &v, &w);
    u--;
    v--;
    vout[u].push_back(edge(v, w));
    vout[v].push_back(edge(u, w));
  }

  /* build tree */
  memset(d, -1, sizeof(d));
  d[0] = 0;
  build_tree(0);

  /* read quest */
  for (int i = 0; i < n; i++)
    q[i].clear();

  for (int i = 0; i < m; i++) {
    int a, b;
    scanf("%d %d", &a, &b);
    a--;
    b--;
    quest[i].a = a;
    quest[i].b = b;
    q[a].push_back(i);
    q[b].push_back(i);
  }

  /* tarjan offline lca algorithm */
  memset(visited, 0, sizeof visited);
  tarjan(0);

  /* output */
  for (int i = 0; i < m; i++) {
    int a = quest[i].a;
    int b = quest[i].b;
    int x = quest[i].x;
    printf("%d\n", d[a] + d[b] - 2 * d[x]);
  }
  puts("");

  return 0;
}
