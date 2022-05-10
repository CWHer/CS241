#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cstring>
#include <algorithm>
using namespace std;

const int N = 5e3;
int dp[N][N];
void dfs(const string &X, const string &Y,
         int i, int j, string now, set<string> &ans)
{
    if (dp[i][j] == 0)
    {
        ans.insert(now);
        return;
    }
    if (dp[i - 1][j] == dp[i][j])
        dfs(X, Y, i - 1, j, now, ans);
    if (dp[i][j - 1] == dp[i][j])
        dfs(X, Y, i, j - 1, now, ans);
    if (X[i - 1] == Y[j - 1] && dp[i - 1][j - 1] + 1 == dp[i][j])
    {
        now = X[i - 1] + now;
        dfs(X, Y, i - 1, j - 1, now, ans);
    }
}
set<string> LCS(const string &X, const string &Y)
{
    int n = X.size(), m = Y.size();
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
        {
            dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            dp[i][j] = max(dp[i][j], dp[i - 1][j - 1] + (X[i - 1] == Y[j - 1]));
        }
    set<string> ret;
    dfs(X, Y, n, m, "", ret);
    return ret;
}

int main()
{
    // debug
    // freopen("in", "r", stdin);

    string X, Y;
    cin >> X;
    cin >> Y;

    set<string> ans = LCS(X, Y);

    // print set elements
    for (string str : ans)
        cout << str << endl;
    return 0;
}