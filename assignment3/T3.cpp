#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <algorithm>

using namespace std;

template <class T>
void qsort(vector<T> &w, int l, int r)
{
    int i = l, j = r;
    T mid = w[(l + r) / 2];
    do
    {
        while (w[i] > mid)
            ++i;
        while (w[j] < mid)
            --j;
        if (i <= j)
            swap(w[i++], w[j--]);
    } while (i < j);
    if (i < r)
        qsort(w, i, r);
    if (l < j)
        qsort(w, l, j);
}

class Solution
{
private:
    float Median(vector<int>::iterator ori, int size)
    {
        return size & 1 ? *(ori + size / 2)
                        : (*(ori + size / 2 - 1) + *(ori + size / 2)) / 2.0;
    }

public:
    float hMedian(vector<int> &citations)
    {
        qsort(citations, 0, citations.size() - 1);
        for (unsigned i = 0; i < citations.size(); ++i)
            if (citations[i] < i + 1)
                return Median(citations.begin(), i);
        return Median(citations.begin(), citations.size());
    }
};

int main(int argc, char **argv)
{
    // freopen("in", "r", stdin);

    std::string line;
    int number;
    std::vector<int> numbers;

    std::getline(std::cin, line);
    std::istringstream stream(line);
    while (stream >> number)
        numbers.push_back(number);

    Solution s;
    float res = s.hMedian(numbers);

    std::cout << std::fixed;
    std::cout << std::setprecision(1) << res << endl;

    return 0;
}
