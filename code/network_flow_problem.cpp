/**
 *   Copyright (C) 2021 All rights reserved.
 *   
 *   filename: network_flow_problem.cpp
 *   author: doublehh
 *   e-mail: sserdoublehh@foxmail.com
 *   create time: 2021-08-01 00:11:38
 *   last modified: 2021-08-01 00:11:38
 */
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

template<typename T>
struct Dinic {

    struct Edge {
        int from, to;
        T cap, flow;
        Edge(int from, int to, T cap, T flow):
            from(from), to(to), cap(cap), flow(flow) {}
    };

    static const int N = 200;
    static const T INF = 1 << 30;
    int n, m, s, t;

    vector<Edge> edges;
    vector<int> G[N];
    bool vis[N];
    int d[N];
    int cur[N];

    void Init(int n) {
        this->n = n;
        edges.clear();
        for (int i = 0; i < n; i++) G[i].clear();
    }

    void AddEdge(int from, int to, T cap) {
        edges.emplace_back(from, to, cap, 0);
        edges.emplace_back(to, from, 0, 0);
        m = edges.size();
        G[from].push_back(m - 2);
        G[to].push_back(m - 1);
    }

    bool bfs() {
        for (int i = 0; i < n; i++) vis[i] = false;
        queue<int> Q;
        Q.push(s), d[s] = 0, vis[s] = true;
        while (!Q.empty()) {
            int x = Q.front();
            Q.pop();
            for (int eid: G[x]) {
                Edge &e = edges[eid];
                if (!vis[e.to] && e.cap > e.flow) {
                    vis[e.to] = true;
                    d[e.to] = d[x] + 1;
                    Q.push(e.to);
                }
            }
        }
        return vis[t];
    }

    T dfs(int x, T a) {
        if (x == t || a == 0) return a;
        T flow = 0, f;
        for (int &i = cur[x]; i < (int)G[x].size(); i++) {
            int eid = G[x][i];
            Edge &e = edges[eid];
            if (d[x] + 1 == d[e.to] && (f = dfs(e.to, min(a, e.cap - e.flow))) > 0) {
                e.flow += f;
                edges[eid ^ 1].flow -= f;
                flow += f;
                a -= f;
                if (a == 0) break;
            }
        }
        return flow;
    }

    T Maxflow(int s, int t) {
        this->s = s;
        this->t = t;
        T flow = 0;
        while (bfs()) {
            for (int i = 0; i < n; i++) cur[i] = 0;
            flow += dfs(s, INF);
        }
        return flow;
    }
};

template<typename T>
struct MCMF {

    struct Edge {
        int from, to;
        T cap, flow, cost;
        Edge(int from, int to, T cap, T flow,T cost)
            :from(from), to(to), cap(cap), flow(flow), cost(cost) {}
    };

    static const int N = 202;
    static const T INF = 1 << 30;
    int n, m, s, t;
    vector<Edge> edges;
    vector<int> G[N];
    bool inq[N];
    int d[N];
    int p[N];
    int a[N];

    void AddEdge(int from, int to, T cap, T cost) {
        edges.emplace_back(from, to, cap, 0, cost);
        edges.emplace_back(to, from, 0, 0, -cost);
        m = edges.size();
        G[from].push_back(m - 2);
        G[to].push_back(m - 1);
    }

    void Init(int n) {
        this->n = n;
        edges.clear();
        for (int i = 0; i < n; i++) G[i].clear();
    }

    bool BellmanFord(int s, int t, T &flow, T &cost) {
        for (int i = 0; i < n; i++) d[i] = INF;
        memset(inq, 0, sizeof(inq));
        d[s] = 0, inq[s] = true, p[s] = 0, a[s] = INF;
        queue<int> Q;
        Q.push(s);
        while (!Q.empty()) {
            int u = Q.front(); Q.pop();
            inq[u]=0;
            for (int eid: G[u]) {
                Edge &e = edges[eid];
                if (e.cap > e.flow && d[e.to] > d[u] + e.cost) {
                    d[e.to] = d[u] + e.cost;
                    p[e.to] = eid;
                    a[e.to] = min(a[u], e.cap - e.flow);
                    if (!inq[e.to]) {
                        Q.push(e.to);
                        inq[e.to] = true;
                    }
                }
            }
        }
        if (d[t] == INF) return false;
        flow += a[t];
        cost += d[t] * a[t];
        int u = t;
        while (u != s) {
            edges[p[u]].flow += a[t];
            edges[p[u] ^ 1].flow -= a[t];
            u = edges[p[u]].from;
        }
        return true;
    }

    pair<T, T> MaxflowMinCost(int s, int t) {
        this->s = s, this->t = t;
        T flow = 0, cost = 0;
        while (BellmanFord(s, t, flow, cost));
        return make_pair(flow, cost);
    }
};

void poj_1273() {
    Dinic<int> solver;
    int n, m;
    while (cin >> m >> n) {
        solver.Init(n);
        while (m--) {
            int s, e, c;
            cin >> s >> e >> c;
            solver.AddEdge(s - 1, e - 1, c);
        }
        cout << solver.Maxflow(0, n - 1) << endl;
    }
}

void poj_2195() {
    MCMF<int> solver;
    int n, m;
    const int N = 101;
    string s[N];
    while (cin >> n >> m) {
        vector< pair<int, int> > x;
        vector< pair<int, int> > y;
        if (n == 0 && m == 0) break;

        for (int i = 0; i < n; i++)
            cin >> s[i];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                if (s[i][j] == 'm') x.emplace_back(i, j);
                if (s[i][j] == 'H') y.emplace_back(i, j);
            }
        int sz = x.size();
        solver.Init(2 * sz + 2);
        for (int i = 0; i < sz; i++) {
            solver.AddEdge(2 * sz, i, 1, 0);
            solver.AddEdge(sz + i, 2 * sz + 1, 1, 0);
        }
        for (int i = 0; i < sz; i++)
            for (int j = 0; j < sz; j++)
                solver.AddEdge(
                    i, sz + j, 1,
                    abs(x[i].first - y[j].first) + abs(x[i].second - y[j].second));
        cout << solver.MaxflowMinCost(2 * sz, 2 * sz + 1).second << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    poj_2195();
}
