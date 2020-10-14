#include <cstdio>
#include <complex>
#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>
using namespace std;
using Complex = complex<double>;
const double PI = acos(-1);
const int N = 2.5e6;

// calculate (A(w_0_n),A(w_1_n)...)
//      with f(x)=a0+a1*x+a2*x^2+...
// t=-1 indicates reverse transform
class naive_FFT
{
private:
    Complex buf[N], a[N];
    int n, t;
    void FFT(Complex *a, int n)
    {
        if (n == 1)
            return;
        int mid = n >> 1;
        // divide a into
        //      A0=a0+a2+a4+...
        //      A1=a1+a3+a5+...
        //  and calculate A with x=w_k_n/2
        for (int i = 0; i < mid; ++i)
        {
            buf[i] = a[i << 1];
            buf[i + mid] = a[i << 1 | 1];
        }
        memcpy(a, buf, n * sizeof(Complex));
        Complex *A0 = a, *A1 = a + mid;
        FFT(A0, mid), FFT(A1, mid);
        // calculate A with x=w_k_n
        for (int i = 0; i < mid; ++i)
        {
            Complex wi = Complex(cos(2 * PI / n * i),
                                 sin(2 * PI / n * i * t));
            buf[i] = A0[i] + wi * A1[i];
            buf[i + mid] = A0[i] - wi * A1[i];
        }
        memcpy(a, buf, n * sizeof(Complex));
    }

public:
    // copy a[0,n)
    void init(Complex *_a, int _n, int _t)
    {
        n = _n, t = _t;
        memcpy(a, _a, n * sizeof(Complex));
    }
    void run()
    {
        FFT(a, n);
    }
    void gather(Complex *_a)
    {
        memcpy(_a, a, n * sizeof(Complex));
    }
};

class FFT
{
private:
    vector<Complex> a;
    vector<int> r;
    // i will be placed in r[i] in the bottom of FFT process
    //      note that r[i] has a reversed bits of i
    int t;

public:
    void init(const vector<Complex> &_a, int _t)
    {
        a = _a, t = _t;
        r.resize(a.size());
        for (unsigned i = 0; i < r.size(); i++)
            r[i] = (r[i >> 1] >> 1) | ((i & 1) * r.size() / 2);
    }
    void run()
    {
        for (unsigned i = 0; i < a.size(); ++i)
            if (i < r[i])
                swap(a[i], a[r[i]]);
        for (unsigned n = 1; n < a.size(); n <<= 1)
        {
            Complex w_n(cos(PI / n), t * sin(PI / n));
            for (unsigned i = 0; i < a.size(); i += n << 1)
            {
                Complex wk(1, 0);
                for (unsigned k = 0; k < n; ++k, wk *= w_n)
                {
                    Complex A0 = a[i + k],
                            A1 = a[i + k + n] * wk;
                    a[i + k] = A0 + A1;
                    a[i + k + n] = A0 - A1;
                }
            }
        }
    }
    vector<Complex> gather()
    {
        return a;
    }
} F;

vector<Complex> a, b;
int main()
{
    // freopen("in", "r", stdin);
    // freopen("out", "w", stdout);
    ios::sync_with_stdio(0);
    int n, m, max_size = 1;
    cin >> n >> m;
    while (max_size <= n + m)
        max_size <<= 1;
    a.resize(max_size), b.resize(max_size);
    for (int i = 0; i <= n; ++i)
        cin >> a[i];
    for (int i = 0; i <= m; ++i)
        cin >> b[i];
    F.init(a, 1);
    F.run(), a = move(F.gather());
    F.init(b, 1);
    F.run(), b = move(F.gather());
    for (int i = 0; i < max_size; ++i)
        a[i] *= b[i];
    F.init(a, -1);
    F.run(), a = move(F.gather());
    for (int i = 0; i <= n + m; ++i)
        cout << int(a[i].real() / max_size + .5) << ' ';
    return 0;
}