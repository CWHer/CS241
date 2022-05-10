// a naive version without optimizing
// can't handle signed number like 2*-2

// todo: apply expr.cpp's method in this code

// current version: only handle +,-,*,/,()

// a trivial parser to handle naive arthemetic expr
//      which only includes +,-,*,/,()
#include <cstdio>
#include <stack>
#include <vector>
#include <memory>
#include <cstring>
#include <string>
#include <cmath>
#include <iostream>
using std::cin, std::cout, std::endl;
using std::dynamic_pointer_cast;
using std::shared_ptr, std::make_shared;
using std::strlen;

const int N = 1e3;
const double eps = 1e-20;
char str[N];
enum OPT
{
    EMPTY,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    NUMBER,
    BRACKET
};
class Expr_Compiler;
class Expression;
class Term;
class Primary;

// base class
class Item
{

    friend class Expr_Compiler;

protected:
    shared_ptr<Item> lhs, rhs;
    OPT opt;
    double val;

public:
    Item() : lhs(nullptr), rhs(nullptr),
             opt(EMPTY), val(0) {}
};

double func(const OPT &opt, const double &lhs, const double &rhs)
{
    switch (opt)
    {
    case EMPTY:
    case BRACKET:
        return lhs;
    case PLUS:
        return lhs + rhs;
    case MINUS:
        return lhs - rhs;
    case TIMES:
        return lhs * rhs;
    case DIVIDE:
        if (fabs(rhs) < eps)
            throw std::overflow_error("divide by zero");
        return lhs / rhs;
    }
}

class Expr_Compiler
{

private:
    int len;
    char str[N];
    shared_ptr<Item> ptr;

    double calc(shared_ptr<Item> o)
    {
        if (o->opt == NUMBER)
            return o->val;
        double lhs, rhs;
        if (o->lhs != nullptr)
            lhs = calc(o->lhs);
        if (o->rhs != nullptr)
            rhs = calc(o->rhs);
        return o->val = func(o->opt, lhs, rhs);
    }

public:
    Expr_Compiler() : len(0), ptr(nullptr) {}

    double calc()
    {
        return calc(ptr);
    }

    void read()
    {
        cin.getline(str, N - 1);
        int ori_len = strlen(str);
        // remove space
        for (int i = 0; i < ori_len; ++i)
            if (str[i] != ' ')
                str[len++] = str[i];
    }

    void compile()
    {
        ptr = dynamic_pointer_cast<Item>(
            make_shared<Expression>(str, 0, len - 1));
    }
};

// + or -
class Expression : public Item
{
public:
    Expression(const char str[], int l, int r)
    {
        // bracket counter
        auto cnt = 0;
        for (int i = r; i >= l; --i)
        {
            cnt += str[i] == '(';
            cnt -= str[i] == ')';
            if (cnt == 0 && (str[i] == '+' || str[i] == '-'))
            {
                lhs = dynamic_pointer_cast<Item>(
                    make_shared<Expression>(str, l, i - 1));
                rhs = dynamic_pointer_cast<Item>(
                    make_shared<Term>(str, i + 1, r));
                opt = str[i] == '+' ? PLUS : MINUS;
                break;
            }
        }

        if (lhs == nullptr)
            lhs = dynamic_pointer_cast<Item>(
                make_shared<Term>(str, l, r));
    }
};

// * or /
class Term : public Item
{
public:
    Term(const char str[], int l, int r)
    {
        // bracket counter
        auto cnt = 0;
        for (int i = r; i >= l; --i)
        {
            cnt += str[i] == '(';
            cnt -= str[i] == ')';
            if (cnt == 0 && (str[i] == '*' || str[i] == '/'))
            {
                lhs = dynamic_pointer_cast<Item>(
                    make_shared<Term>(str, l, i - 1));
                rhs = dynamic_pointer_cast<Item>(
                    make_shared<Primary>(str, i + 1, r));
                opt = str[i] == '*' ? TIMES : DIVIDE;
                break;
            }
        }

        if (lhs == nullptr)
            lhs = dynamic_pointer_cast<Item>(
                make_shared<Primary>(str, l, r));
    }
};

// single number or (...)
class Primary : public Item
{
public:
    Primary(const char str[], int l, int r)
    {
        opt = str[l] == '(' ? BRACKET : NUMBER;
        if (opt == BRACKET)
        {
            lhs = dynamic_pointer_cast<Item>(
                make_shared<Expression>(str, l + 1, r - 1));
            return;
        }
        if (opt == NUMBER)
        {
            bool flag = 0;
            double base = 0.1;
            val = 0;
            for (int i = l; i <= r; ++i)
            {
                if (str[i] == '.')
                {
                    flag = 1;
                    continue;
                }
                if (flag)
                {
                    val += base * (str[i] - '0');
                    base /= 10;
                }
                else
                    val = val * 10 + str[i] - '0';
            }
            return;
        }
    }
};

Expr_Compiler c;
int main()
{
    freopen("in", "r", stdin);
    c.read();
    try
    {
        c.compile();
        cout << c.calc() << endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}