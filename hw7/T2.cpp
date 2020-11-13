#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

class Genetic_Algorithm
{
private:
    // iteration counter
    int T;
    unsigned bit_num;
    vector<unsigned> w;
    // death threshold
    static constexpr double threshold = 0.55;
    // among all generation
    double max_fit_val, max_fit;
    // each generation
    double cur_fit, avg_fit;
    double cur_fit_val;

    // return double in [0,1]
    double random_unit_double()
    {
        static mt19937 gen(time(0));
        static uniform_real_distribution<double> distribution(0, 1);
        return distribution(gen);
    }
    unsigned random_bits()
    {
        static mt19937 gen(time(0));
        static uniform_int_distribution<int> distribution(0, (1 << bit_num) - 1);
        return distribution(gen);
    }
    unsigned random_int(int min, int max)
    {
        static mt19937 gen(time(0));
        uniform_int_distribution<int> distribution(min, max);
        return distribution(gen);
    }
    // bit_num=16 bit unsigned to double
    //      which ranges in [-1,15]
    double u2d(unsigned x)
    {
        return (double)x / (1 << 12) - 1;
    }
    double F(double x)
    {
        return -x * sin(x);
    }
    // sigmoid
    //      R --> [0,1]
    double sig(double x)
    {
        return 1 / (1 + exp(-x));
    }

    void selection()
    {
        sort(w.begin(), w.end(),
             [this](const unsigned &a,
                    const unsigned &b) { return this->fitness(a) > this->fitness(b); });
        for (unsigned i = 1; i < w.size(); ++i)
            if (sig(fitness(w[i]) / max_fit) < threshold)
                w[i] = random_unit_double() < 0.5 ? max_fit_val : w[random_int(0, i - 1)];
    }

    unsigned cross(const unsigned &x,
                   const unsigned &y)
    {
        // pdf
        double px = sig(fitness(x));
        px = px / (px + sig(fitness(y)));
        unsigned ret = 0;
        for (unsigned k = 0; k < bit_num; ++k)
        {
            // auto p = random_unit_double();
            ret |= ((random_unit_double() < px ? x : y) >> k & 1) << k;
        }

        return ret;
    }

    void mutation(unsigned &x)
    {
        for (unsigned i = 0; i < bit_num; ++i)
            if (random_unit_double() < 1.0 / ((1 << i / 2) + 5))
                x ^= 1 << i;
    }

public:
    Genetic_Algorithm(unsigned num)
        : T(0), bit_num(num) {}

    double fitness(unsigned x)
    {
        return F(u2d(x));
    }
    void init(int num) // num must be even in this
    {
        w.resize(num);
        cur_fit = 0;
        for (int i = 0; i < num; ++i)
        {
            w[i] = random_bits();
            // w[i] = ((1 << bit_num) - 1) / num * (i + 1);
            auto f = fitness(w[i]);
            if (f > cur_fit)
            {
                max_fit = cur_fit = f;
                max_fit_val = cur_fit_val = w[i];
            }
        }
    }
    void next()
    {
        T++;
        selection();
        // shuffle(w.begin(), w.end(), gen);
        sort(w.begin(), w.end());
        // crossover
        for (unsigned i = 0; i < w.size(); i += 2)
        {
            unsigned s1 = cross(w[i], w[i ^ 1]);
            unsigned s2 = cross(w[i], w[i ^ 1]);
            mutation(s1), mutation(s2);
            w[i] = s1, w[i ^ 1] = s2;
        }
        // evaluate
        avg_fit = cur_fit = 0;
        for (auto &wi : w)
        {
            auto f = fitness(wi);
            avg_fit += sig(f / max_fit);
            if (f > cur_fit)
            {
                cur_fit = f;
                cur_fit_val = wi;
                if (f > max_fit)
                    max_fit = f, max_fit_val = wi;
            }
        }
        avg_fit /= w.size();
    }
    void print()
    {
        cout << T << " generation" << '\n';
        cout << "average fitness:" << avg_fit << '\n';
        cout << "cur fitness:" << cur_fit << '\n'
             << "best fitness val:" << u2d(cur_fit_val) << '\n';
        cout << endl;
    }
    void print_result()
    {
        cout << "result of all generations" << '\n';
        cout << "max fitness:" << max_fit << '\n'
             << "max fitness val:" << u2d(max_fit_val) << '\n';
    }
    // debug
    void print_all()
    {
        cout << "print all data" << '\n';
        for (const auto &wi : w)
            cout << "val:" << u2d(wi) << '\n'
                 << "fitness:" << sig(fitness(wi) / max_fit) << endl;
    }
};

// 16 bits
Genetic_Algorithm solver(16);
int main()
{
    double start_time = clock();
    ios::sync_with_stdio(0);
    solver.init(40);
    int T = 1000;
    // int T = 50;
    for (int i = 1; i <= T; ++i)
    {
        solver.print();
        solver.next();
    }
    // solver.print();
    solver.print_result();
    // solver.print_all();

    double end_time = clock();
    cout << "time:"
         << (end_time - start_time) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
    return 0;
}
