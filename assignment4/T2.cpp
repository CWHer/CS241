#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

vector<int> w;
template <class T>
void n_element(vector<T> &w, int l, int r, int K)
{
    int i = l, j = r;
    T mid = w[(l + r) >> 1];
    do
    {
        while (w[i] > mid)
            i++;
        while (w[j] < mid)
            j--;
        if (i <= j)
            swap(w[i++], w[j--]);
    } while (i <= j);
    if (K <= j && l < j)
        n_element(w, l, j, K);
    if (K >= i && i < r)
        n_element(w, i, r, K);
}
int main()
{
    // freopen("in", "r", stdin);
    ios::sync_with_stdio(0);
    string s;
    getline(cin, s);
    stringstream in(s);
    int x;
    while (in >> x)
        w.push_back(move(x));
    while (cin >> x)
    {
        n_element(w, 0, w.size() - 1, x - 1);
        cout << w[x - 1] << ' ';
    }
    cout << endl;
    return 0;
}