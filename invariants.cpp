#include "algorithms.h"
#include <set>
#include <numeric>

std::multiset<int> degrees(const Graph& g)
{
    std::multiset<int> res;
    for (int i = 0; i < g.size(); i++)
        res.insert(std::accumulate(g[i].begin(), g[i].end(), 1));
    return res;
}

std::multiset<std::multiset<int>> dists(const Graph& g)
{
    int n = g.size();
    std::vector<std::vector<int>> d(n, std::vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i == j)
                d[i][j] = 0;
            else if (g[i][j])
                d[i][j] = 1;
            else
                d[i][j] = n;
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
    std::multiset<std::multiset<int>> res;
    for (auto row : d)
        res.insert(std::multiset<int>(row.begin(), row.end()));
    return res;
}

bool invariants(const Graph& g1, const Graph& g2)
{
    if (g1.size() != g2.size())
        return false;
    if (degrees(g1) != degrees(g2))
        return false;
    if (dists(g1) != dists(g2))
        return false;
    return true;
}
