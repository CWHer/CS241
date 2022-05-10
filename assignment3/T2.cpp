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
const int MAX_ITERATIONS = 1e6;

// calc rank(A) and check root condition
int Rank(vector<vector<double>> w)
{
    int R = 0;
    int r = w.size(), c = w[0].size();
    for (int i = 0; i < c; ++i) //i th column
    {
        int t = R;
        for (int j = R + 1; j < r; ++j)
            if (fabs(w[j][i]) > fabs(w[t][i]))
                t = j;
        if (fabs(w[t][i]) < eps)
            continue;
        if (t != R)
            swap(w[t], w[R]);
        for (int j = R + 1; j < r; ++j)
        {
            double t = w[j][i] / w[i][i];
            for (int k = i; k < c; ++k)
                w[j][k] -= w[i][k] * t;
        }
        if (++R >= min(r, c))
            break;
    }
    return R;
}
bool Jacobi_solver(vector<vector<double>> w)
{
    int n = w.size(), R_Ab = Rank(w);
    vector<double> b(n), x(n), xk(n);
    for (unsigned i = 0; i < w.size(); ++i)
    {
        b[i] = move(w[i].back());
        w[i].pop_back();
    }
    if (R_Ab > Rank(w))
        throw invalid_argument("No solution!");
    if (R_Ab < n)
        throw invalid_argument("No unique solution!");
    for (int T = 1, flag = 1; T <= MAX_ITERATIONS; ++T, flag = 1)
    {
        for (int i = 0; i < n; ++i)
        {
            xk[i] = b[i];
            for (int j = 0; j < n; ++j)
                if (i != j)
                    xk[i] -= w[i][j] * x[j];
            xk[i] /= w[i][i];
        }
        // converge
        for (int i = 0; i < n; ++i)
            if (fabs(xk[i] - x[i]) > eps)
                flag = 0;
        if (flag)
        {
            for (int i = 0; i < n; ++i)
            {
                double val = 0;
                for (int j = 0; j < n; ++j)
                    val += w[i][j] * x[j];
                if (fabs(val - b[i]) > eps)
                    return 0;
            }
            for (const auto &xi : x)
                cout << fixed << setprecision(3) << xi << ' ';
            return 1;
        }
        x = xk;
    }
    // not convergence
    return 0;
}
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
        if (!Jacobi_solver(w))
            Gaussian_solver(w);
    }
    catch (exception &e)
    {
        cout << e.what();
    }
    cout << endl;
    return 0;
}
