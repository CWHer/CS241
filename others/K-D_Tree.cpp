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
const double eps = 1e-8;

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
        vector<double> mn, mx;
        vector<double> x;
        // ls: <= and rs: >=
        shared_ptr<node> ls, rs;
        node() : ls(nullptr), rs(nullptr) {}
        node(vector<double> _x)
            : ls(nullptr), rs(nullptr)
        {
            x = move(_x);
            mn = mx = x;
        }
    };
    shared_ptr<node> root;

    int dim_num;
    double sqr(double x) { return x * x; }
    // return dist_squared
    double norm2_dist(const vector<double> &a,
                      const vector<double> &b)
    {
        double ret = 0;
        for (unsigned i = 0; i < a.size(); ++i)
            ret += sqr(a[i] - b[i]);
        return ret;
    }
    // probable dist
    double pro_dist(const vector<double> &x, shared_ptr<node> o)
    {
        if (o == nullptr)
            return numeric_limits<double>::infinity();
        double ret = 0;
        for (int i = 0; i < dim_num; ++i)
            if (!(o->mn[i] <= x[i] && x[i] <= o->mx[i]))
                ret += sqr(min(fabs(o->mn[i] - x[i]), fabs(o->mx[i] - x[i])));
        return ret;
    }
    // true if a==b
    bool equal(const vector<double> &a, const vector<double> &b)
    {
        for (int i = 0; i < dim_num; ++i)
            if (fabs(a[i] - b[i]) > eps)
                return 0;
        return 1;
    }
    // true if a<=b
    bool less(const vector<double> &a, const vector<double> &b)
    {
        for (int i = 0; i < dim_num; ++i)
            if (a[i] > b[i] + eps)
                return 0;
        return 1;
    }

private:
    // return min with f=0
    //      or max with f=1
    shared_ptr<node> check(shared_ptr<node> a,
                           shared_ptr<node> b,
                           int f, int dim)
    {
        if (a == nullptr || b == nullptr)
            return a == nullptr ? b : a;
        return (a->x[dim] < b->x[dim]) ^ f ? a : b;
    }
    // find min/max node along dim_cmp dimension
    //      find min with f=0, while find max with f=1
    shared_ptr<node> find(shared_ptr<node> o, bool f, int dim_cmp, int dim)
    {
        if (o == nullptr)
            return nullptr;
        shared_ptr<node> ret1 = nullptr, ret2 = nullptr;
        ret1 = find(f ? o->rs : o->ls, f, dim_cmp, (dim + 1) % dim_num);
        if (dim != dim_cmp)
            ret2 = find(f ? o->ls : o->rs, f, dim_cmp, (dim + 1) % dim_num);
        return check(o, check(ret1, ret2, f, dim_cmp), f, dim_cmp);
    }
    // build KD-Tree with array w
    void build(shared_ptr<node> &o,
               vector<vector<double>> &w,
               int l, int r, int dim)
    {
        if (l >= r)
            return;
        int mid = (l + r) >> 1;
        nth_element(w.begin() + l, w.begin() + mid, w.begin() + r,
                    [dim](const vector<double> &a,
                          const vector<double> &b) { return a[dim] < b[dim]; });
        // can ensure rs_val>mid !!!!
        //      only if all coordinates are unique
        o = make_shared<node>(node(w[mid]));
        build(o->ls, w, l, mid, (dim + 1) % dim_num);
        build(o->rs, w, mid + 1, r, (dim + 1) % dim_num);
        push_up(o, o->ls, o->rs);
    }
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
    // insert node
    void insert(shared_ptr<node> &o,
                const vector<double> &x, int dim)
    {
        if (o == nullptr)
        {
            o = make_shared<node>(node(x));
            return;
        }
        if (x[dim] <= o->x[dim])
            insert(o->ls, x, (dim + 1) % dim_num);
        if (x[dim] > o->x[dim])
            insert(o->rs, x, (dim + 1) % dim_num);
        push_up(o, o->ls, o->rs);
    }
    // erase node
    void erase(shared_ptr<node> &o,
               const vector<double> &x, int dim)
    {
        static mt19937 gen(20000905);
        static uniform_int_distribution<int> Rand(0, 1);
        if (o == nullptr)
            return;
        if (equal(o->x, x))
        {
            if (o->ls == nullptr && o->rs == nullptr)
            {
                o = nullptr;
                return;
            }
            // randomly erase ls or rs
            bool f = Rand(gen);
            shared_ptr<node> nxt = find(f ? o->ls : o->rs, f, dim, (dim + 1) % dim_num);
            if (nxt == nullptr)
                f ^= 1, nxt = find(f ? o->ls : o->rs, f, dim, (dim + 1) % dim_num);
            o->x = nxt->x;
            erase(f ? o->ls : o->rs, nxt->x, (dim + 1) % dim_num);
            push_up(o, o->ls, o->rs);
            return;
        }
        if (x[dim] <= o->x[dim])
            erase(o->ls, x, (dim + 1) % dim_num);
        if (x[dim] >= o->x[dim])
            erase(o->rs, x, (dim + 1) % dim_num);
        push_up(o, o->ls, o->rs);
    }
    // find closest node with x
    void closest_search(shared_ptr<node> o,
                        const vector<double> &x,
                        double &min_dist, vector<double> &t)
    {
        double now_dist = norm2_dist(x, o->x);
        if (now_dist < min_dist)
            min_dist = now_dist, t = o->x;
        double lval = pro_dist(x, o->ls);
        double rval = pro_dist(x, o->rs);
        // nullptr has infinity value
        //      which would stop recursive
        if (lval < rval)
        {
            if (lval < min_dist)
                closest_search(o->ls, x, min_dist, t);
            if (rval < min_dist)
                closest_search(o->rs, x, min_dist, t);
        }
        else
        {
            if (rval < min_dist)
                closest_search(o->rs, x, min_dist, t);
            if (lval < min_dist)
                closest_search(o->ls, x, min_dist, t);
        }
    }
    // search range between low and high
    void range_search(shared_ptr<node> o, int dim,
                      const vector<double> &low, const vector<double> &high,
                      vector<vector<double>> &t)
    {
        if (o == nullptr)
            return;
        if (less(low, o->x) && less(o->x, high))
            t.push_back(o->x);
        if (low[dim] <= o->x[dim])
            range_search(o->ls, (dim + 1) % dim_num, low, high, t);
        if (o->x[dim] <= high[dim])
            range_search(o->rs, (dim + 1) % dim_num, low, high, t);
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
    KD_Tree(int _dim_num) : dim_num(_dim_num) {}
    ~KD_Tree() { dfs_destruct(root); }
    // build KD-Tree with array w
    //      will modify w
    void build(vector<vector<double>> &w)
    {
        build(root, w, 0, w.size(), 0);
    }
    void insert(const vector<double> &x)
    {
        insert(root, x, 0);
    }
    void erase(const vector<double> &x)
    {
        erase(root, x, 0);
    }
    // return a vector of node between low and high
    vector<vector<double>> range_search(const vector<double> &low,
                                        const vector<double> &high)
    {
        vector<vector<double>> t;
        range_search(root, 0, low, high, t);
        return t;
    }
    // return a pair of (min_dist,min_dist_node)
    pair<double, vector<double>> closest_search(const vector<double> &x)
    {
        double min_dist = numeric_limits<double>::infinity();
        vector<double> t(dim_num);
        if (root != nullptr)
            closest_search(root, x, min_dist, t);
        return make_pair(sqrt(min_dist), t);
    }
} T(2);

vector<vector<double>> w;
int main()
{
    // freopen("in", "r", stdin);
    // freopen("out", "w", stdout);
    ios::sync_with_stdio(0);
    int n;
    cin >> n;
    w.resize(n);
    for (auto &x : w)
    {
        x.resize(2);
        for (auto &xi : x)
            cin >> xi;
    }
    // shuffle(w.begin(), w.end(), mt19937(20000905));
    double ans = numeric_limits<double>::infinity();
    for (const auto &wi : w)
    {
        ans = min(ans, T.closest_search(wi).first);
        T.insert(wi);
        // cout << ans << '\n';
    }
    // T.build(w);
    // for (int i = 0; i < w.size(); ++i)
    // {
    //     T.erase(w[i]);
    //     ans = min(ans, T.closest_search(w[i]).first);
    //     T.insert(w[i]);
    //     // cout << ans << '\n';
    // }
    cout << fixed << setprecision(4) << ans << endl;
    return 0;
}