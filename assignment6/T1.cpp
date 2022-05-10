#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

struct edge
{
    int u, v, val;
};
vector<edge> e;
vector<int> f, w;

int find(int x) { return f[x] == x ? x : f[x] = find(f[x]); }

int main()
{
    // freopen("in", "r", stdin);
    ios::sync_with_stdio(0);
    string str;
    getline(cin, str);
    stringstream s_in(str);
    int x;
    while (s_in >> x)
        w.emplace_back(move(x));
    int n = w.size();
    f.resize(w.size() + 1);
    for (int i = 1; i <= n; ++i)
    {
        f[i] = i;
        for (int j = i; j < n; ++j) if (w[j])
            e.emplace_back(move((edge){i, j + 1, w[j]}));
        getline(cin, str);
        s_in.clear(), s_in.str(str);
        for (int j = 0; j < n; ++j)
            s_in >> w[j];
    }
    sort(e.begin(), e.end(),
         [](const edge &a, const edge &b) { return a.val < b.val; });
    int ans = 0;
    for (const auto &ei : e)
    {
        auto u = find(ei.u);
        auto v = find(ei.v);
        if (u == v)
            continue;
        ans += ei.val;
        f[u] = v;
    }
    cout << ans << endl;
    return 0;
}