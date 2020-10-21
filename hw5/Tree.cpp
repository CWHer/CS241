#include "Tree.h"

// ==================== k-d tree ======================
kdTree::kdTree()
{
    root = nullptr;
    dim_num = 2;
}

void kdTree::insert(shared_ptr<kdNode> rt, int arr[], int dim)
{
    vector<int> x;
    for (int i = 0; i < dim_num; ++i)
        x.push_back(arr[i]);
    insert(root, x, 0);
}

kdNode *kdTree::search(shared_ptr<kdNode> rt, int arr[], int dim)
{
    vector<kdNode *> t = move(rangeSearch(rt, 0, arr, arr));
    return t.empty() ? NULL : t.front();
}

void kdTree::remove(shared_ptr<kdNode> rt, int arr[], int dim)
{
    vector<int> x;
    for (int i = 0; i < dim_num; ++i)
        x.push_back(arr[i]);
    erase(root, x, 0);
}

kdNode *kdTree::findMin(shared_ptr<kdNode> rt, int dimCmp, int dim)
{
    return find(root, 0, dimCmp, 0).get();
}

vector<struct kdNode *> kdTree::rangeSearch(shared_ptr<kdNode> rt, int dim, int arr1[], int arr2[])
{
    vector<int> low, high;
    for (int i = 0; i < dim_num; ++i)
    {
        low.push_back(arr1[i]);
        high.push_back(arr2[i]);
    }
    vector<shared_ptr<kdNode>> t;
    range_search(root, 0, low, high, t);
    vector<kdNode *> ret;
    for (const auto &ti : t)
        ret.push_back(move(ti.get()));
    return ret;
}

// ==================== trie (prefix tree) ======================
Trie::Trie()
{
    root = make_shared<TrieNode>(TrieNode());
}

void Trie::insert(string s)
{
    shared_ptr<TrieNode> o = root;
    for (unsigned i = 0; i < s.size(); ++i)
    {
        if (o->ch[s[i] - 'a'] == nullptr)
            o->ch[s[i] - 'a'] = make_shared<TrieNode>(TrieNode());
        o = o->ch[s[i] - 'a'];
    }
    o->cnt++;
}

bool Trie::search(string s)
{
    shared_ptr<TrieNode> o = root;
    for (unsigned i = 0; i < s.size(); ++i)
    {
        if (o == nullptr)
            return 0;
        o = o->ch[s[i] - 'a'];
    }
    return o == nullptr ? 0 : o->cnt > 0;
}

bool Trie::startsWith(string s)
{
    shared_ptr<TrieNode> o = root;
    for (unsigned i = 0; i < s.size(); ++i)
    {
        if (o == nullptr)
            return 0;
        o = o->ch[s[i] - 'a'];
    }
    return o != nullptr;
}
