/**
 *   Copyright (C) 2021 All rights reserved.
 *   
 *   filename: data_structure.cpp
 *   author: doublehh
 *   e-mail: sserdoublehh@foxmail.com
 *   create time: 2021-08-01 05:06:46
 *   last modified: 2021-08-01 05:06:46
 */
#include <cstdio>
#include <random>
#include <iostream>
using namespace std;

template<typename T>
struct Treap {
    struct Node {
        Node *ch[2];
        int r;
        T v;
        int s;

        Node(T v): v(v) {
            ch[0] = ch[1] = NULL;
            r = rand();
            s = 1;
        }

        bool operator < (const Node &rhs) const {
            return r < rhs.r;
        }

        int cmp(T x) const {
            if (x == v) return -1;
            return x < v? 0: 1;
        }

        void maintain() {
            s = 1;
            if (ch[0] != NULL) s += ch[0]->s;
            if (ch[1] != NULL) s += ch[1]->s;
        }
    };

    void rotate(Node* &o, int d) {
        Node* k = o->ch[d ^ 1];
        o->ch[d ^ 1] = k->ch[d];
        k->ch[d] = o;
        o->maintain();
        k->maintain();
        o = k;
    }

    void insert(Node* &o, T x) {
        if (o == NULL) {
            o = new Node(x);
        } else {
            int d = (x < o->v? 0: 1);
            insert(o->ch[d], x);
            if (o->ch[d] > o) rotate(o, d ^ 1);
        }
        o->maintain();
    }

    void remove(Node* &o, T x) {
        int d = o->cmp(x);
        if (d == -1) {
            Node *u = o;
            if (o->ch[0] != NULL && o->ch[1] != NULL) {
                int d2 = (o->ch[0] > o->ch[1]? 1: 0);
                rotate(o, d2); remove(o->ch[d2], x);
            } else {
                if (o->ch[0] == NULL) o = o->ch[1];
                else o = o->ch[0];
                delete u;
            }
        } else
            remove(o->ch[d],x);
        if (o != NULL)
            o->maintain();
    }

    bool find(Node* o, T x) {
        while (o != NULL) {
            int d = o -> cmp(x);
            if (d == -1) return true;
            o = o->ch[d];
        }
        return false;
    }

    T kth(Node *o, int k) {
        if (o == NULL || k <= 0 || k > o->s) return -1;
        int s = (o->ch[0] == NULL? 0: o->ch[0]->s);
        if (k == s + 1) return o->v;
        else if (k <= s) return kth(o->ch[0], k);
        else return kth(o->ch[1], k - s - 1);
    }

    int rank(Node *o, T v) {
        int d = o->cmp(v);
        int s = (o->ch[0] == NULL? 0: o->ch[0]->s);
        if (d == -1) return s + 1;
        else if (d == 0) return rank(o->ch[0], v);
        else return s + 1 + rank(o->ch[1], v);
    }

    int upper_bound(Node *o, T v) {
        if (o == NULL) return 0;
        int d = o->cmp(v);
        int s = (o->ch[0] == NULL? 0: o->ch[0]->s);
        if (d == 0) return upper_bound(o->ch[0], v);
        else return s + 1 + upper_bound(o->ch[1], v);
    }

    Node *root;

    void Init() {
        root = NULL;
    }

    void Insert(T v) {
        insert(root, v);
    }

    void Remove(T v) {
        remove(root, v);
    }

    bool Find(T v) {
        // 在Treap中查找x是否存在
        return find(root, v);
    }

    T GetKth(int k) {
        // 计算排名第k的数是多少
        return kth(root, k);
    }

    int GetRank(T v) {
        // 计算v在Treap上的排名(v需要在Treap中)
        return rank(root, v);
    }

    int UpperBound(T v) {
        // 计算有多少节点的值不超过v
        return upper_bound(root, v);
    }
    
    int Size() {
        if (root == NULL) return 0;
        return root->s;
    }
};

template<typename T>
struct Splay {
    struct Node {
        Node *ch[2];
        T v;
        int s;
        bool flip;

        Node(int v): v(v), s(1),flip(0) {}

        int cmp(int k) const {
            int ts = (ch[0] ? ch[0]->s: 0) + 1;
            if (k == ts) return -1;
            return k < ts ? 0 : 1;
        }

        void maintain() {
            s = 1;
            if (ch[0]) s += ch[0]->s;
            if (ch[1]) s += ch[1]->s;
        }

        void pushdown() {
            if (flip) {
                flip = 0;
                swap(ch[0], ch[1]);
                if (ch[0]) ch[0]->flip ^= 1;
                if (ch[1]) ch[1]->flip ^= 1;
            }
        }
    };

    Node * root;

    Node* build(T *a, int sz) {
        if (!sz) return NULL;
        Node* o = new Node(*(a + sz / 2 + 1));
        o->ch[0] = build(a, sz / 2);
        o->ch[1] = build(a + sz / 2 + 1, sz - sz / 2 - 1);
        o->maintain();
        return o;
    }

    void Build(T * a, int n) {
        root = build(a, n);
    }

    // d = 0 left rotate, d = 1 right rotate
    void rotate(Node* &o, int d) {
        Node *k = o->ch[d ^ 1];
        o->ch[d ^ 1] = k->ch[d];
        k->ch[d] = o;
        o->maintain();
        k->maintain();
        o = k;
    }

    void splay(Node* &o, int k) {
        o->pushdown();
        int d = o->cmp(k);
        if (d == 1) k -= (o->ch[0]? o->ch[0]->s: 0) + 1;
        if (d != -1) {
            Node *p = o->ch[d];
            p->pushdown();
            int d2 = p->cmp(k);
            if (d2 == 1) k -= (p->ch[0]? p->ch[0]->s: 0) + 1;
            if (d2 != -1) {
                splay(p->ch[d2], k);
                if (d == d2) rotate(o, d ^ 1);
                else rotate(o->ch[d], d);
            }
            rotate(o, d^1);
        }
    }

    Node *merge(Node* left, Node* right) {
        splay(left, left->s);
        left->ch[1] = right;
        left->maintain();
        return left;
    }

    void split(Node* o, int k, Node* &left, Node* &right) {
        splay(o, k);
        left = o;
        right = o->ch[1];
        left->ch[1] = NULL;
        left->maintain();
    }
};

struct HDU_5887 {
    static const int N = 100000;
    int n;
    long long k, ans;
    int a[N], d[N];
    vector<int> G[N];
    Treap<long long> treap;

    void dfs(int u) {
        if (a[u] == 0)
            ans += treap.Size();
        else
            ans += treap.UpperBound(k / a[u]);
        treap.Insert(a[u]);
        for (int v: G[u]) dfs(v);
        treap.Remove(a[u]);
    }

    void Solve() {
        int T;
        cin >> T;
        while (T--) {
            cin >> n >> k;
            for (int i = 0; i < n; i++) {
                cin >> a[i];
                d[i] = 0;
                G[i].clear();
            }
            for (int i = 1; i < n; i++) {
                int u, v;
                cin >> u >> v;
                u--, v--;
                G[u].push_back(v);
                d[v]++;
            }
            ans = 0;
            treap.Init();
            for (int i = 0; i < n; i++)
                if (d[i] == 0)
                    dfs(i);
            cout << ans << endl;
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    HDU_5887 problem;
    problem.Solve();
}
