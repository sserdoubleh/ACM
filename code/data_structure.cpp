/**
 *   Copyright (C) 2021 All rights reserved.
 *   
 *   filename: data_structure.cpp
 *   author: doublehh
 *   e-mail: sserdoublehh@foxmail.com
 *   create time: 2021-08-01 05:06:46
 *   last modified: 2021-08-01 05:06:46
 */
#include <bits/stdc++.h>
using namespace std;

// Point update + Prefix query
// 1. Update: tree.Update(P, val)
// 2. Query: bit.Sum(P)
// Requirement:
// 0. Zero: 0 + a = a
// 1. Associative law: a + b + c = (a + b) + c = a + (b + c)
// example: addition, multiplication, maximum, minimum
//
// Point update + Range update
// Note: Query slow than ZKWSegmentTree
// 1. Update: tree.Update(P, val)
// 2. Query: bit.Sum(R) - bit.Sum(L - 1)
// Requirement:
// 0. Zero: 0 + a = a
// 1. Associative law: a + b + c = (a + b) + c = a + (b + c)
// 2. a + b - b = a
// example: addition, multiplication
//
// Range update + Point query
// 1. Update: bit.Update(L, val), bit.Update(R + 1, val)
// 2. Query: bit.Sum(P, val)
// Requirement:
// 0. Zero: 0 + a = a
// 1. Associative law: a + b + c = (a + b) + c = a + (b + c)
// 2. a + b - b = a
// example: addition, multiplication
//
// Note:
// 1. Index start from 1
template<typename T>
class BinaryIndexedTree {
private:
    vector<T> c;

public:
    void Build(int n) {
        c = vector<T>(n + 1, 0);
    }

    void Add(int x, T v) {
        while (x < c.size()) {
            // TODO: Overloaded
            c[x] += v;
            x += x&-x;
        }
    }

    T Sum(int x) {
        T ret = 0;
        while (x) {
            // TODO: Overloaded
            ret += c[x];
            x -= x&-x;
        }
        return ret;
    }
};

// Note:
// 1. Indexed from 0.
class DisjointSet {
private:
    vector<int> pa;

public:
    void Build(int n) {
        pa.resize(n);
        for (int i = 0; i < n; i++) pa[i] = i;
    }

    int Find(int x) {
        return x == pa[x]? x: pa[x] = Find(pa[x]);
    }

    bool Union(int x, int y) {
        int fx = Find(x), fy = Find(y);
        if (fx == fy) return false;
        pa[fx] = fy;
        // TODO: Overloaded
        return true;
    }
};

// Range query
// 1. Query: table.Query(L, R)
// Requirement:
// 0. Zero: 0 + a = a
// 1. Associative law: a + b + c = (a + b) + c = a + (b + c)
// example: addition, multiplication, maximum, minimum
//
// Note:
// 1. Indexed from 0.
template <typename T, class F = function<T(const T&, const T&)>>
class SparseTable {
private:
    int n;
    vector<vector<T>> mat;
    F func;

public:
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

    T Query(int from, int to) const {
        assert(0 <= from && from <= to && to <= n - 1);
        int lg = 32 - __builtin_clz(to - from + 1) - 1;
        return func(mat[lg][from], mat[lg][to - (1 << lg) + 1]);
    }
};

// Point update + Range query
// 1. Update: tree.Update(P, val)
// 2. Query: tree.Query(L, R)
// Requirement:
// 0. Zero: 0 + a = a
// 1. Associative law: a + b + c = (a + b) + c = a + (b + c)
// example: addition, multiplication, maximum, minimum
//
// Range update + point query
// 1. Update: tree.Update(L, val), tree.Update(R + 1, -val)
// 2. Query: tree.Query(1, P)
// Requirement:
// 0. Zero: 0 + a = a
// 1. Associative law: a + b + c = (a + b) + c = a + (b + c)
// 2. a + b - b = a
// example: addition, multiplication
//
// Note:
// 1. Index start from 1
template<typename T>
class ZKWSegmentTree {
private:
    int M;
    vector<T> sumv;

public:
    void Init(int n) {
        for (M = 1; M < n; M <<= 1);
        sumv = vector<T>(M<<1);
        for (int i = 1; i < M<<1; i++)
            // TODO: Overloaded
            sumv[i] = 0;
    }

    void Update(int u, T v) {
        for (u += M; u; u >>= 1)
            // TODO: Overloaded
            sumv[u] += v;
    }

    T Query(int u, int v) {
        T ret = 0;
        if (v < u) return ret;
        for (u += M - 1, v += M + 1; u ^ v ^ 1; u >>= 1, v >>= 1) {
            // TODO: Overloaded
            if (~u&1) ret += sumv[u ^ 1];
            if ( v&1) ret += sumv[v ^ 1];
        }
        return ret;
    }
};

// Point update + Range query
// Range update + point query
//
// Range update + Range query
// Support lazy update: up / down
// 0. Build: tree.Build(n)
// 1. Update: tree.Update(L, R)
// 2. Query: tree.Query(L, R)
//
// Note:
// 1. Index start from 1
template<typename T>
class SegmentTree {
private:
#define xxx int lc = o<<1, rc = o<<1|1, mid = (l + r) >> 1
#define lson lc, l, mid
#define rson rc, mid + 1, r

    int n;
    vector<T> sumv;
    vector<T> addv;

    int ql, qr;
    T qv;
    T q_sum;

    // 初始化线段树
    void init(int o, int l, int r) {
        sumv[o] = addv[o] = 0;
        if (l == r) return;
        xxx;
        init(lson);
        init(rson);
    }

    // 维护线段树
    void down(int o, int l, int r) {
        // TODO: Overloaded
        if (addv[o]) {
            xxx;
            addv[lc] += addv[o];
            addv[rc] += addv[o];
            sumv[o] += addv[o] * (r - l + 1);
            addv[o] = 0;
        }
    }

    void up(int o, int l, int r) {
        // TODO: Overloaded
        xxx;
        sumv[o] = sumv[lc] + addv[lc] * (mid - l + 1) + sumv[rc] + addv[rc] * (r - mid);
    }

    void update(int o, int l, int r) {
        if (ql <= l && r <= qr) {
            // TODO: Overloaded
            addv[o] += qv;
            return;
        }
        xxx;
        if (ql <= mid) update(lson);
        if (qr > mid) update(rson);
        up(o, l, r);
    }

    void query(int o, int l, int r) {
        if (ql <= l && r <= qr) {
            q_sum += sumv[o] + addv[o] * (r - l + 1);
            return;
        }
        xxx;
        down(o, l, r);
        if (ql <= mid) query(lson);
        if (qr > mid) query(rson);
    }

public:
    void Build(int n) {
        this->n = n;
        // TODO: Overloaded
        sumv = vector<T>(2 * n - 1);
        addv = vector<T>(2 * n - 1);
        init(1, 1, n);
    }

    void Add(int l, int r, T v) {
        ql = l;
        qr = r;
        qv = v;
        if (l > r) return;
        update(1, 1, n);
    }

    T Query(int l, int r) {
        ql = l;
        qr = r;
        // TODO: Overloaded
        q_sum = 0;
        if (l > r)
            return q_sum;
        query(1, 1, n);
        return q_sum;
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
    int n;
    long long k, ans;
    vector<int> a, d;
    vector<vector<int>>  G;
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
            a.resize(n);
            d.resize(n);
            G.resize(n);
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
    vector<int> a;
    Splay<int> splay;

public:
    void Solve() {
        int n, m;
        cin >> n >> m;
        a.resize(n + 1);
        for (int i = 0; i <= n; i++) a[i] = i;
        // 需要新建一个虚拟节点，方便操作，无需再考虑是否存在NULL
        splay.Build(a.data(), n + 1);
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
