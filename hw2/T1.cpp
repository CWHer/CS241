#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long LL;
const LL INF = 1e16;
const int p[] = {2, 3, 5};
class Solution
{
private:
    vector<LL> ans;
    void dfs(int k, LL x)
    {
        if (k == 3)
        {
            ans.push_back(x);
            return;
        }
        dfs(k + 1, x);
        while (INF / p[k] > x)
        {
            x *= p[k];
            dfs(k + 1, x);
        }
    }

public:
    Solution()
    {
        ans.clear();
        dfs(0, 1);
        sort(ans.begin(), ans.end());
        // ans.resize(unique(ans.begin(), ans.end()) - ans.begin());
        // cout << ans.size() << endl;
    }

    LL nthUglyNumber(int n)
    {
        return ans[n - 1];
    }
};

int main()
{
    Solution s;
    int n;
    cin >> n;
    cout << s.nthUglyNumber(n) << endl;
    return 0;
}