// an unsuccessful attempt of luogu's P3810
//      using k-d tree
#include <cstdio>
#include <vector>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <cmath>
#include <limits>
#include <random>
using namespace std;
const int MAX_DIM = 3;

// 1. can only handle unique sequence
// 2. build can't ensure rs>o
//      thus erase can't ensure finding nxt>o in right subtree
//      thus ls<= and rs>=, = can exists in both subtrees
class KD_Tree
{
private:
    struct node
    {
        // AABB
        vector<int> x;
        vector<int> mn, mx;
        int sz;
        // ls: <= and rs: >=
        shared_ptr<node> ls, rs;
        node() : sz(1), ls(nullptr), rs(nullptr) {}
        node(vector<int> _x)
            : ls(nullptr), rs(nullptr)
        {
            sz = 1;
            x = move(_x);
            mn = mx = x;
        }
    };
    shared_ptr<node> root;

    int dim_num;
    // recalculate o's AABB
    //      after modifying its subtrees
    void push_up(shared_ptr<node> o,
                 shared_ptr<node> ls, shared_ptr<node> rs)
    {
        for (int i = 0; i < dim_num; ++i)
            o->mn[i] = o->mx[i] = o->x[i];
        if (ls != nullptr)
            for (int i = 0; i < dim_num; ++i)
            {

                o->mn[i] = min(o->mn[i], ls->mn[i]);
                o->mx[i] = max(o->mx[i], ls->mx[i]);
            }
        if (rs != nullptr)
            for (int i = 0; i < dim_num; ++i)
            {

                o->mn[i] = min(o->mn[i], rs->mn[i]);
                o->mx[i] = max(o->mx[i], rs->mx[i]);
            }
    }
    // true if a<=b
    bool less(const vector<int> &a, const vector<int> &b)
    {
        for (int i = 0; i < dim_num; ++i)
            if (a[i] > b[i])
                return 0;
        return 1;
    }

private:
    // build KD-Tree with array w
    void build(shared_ptr<node> &o,
               vector<vector<int>> &w,
               int l, int r, int dim)
    {
        if (l >= r)
            return;
        int mid = (l + r) >> 1;
        nth_element(w.begin() + l, w.begin() + mid, w.begin() + r,
                    [dim](const vector<int> &a,
                          const vector<int> &b) { return a[dim] < b[dim]; });
        // can ensure rs_val>mid !!!!
        //      only if all coordinates are unique
        o = make_shared<node>(node(w[mid]));
        o->sz = r - l;
        build(o->ls, w, l, mid, (dim + 1) % dim_num);
        build(o->rs, w, mid + 1, r, (dim + 1) % dim_num);
        push_up(o, o->ls, o->rs);
    }
    // search range between low and high
    void range_search(shared_ptr<node> o, int dim,
                      const vector<int> low, const vector<int> high,
                      int &cnt)
    {
        if (o == nullptr)
            return;
        if (less(low, o->mn) && less(o->mx, high))
        {
            cnt += o->sz;
            return;
        }
        if (less(low, o->x) && less(o->x, high))
            cnt++;
        if (low[dim] <= o->x[dim])
            range_search(o->ls, (dim + 1) % dim_num, low, high, cnt);
        if (o->x[dim] <= high[dim])
            range_search(o->rs, (dim + 1) % dim_num, low, high, cnt);
    }
    // destructor
    void dfs_destruct(shared_ptr<node> &o)
    {
        if (o == nullptr)
            return;
        dfs_destruct(o->ls);
        dfs_destruct(o->rs);
        o = nullptr;
    }

public:
    KD_Tree(int num) : dim_num(num) {}
    ~KD_Tree() { dfs_destruct(root); }
    // build KD-Tree with array w
    //      will modify w
    void build(vector<vector<int>> &w)
    {
        build(root, w, 0, w.size(), 0);
    }
    // return a cnt of node between low and high
    int range_search(const vector<int> &low,
                     const vector<int> &high)
    {
        int cnt = 0;
        range_search(root, 0, low, high, cnt);
        return cnt;
    }
} Tree(MAX_DIM);

vector<vector<int>> w;
vector<int> ans;
int main()
{
    // freopen("in", "r", stdin);
    // freopen("out", "w", stdout);
    ios::sync_with_stdio(0);
    int n, K;
    cin >> n >> K;
    w.resize(n);
    for (auto &wi : w)
    {
        wi.resize(MAX_DIM);
        for (auto &xi : wi)
            cin >> xi;
    }
    Tree.build(w);
    ans.resize(n);
    vector<int> low(MAX_DIM, 0);
    for (int i = 0; i < n; ++i)
    {
        // cerr << i << '\r';
        vector<int> high = w[i];
        ans[Tree.range_search(low, high) - 1]++;
    }
    for (const auto &x : ans)
        cout << x << '\n';
    cout << endl;
    return 0;
}