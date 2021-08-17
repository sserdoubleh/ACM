/**
 *   Copyright (C) 2021 All rights reserved.
 *   
 *   filename: code/string.cpp
 *   author: doublehh
 *   e-mail: sserdoubleh@gmail.com
 *   create time: 2021-08-17 03:15:19
 *   last modified: 2021-08-17 03:15:19
 */
#include <bits/stdc++.h>
using namespace std;

template<typename T>
class KMP {
private:
    int n, m;
    vector<T> s, t;
    vector<int> f;
public:
    KMP(const vector<T> &s, const vector<T> &t): s(s), t(t), n(s.size()), m(t.size()), f(n + 1) {}
    void pre() {
        f[0] = f[1] = 0;
        for (int i = 1; i < m; i++) {
            int j = f[i];
            while (j && t[i] != t[j]) j = f[j];
            f[i + 1] = t[i] == t[j]? j + 1: 0;
        }
    }
    int solve() {
        for (int i = 0, j = 0; i < n; i++) {
            while (j && s[i] != t[j]) j = f[j];
            if (s[i] == t[j]) j++;
            if (j == m) return i - m + 1;
        }
        return -1;
    }
};

class Trie {
private:
	static const int N = 600001, M = 26;
    int ch[N][M];
    bool val[N];
    int fa[N];
    int last[N];
    int sz;
    vector<int> word;
public:
    Trie() {
        sz = 1;
        val[0] = false;
    }
    inline int ind(char c) { return c - 'a'; }
    void insert(char *s, int n) {
        int u = 0;
        for (int i = 0; i < n; i++) {
            int c = ind(s[i]);
            if (!ch[u][c]) {
                val[sz] = 0;
                ch[u][c] = sz++;
            }
            u = ch[u][c];
        }
        val[u] = true;
        word.push_back(u);
    }
    void get_fail() {
        queue<int> q;
        fa[0] = 0;
        for (int i = 0; i < M; i++) if (ch[0][i]) {
            int u = ch[0][i];
            fa[u] = last[u] = 0;
            q.push(u);
        }
        while (!q.empty()) {
            int r = q.front(); q.pop();
            for (int c = 0; c < M; c++) if (ch[r][c]) {
                int u = ch[r][c];
                q.push(u);
                int v = fa[r];
                while (v && !ch[v][c]) v = fa[v];
                fa[u] = ch[v][c];
                last[u] = val[fa[u]? fa[u]: last[fa[u]]];
            }
        }
    }
    void print(int j, int &ans) {
        if (j) {
            ans++;
            val[j] = false;
        }
    }
    void find(char *s, int n, int &ans) {
        for (int i = 0, j = 0; i < n; i++) {
            int c = ind(s[i]);
            while (j && !ch[j][c]) j = ch[j][c];
            j = ch[j][c];
            if (val[j]) print(j, ans);
            else if (!last[j]) print(last[j], ans);
        }
    }
};

int main() {
    cin.tie(0), cout.tie(0);
    ios::sync_with_stdio(false);
    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<int> S(n), T(m);
        for (int i = 0; i < n; i++) cin >> S[i];
        for (int i = 0; i < m; i++) cin >> T[i];
        KMP<int> kmp(S, T);
        kmp.pre();
        int ans = kmp.solve();
        if (ans < 0) cout << "-1\n";
        else cout << ans + 1 << "\n";
    }
}
