#ifndef TREE_H
#define TREE_H

// Add any standard library you need.
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <random>
using namespace std;

//
// Below is the class template provided by us.
// Feel free to reconstruct the code as long as its functions are realized.
//

// ==================== k-d tree ======================
struct kdNode
{
    int key[2];
    vector<int> x;
    // ls: <= and rs: >=
    shared_ptr<kdNode> ls, rs;
    kdNode() : ls(nullptr), rs(nullptr) {}
    kdNode(vector<int> _x)
        : ls(nullptr), rs(nullptr)
    {
        x = move(_x);
        key[0] = x[0], key[1] = x[1];
    }
};
class kdTree
{
private:
    int dim_num;
    // return true if a==b
    bool equal(const vector<int> &a, const vector<int> &b)
    {
        for (int i = 0; i < dim_num; ++i)
            if (a[i] != b[i])
                return 0;
        return 1;
    }
    // return true if a<=b
    bool less(const vector<int> &a, const vector<int> &b)
    {
        for (int i = 0; i < dim_num; ++i)
            if (a[i] > b[i])
                return 0;
        return 1;
    }

private:
    // return min with f=0
    //      or max with f=1
    shared_ptr<kdNode> check(shared_ptr<kdNode> a,
                             shared_ptr<kdNode> b,
                             int f, int dim)
    {
        if (a == nullptr || b == nullptr)
            return a == nullptr ? b : a;
        return (a->x[dim] < b->x[dim]) ^ f ? a : b;
    }
    // find min/max node along dim_cmp dimension
    //      find min with f=0, while find max with f=1
    shared_ptr<kdNode> find(shared_ptr<kdNode> o, bool f, int dim_cmp, int dim)
    {
        if (o == nullptr)
            return nullptr;
        shared_ptr<kdNode> ret1 = nullptr, ret2 = nullptr;
        ret1 = find(f ? o->rs : o->ls, f, dim_cmp, (dim + 1) % dim_num);
        if (dim != dim_cmp)
            ret2 = find(f ? o->ls : o->rs, f, dim_cmp, (dim + 1) % dim_num);
        return check(o, check(ret1, ret2, f, dim_cmp), f, dim_cmp);
    }
    // insert node
    void insert(shared_ptr<kdNode> &o,
                const vector<int> &x, int dim)
    {
        if (o == nullptr)
        {
            o = make_shared<kdNode>(kdNode(x));
            return;
        }
        if (x[dim] <= o->x[dim])
            insert(o->ls, x, (dim + 1) % dim_num);
        else
            insert(o->rs, x, (dim + 1) % dim_num);
    }
    // erase node
    void erase(shared_ptr<kdNode> &o,
               const vector<int> &x, int dim)
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
            // randomly choose ls or rs
            bool f = Rand(gen);
            shared_ptr<kdNode> nxt = find(f ? o->ls : o->rs, f, dim, (dim + 1) % dim_num);
            if (nxt == nullptr)
                f ^= 1, nxt = find(f ? o->ls : o->rs, f, dim, (dim + 1) % dim_num);
            o->x = nxt->x;
            erase(f ? o->ls : o->rs, nxt->x, (dim + 1) % dim_num);
            return;
        }
        if (x[dim] <= o->x[dim])
            erase(o->ls, x, (dim + 1) % dim_num);
        if (x[dim] > o->x[dim])
            erase(o->rs, x, (dim + 1) % dim_num);
    }
    void range_search(shared_ptr<kdNode> o, int dim,
                      const vector<int> &low, const vector<int> &high,
                      vector<shared_ptr<kdNode>> &t)
    {
        if (o == nullptr)
            return;
        if (less(low, o->x) && less(o->x, high))
            t.push_back(o);
        if (low[dim] <= o->x[dim]) //ls <=
            range_search(o->ls, (dim + 1) % dim_num, low, high, t);
        if (o->x[dim] < high[dim]) //rs >
            range_search(o->rs, (dim + 1) % dim_num, low, high, t);
    }

public:
    shared_ptr<kdNode> root;
    kdTree();

    // Insert a new node with 2 dimensional value `arr`.
    // `dim` refers to the dimension of the root.
    void insert(shared_ptr<kdNode> rt, int arr[], int dim);

    // Search node with value `arr`.
    // Return NULL if not exist.
    kdNode *search(shared_ptr<kdNode> rt, int arr[], int dim);

    // Delete the node with value `arr`.
    void remove(shared_ptr<kdNode> rt, int arr[], int dim);

    // Find the node with the minimum value in dimension `dimCmp`.
    // `dimCmp` equals to 0 or 1.
    kdNode *findMin(shared_ptr<kdNode> rt, int dimCmp, int dim);

    // Find a vector of nodes whose values are >= `lower_bound` and <= `upper_bound`.
    // `lower_bound` contains lower bound for each dimension of the target nodes.
    // `upper_bound`, on the contrary, holds upper bound per dimension.
    // e.g. given lower_bound=[1, 2], upper_bound=[3, 4], node=[2, 3] will be chosen, because 1<=2<=3, 2<=3<=4.
    vector<struct kdNode *> rangeSearch(shared_ptr<kdNode> rt, int dim, int lower_bound[], int upper_bound[]);
};

/*
// Your class will be instantiated and called as such:
const int k = 2;
kdTree* obj = new kdTree();
int points[][k] = {{30, 40}, {5, 25}, {70, 70},
                {10, 12}, {50, 30}, {35, 45}};
obj->insert(obj->root, points[0], 0);
obj->insert(obj->root, points[1], 0);
kdNode* res1 = obj->search(obj->root, points[2], 0); // and then test res1;
obj->remove(obj->root, points[1], 0);
kdNode* res2 = obj->findmin(obj->root, 1, 0); // and then test res2;
int bounds[][k] = {{0, 10}, {50, 35}};
vector<kdNode*> res3 = obj->rangesearch(obj->root, 0, bounds[0], bounds[1]); // and then test res3.
*/

// ==================== trie (prefix tree) ======================
struct TrieNode
{
    int cnt;
    // a-z
    shared_ptr<TrieNode> ch[26];
    TrieNode() : cnt(0) {}
};
class Trie
{
private:
    shared_ptr<TrieNode> root;

public:
    Trie();

    // Inserts a word into the trie.
    void insert(string word);

    // Returns true if the word is in the trie.
    bool search(string word);

    // Returns true if there is any word in the trie that starts with the given prefix.
    bool startsWith(string prefix);
};

/*
// Your Trie object will be instantiated and called as such:
Trie* obj = new Trie();
string word = "apple";
string prefix = "app";
obj->insert(word);
bool res1 = obj->search(word);
bool res2 = obj->startsWith(prefix);
*/

#endif
