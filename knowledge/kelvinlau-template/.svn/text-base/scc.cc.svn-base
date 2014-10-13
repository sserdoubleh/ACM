#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
using namespace std;

const int N = 10010;

int n, m, c;
int mark[N], stack[N], top;
vector<int> vout1[N], vout2[N];

void dfs1(int u) {
  mark[u] = -1;
  for (int z = 0; z < vout1[u].size(); z++) {
    int v = vout1[u][z];
    if (!mark[v]) dfs1(v);
  }
  stack[top++] = u;
}

void dfs2(int u, int k) {
  mark[u] = k;
  for (int z = 0; z < vout2[u].size(); z++) {
    int v = vout2[u][z];
    if (mark[v] == -1) dfs2(v, k);
  }
}

int main() {
  scanf("%d %d", &n, &m);
  while (m--) {
    int u, v;
    scanf("%d %d", &u, &v);
    vout1[u].push_back(v);
    vout2[v].push_back(u);
  }
    
  top = 0;
  memset(mark, 0, n * sizeof(mark[0]));
  for (int u = 0; u < n; u++)
    if (!mark[u]) dfs1(u);
  
  c = 0;
  while (top) {
    int u = stack[--top];
    if (mark[u] == -1) dfs2(u, c++);
  }
  
  return 0;
}
