#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 20010;
const int M = 200010;
const int INF = 0x3f3f3f3f;

int n, en, hd[N];
struct edge { 
  int v, w, p; 
} ed[M];
int dist[N];
int heap[N], pos[N], hn;

void init(int n) {
  en = 0; 
  memset(hd, -1, sizeof(hd[0]) * n);
}

void add(int u, int v, int w) {
  ed[en] = (edge) { v, w, hd[u] }; // XXX g++ only!!
  hd[u] = en++;
}

void heap_init(int n) {
  hn = n;
  for (int i = 0; i < n; i++)
    heap[i] = pos[i] = i;
}

void heap_up(int x) {
  int p;
  while (x && dist[heap[x]] < dist[heap[p = (x - 1) >> 1]]) {
    swap(heap[x], heap[p]);
    swap(pos[heap[x]], pos[heap[p]]);
    x = p;
  }
}

void heap_down(int x) {
  int c;
  while ((c = (x << 1) + 1) < hn) {
    if (c + 1 < hn && dist[heap[c + 1]] < dist[heap[c]]) c++;
    if (dist[heap[c]] < dist[heap[x]]) {
      swap(heap[x], heap[c]);
      swap(pos[heap[x]], pos[heap[c]]);
      x = c;
    } else break;
  }
}

int heap_pop() {
  int t = heap[0];
  pos[heap[0] = heap[--hn]] = 0;
  heap_down(0);
  return t;
}

int main() {
  int n, m, s, u, v, w, z;

  scanf("%d %d %d", &n, &m, &s);
  init(n);
  while (m--) {
    scanf("%d %d %d", &u, &v, &w);
    add(u, v, w);
  }

  heap_init(n);
  memset(dist, 0x3f, sizeof(dist[0]) * n); 
  dist[s] = 0;
  heap_up(pos[s]);

  while (hn) {
    u = heap_pop();
    for (z = hd[u]; z > -1; z = ed[z].p) {
      v = ed[z].v, w = ed[z].w;
      if (dist[v] > dist[u] + w) {
        dist[v] = dist[u] + w;
        heap_up(pos[v]);
      }
    }
  }

  for (u = 0; u < n; u++)
    printf("%d\n", dist[u] < INF ? dist[u] : -1);

  return 0;
}
