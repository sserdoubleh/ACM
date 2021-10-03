#include <bits/stdc++.h>
using namespace std;

#define debug(...) cerr << "#LINE-" << __LINE__ << ' ' << #__VA_ARGS__ << " - ", _do(__VA_ARGS__)
template<typename T> void _do(vector<T> x) {for(auto i: x) cerr << i << ' '; cerr << "\n";}
template<typename T> void _do(set<T> x) {for(auto i: x) cerr << i << ' '; cerr << "\n";}
template<typename T> void _do(unordered_set<T> x) {for(auto i: x) cerr << i << ' '; cerr << "\n";}
template<typename T> void _do(T && x) {cerr << x << endl;}
template<typename T, typename ...S> void _do(T && x, S&&...y) {cerr << x << ", "; _do(y...);}

template<typename T> void print(T && x) {cout << x << "\n";}
template<typename T, typename... S> void print(T && x, S&&... y) {cout << x << ' '; print(y...);}

void solve() {
    // TODO
}

int main() {
    cin.tie(0), cout.tie(0);
    ios::sync_with_stdio(false);
#ifdef DEBUG
    auto begin = std::chrono::steady_clock::now();
#endif
    int T;
    cin >> T;
    while (T--)
        solve();
#ifdef DEBUG
    auto end = std::chrono::steady_clock::now();
    cout << std::chrono::steady_clock::duration(end - begin).count() / 1000. / 1000. << endl;
#endif
}
