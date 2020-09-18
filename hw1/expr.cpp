#include <iostream>
#include <cstdio>
#include <iomanip>
#include <cmath>
using namespace std;

const char QUIT = ';';
const char NUMBER = 'n';
const double eps = 1e-6;

struct Token
{
    char kind;    // what kind of token
    double value; // for numbers: a value
    // make a Token from a char
    Token(char ch) : kind(ch), value(0) {}
    // make a Token from a char and a double
    Token(char ch, double val) : kind(ch), value(val) {}
};

class Token_stream
{
private:
    bool full;    // is there a Token in the buffer?
    Token buffer; // here is where we keep a Token put back using putback()

public:
    Token_stream() : full(false), buffer(0) {} // make a Token_stream that reads from cin
    Token get()                                // get a Token
    {
        {
            if (full)
            { // do we already have a Token ready?
                // remove token from buffer
                full = false;
                return buffer;
            }
            char ch;
            cin >> ch;
            if (ch == '.' || isdigit(ch))
            {
                cin.putback(ch); // put digit back into the input stream
                double val;
                cin >> val;                // read a floating-point number
                return Token(NUMBER, val); // let 'n' represent "a number"
            }
            switch (ch)
            {
            case ';': // for "print"
            case '(':
            case ')':
            case '+':
            case '-':
            case '*':
            case '/':
                return Token(ch); // let each character represent itself
            default:
                throw invalid_argument("Bad token");
            }
        }
    }
    void putback(Token t) // put a Token back
    {
        {
            if (full)
                throw invalid_argument("putback() into a full buffer");
            buffer = t;  // copy t to buffer
            full = true; // buffer is now full
        }
    }
};

Token_stream ts; // provides get() and putback()
double expression();
double term();
double primary();

// Start here!
double expression()
{
    double ret = term();
    while (true)
    {
        Token opt = ts.get();
        switch (opt.kind)
        {
        case '+':
            ret += term();
            break;
        case '-':
            ret -= term();
            break;
        default:
            ts.putback(opt.kind);
            return ret;
        }
    }
}

double term()
{
    double ret = primary();
    while (true)
    {
        Token opt = ts.get();
        switch (opt.kind)
        {
        case '*':
            ret *= primary();
            break;
        case '/':
        {
            double val = primary();
            if (fabs(val) < eps)
                throw invalid_argument("divide by zero");
            ret /= val;
            break;
        }
        default:
            ts.putback(opt.kind);
            return ret;
        }
    }
}

double primary()
{
    Token opt = ts.get();
    switch (opt.kind)
    {
    case '(':
    {
        double ret = expression();
        if (ts.get().kind != ')')
            throw invalid_argument("miss )");
        return ret;
    }
    case '+':
    case NUMBER:
        return opt.value;
    case '-':
        return -ts.get().value;
    default:
        throw invalid_argument("invalid primary");
    }
}

// main function
int main()
{
    // debug
    // freopen("in", "r", stdin);

    double val = 0;
    while (cin)
    {
        Token t = ts.get();
        if (t.kind == QUIT)
        {
            cout << fixed;
            cout << setprecision(3) << val << '\n';
            break;
        }
        else
            ts.putback(t);
        try
        {
            val = expression();
        }
        catch (const std::exception &e)
        {
            cout << e.what() << '\n';
            return 0;
        }
    }
}
