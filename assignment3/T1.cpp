#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;

// Implement your Gaussian Elimination algorithm.
// You can add any standard library if needed.
const double eps = 1e-8;
vector<vector<double>> w;

void Gaussian_solver(vector<vector<double>> w)
{
    int n = w.size();
    for (int i = 0; i < n; ++i)
    {
        int t = i;
        for (int j = i + 1; j < n; ++j)
            if (fabs(w[j][i]) > fabs(w[t][i]))
                t = j;
        if (fabs(w[i][i]) < eps)
            continue;
        if (t != i)
            swap(w[t], w[i]);
        for (int j = i + 1; j < n; ++j)
        {
            double t = w[j][i] / w[i][i];
            for (int k = i; k <= n; ++k)
                w[j][k] -= w[i][k] * t;
        }
    }
    for (int i = n - 1; ~i; --i)
    {
        for (int j = i + 1; j < n; ++j)
            w[i][n] -= w[i][j] * w[j][n];
        if (fabs(w[i][i]) < eps)
            throw fabs(w[i][n]) < eps ? invalid_argument("No unique solution!")
                                      : invalid_argument("No solution!");
        w[i][n] /= w[i][i];
    }
    for (unsigned i = 0; i < w.size(); ++i)
        cout << fixed << setprecision(3) << w[i][w.size()] << ' ';
}

// Test your implementation.
int main()
{
    // freopen("in", "r", stdin);
    // Input processing.
    string str;
    while (getline(cin, str))
    {
        vector<double> line;
        double val;
        stringstream s(move(str));
        while (s >> val)
            line.push_back(move(val));
        w.push_back(move(line));
    }

    // Solve the linear system and print the results.
    try
    {
        Gaussian_solver(w);
    }
    catch (exception &e)
    {
        cout << e.what();
    }
    cout << endl;
    return 0;
}
