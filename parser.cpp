// a naive version without optimizing

// current version: only handle +,-,*,/

// a trivial parser to handle naive arthemetic expr
//      which only includes +,-,*,/,()
#include <cstdio>
#include <stack>
#include <vector>
#include <memory>
#include <cstring>
#include <string>
#include <iostream>
using std::cin, std::cout, std::endl;
using std::dynamic_pointer_cast;
using std::shared_ptr, std::make_shared;
using std::strlen;

const int N = 1e3;
char str[N];
enum OPT
{
    EMPTY,
    PLUS,
    MINUS,
    TIMES,
    DIVISION,
    NUMBER,
    BRACKET
};
// string includes l&r position in constructors below

// base class
class Item
{

    friend double calc(shared_ptr<Item>);

protected:
    shared_ptr<Item> lhs, rhs;
    OPT opt;
    double val;

public:
    Item()
    {
        opt = EMPTY;
        lhs = rhs = nullptr;
        val = 0;
    }
};
class Expression;
class Term;
class Primary;

double func(const OPT &opt, const double &lhs, const double &rhs)
{
    switch (opt)
    {
    case EMPTY:
        return lhs;
    case PLUS:
        return lhs + rhs;
    case MINUS:
        return lhs - rhs;
    case TIMES:
        return lhs * rhs;
    case DIVISION:
        return lhs / rhs;
    }
}
double calc(shared_ptr<Item> o)
{
    if (o->opt == NUMBER)
        return o->val;
    double lhs, rhs;
    if (o->lhs != nullptr)
        lhs = calc(o->lhs);
    if (o->rhs != nullptr)
        rhs = calc(o->rhs);
    return func(o->opt, lhs, rhs);
}

class Expr_Compiler
{
private:
    int len;
    char str[N];

public:
    shared_ptr<Item> ptr;

public:
    Expr_Compiler()
    {
        len = 0;
        ptr = nullptr;
    }

    void read()
    {
        cin >> str;
        len = strlen(str);
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
        for (int i = l; i <= r; ++i)
            if (str[i] == '+' || str[i] == '-')
            {
                lhs = dynamic_pointer_cast<Item>(
                    make_shared<Term>(str, l, i - 1));
                rhs = dynamic_pointer_cast<Item>(
                    make_shared<Expression>(str, i + 1, r));
                opt = str[i] == '+' ? PLUS : MINUS;
                break;
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
        for (int i = l; i <= r; ++i)
            if (str[i] == '*' || str[i] == '/')
            {
                lhs = dynamic_pointer_cast<Item>(
                    make_shared<Primary>(str, l, i - 1));
                rhs = dynamic_pointer_cast<Item>(
                    make_shared<Term>(str, i + 1, r));
                opt = str[i] == '*' ? TIMES : DIVISION;
                break;
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
        // if (opt == BRACKET)
        // {
        //     lhs = make_shared<Expression>(str, l + 1, r - 1);
        //     return;
        // }

        // number
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
    }
};

Expr_Compiler c;
int main()
{
    freopen("in", "r", stdin);
    c.read();
    c.compile();
    cout << calc(c.ptr) << endl;
    return 0;
}