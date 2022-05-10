#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

// You can add additional standard libraries if necessary.
// Implement the Lagrange interpolation!
class Lagrange
{
public:
    Lagrange(const vector<double> &x,
             const vector<double> &y) : X(x), Y(y) {}
    double calc(double xn)
    {
        double ret = 0;
        for (unsigned k = 0; k < Y.size(); ++k)
        {
            double val = Y[k];
            for (unsigned i = 0; i < X.size(); ++i)
                if (i != k)
                    val = val * (xn - X[i]) / (X[k] - X[i]);
            ret += val;
        }
        return ret;
    }

private:
    vector<double> X, Y;
};

// Test your implementation.
int main()
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
    Lagrange F(X, Y);
    // interp_x is the point to be interpolated.
    double interp_x;
    cin >> interp_x;

    // Do Lagrange interpolation for interp_x using X and Y, and print your results
    // Note: The result retains three decimal places (rounded)!
    cout << fixed << setprecision(3)
         << F.calc(interp_x)
         << endl;
    // End
    return 0;
}
