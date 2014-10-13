#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int N = 1010;
const int M = 100010;
const int INF = 0x3f3f3f3f;

struct edge { int u, v, f, c, p; } ed[M];
int en, hd[N];

void init() {
  en = 0;
  memset(hd, -1, sizeof hd);
}

void add_edge(int u, int v, int f, int c) {
  ed[en] = (edge) { u, v, f, +c, hd[u] }; hd[u] = en++; // XXX g++ only!!
  ed[en] = (edge) { v, u, 0, -c, hd[v] }; hd[v] = en++; // XXX g++ only!!
}

int min_cost_max_flow(int s, int t) {
  static int q[N];
  static int in[N];
  static int best[N];
  static int prev[N];
  int flow = 0, cost = 0;
  int a, b, u, v, z, d, f, c, e;

  while (1) {
    memset(in, 0, sizeof in);
    memset(prev, 0xff, sizeof prev);
    memset(best, 0x3f, sizeof best); 
    
    a = b = best[s] = 0;
    in[q[b++] = s] = 1;
    while (a != b) {
      in[u = q[a]] = 0;
      a = (a + 1) % N;
      for (z = hd[u]; z > -1; z = ed[z].p) {
        v = ed[z].v, f = ed[z].f, c = ed[z].c;
        if (f && best[v] > best[u] + c) {
          best[v] = best[u] + c;
          prev[v] = z;
          if (!in[v]) {
            in[q[b] = v] = 1;
            b = (b + 1) % N;
          }
        }
      }
    }
    if (prev[t] == -1) break;
    
    d = INF;
    for (v = t; v != s; v = ed[e].u) {
      e = prev[v];
      d = min(d, ed[e].f);
    }
    flow += d;
    
    for (v = t, e; v != s; v = ed[e].u) {
      e = prev[v];
      ed[e ^ 0].f -= d;
      ed[e ^ 1].f += d;
      cost += d * ed[e].c;
    }
  }
  return cost;
}
