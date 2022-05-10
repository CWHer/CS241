#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

// You can add additional standard libraries if necessary.
// Implement the Newton interpolation!
class Newton
{
public:
    Newton(const vector<double> &x, const vector<double> &y) : X(x), Y(y)
    {
        // assume equal delta_h
        double h = x.size() == 1 ? 0 : x[1] - x[0];
        a.resize(x.size());
        for (unsigned k = 0; k < a.size(); ++k)
        {
            a[k] = Y[k];
            for (unsigned i = Y.size() - 1; i > k; --i)
                Y[i] = (Y[i] - Y[i - 1]) / (k + 1) / h;
        }
    }
    double calc(double xn)
    {
        double ret = 0;
        for (unsigned i = 0; i < a.size(); ++i)
        {
            double val = a[i];
            for (unsigned k = 0; k < i; ++k)
                val *= xn - X[k];
            ret += val;
        }
        return ret;
    }

private:
    vector<double> X, Y;
    vector<double> a;
};

// Test your implementation.
int main(int argc, const char *argv[])
{
    // freopen("in", "r", stdin);
    //  Input preprocessing.
    string str;
    getline(cin, str);
    stringstream xstr(str);
    getline(cin, str);
    stringstream ystr(str);

    // X and Y are two vectors of equal length to be traversed.
    vector<double> X, Y;
    double a;
    while (xstr >> a)
        X.push_back(a);
    while (ystr >> a)
        Y.push_back(a);

    Newton F(X, Y);
    // interp_x is the point to be interpolated.
    double interp_x;
    cin >> interp_x;

    // Do Newton interpolation for interp_x using X and Y, and print your results
    // Note: The result retains three decimal places (rounded)!
    cout << fixed << setprecision(3)
         << F.calc(interp_x)
         << endl;
    // End
    return 0;
}
