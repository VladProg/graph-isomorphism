#include "algorithms.h"
#include <algorithm>

bool naive(const Graph& g1, const Graph& g2)
{
    if (g1.size() != g2.size())
        return false;
    int n = g1.size();
    std::vector<int> p(n);
    for (int i = 0; i < n; i++)
        p[i] = i;
    do
    {
        bool ok = true;
        for (int i = 0; i < n && ok; i++)
            for (int j = i + 1; j < n && ok; j++)
                ok &= g1[i][j] == g2[p[i]][p[j]];
        if (ok)
            return true;
    } while (std::next_permutation(p.begin(), p.end()));
    return false;
}
