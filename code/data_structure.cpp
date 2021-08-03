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

// SparseTable
template <typename T, class F = function<T(const T&, const T&)>>
class SparseTable {
public:
    int n;
    vector<vector<T>> mat;
    F func;

    SparseTable(const vector<T>& a, const F& f) : func(f) {
        n = static_cast<int>(a.size());
        int max_log = 32 - __builtin_clz(n);
        mat.resize(max_log);
        mat[0] = a;
        for (int j = 1; j < max_log; j++) {
            mat[j].resize(n - (1 << j) + 1);
            for (int i = 0; i <= n - (1 << j); i++) {
                mat[j][i] = func(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    T get(int from, int to) const {
        assert(0 <= from && from <= to && to <= n - 1);
        int lg = 32 - __builtin_clz(to - from + 1) - 1;
        return func(mat[lg][from], mat[lg][to - (1 << lg) + 1]);
    }
};

template<typename T>
class Treap {
private:
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
            int d = x < o->v? 0: 1;
            insert(o->ch[d], x);
            if (o->ch[d] > o) rotate(o, d ^ 1);
        }
        o->maintain();
    }

    void remove(Node* &o, T x) {
        if (o == NULL) return;
        int d = o->cmp(x);
        if (d == -1) {
            Node *u = o;
            if (o->ch[0] != NULL && o->ch[1] != NULL) {
                int d2 = o->ch[0] > o->ch[1]? 1: 0;
                rotate(o, d2);
                remove(o->ch[d2], x);
            } else {
                if (o->ch[0] == NULL) o = o->ch[1];
                else o = o->ch[0];
                delete u;
            }
        } else
            remove(o->ch[d], x);
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
        int s = o->ch[0] == NULL? 0: o->ch[0]->s;
        if (k == s + 1) return o->v;
        else if (k <= s) return kth(o->ch[0], k);
        else return kth(o->ch[1], k - s - 1);
    }

    int rank(Node *o, T v) {
        int d = o->cmp(v);
        int s = o->ch[0] == NULL? 0: o->ch[0]->s;
        if (d == -1) return s + 1;
        else if (d == 0) return rank(o->ch[0], v);
        else return s + 1 + rank(o->ch[1], v);
    }

    int upper_bound(Node *o, T v) {
        if (o == NULL) return 0;
        int d = o->cmp(v);
        int s = o->ch[0] == NULL? 0: o->ch[0]->s;
        if (d == 0) return upper_bound(o->ch[0], v);
        else return s + 1 + upper_bound(o->ch[1], v);
    }

    Node *root;

public:
    void Init() {
        root = NULL;
    }

    void Insert(T v) {
        // 将一个元素v插入
        insert(root, v);
    }

    void Remove(T v) {
        // 将一个元素v删除，允许删除一个不存在的元素（不会修改Treap）
        remove(root, v);
    }

    bool Find(T v) {
        // 在Treap中查找元素v是否存在
        return find(root, v);
    }

    T GetKth(int k) {
        // 返回排名第k小的元素
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
class Splay {
private:
    static const int N = 100000;
    struct Node {
        Node *ch[2];
        T v;
        int s;
        int flip;

        Node(T v): v(v), s(1), flip(0) {}

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

    Node* build(T *a, int sz) {
        if (!sz) return NULL;
        Node* o = new Node(*(a + sz / 2));
        o->ch[0] = build(a, sz / 2);
        o->ch[1] = build(a + sz / 2 + 1, sz - sz / 2 - 1);
        o->maintain();
        return o;
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
        // splay核心操作，将序列的第k个元素所在的节点变为splay的根节点
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
            rotate(o, d ^ 1);
        }
    }

    void print(Node* o) {
        o->pushdown();
        if (o->ch[0]) print(o->ch[0]);
        if (o->v > 0) cout << o->v << "\n"; // 任务相关，不输出虚拟节点
        if (o->ch[1]) print(o->ch[1]);
    }

    Node* merge(Node* left, Node* right) {
        // 合并两个splay维护的序列
        // 需要保证left不为NULL，right则没有这个要求
        splay(left, left->s);
        left->ch[1] = right;
        left->maintain();
        return left;
    }

    void split(Node* o, int k, Node* &left, Node* &right) {
        // 从位置k切分splay维护的序列
        // 切分后，用两个splay维护切分后的序列
        splay(o, k);
        left = o;
        right = o->ch[1];
        left->ch[1] = NULL;
        left->maintain();
    }

    Node* root;

public:
    void Build(T * a, int n) {
        // 构建一个由splay维护的序列
        root = build(a, n);
    }

    void ReverseAndAppend(int l, int r) {
        // 任务相关的合成操作，其余操作可以根据参考这一个实现
        Node *left, *mid, *right, *temp;
        split(root, l, left, temp);
        split(temp, r - l + 1, mid, right);
        mid->flip ^= 1;
        root = merge(merge(left, right), mid);
    }

    void Print() {
        // 遍历Splay，输出序列
        print(root);
    }
};

class HDU_5887 {
private:
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

public:
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

class UVA_11922 {
private:
    static const int N = 100000;
    int a[N + 1];
    Splay<int> splay;

public:
    void Solve() {
        int n, m;
        cin >> n >> m;
        for (int i = 0; i <= n; i++) a[i] = i;
        // 需要新建一个虚拟节点，方便操作，无需再考虑是否存在NULL
        splay.Build(a, n + 1);
        while (m--) {
            int l, r;
            cin >> l >> r;
            splay.ReverseAndAppend(l, r);
        }
        splay.Print();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    UVA_11922 problem;
    problem.Solve();
}
