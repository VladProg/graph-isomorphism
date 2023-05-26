#include "generate.h"
#include <algorithm>
#include <numeric>
#include <set>
#include <tuple>
#include <iostream>
#include <random>
#include <chrono>

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

Graph random_graph(int n, int m)
{
    if (m == -1)
        m = rng() % (n * (n - 1) / 2 + 1);
    std::vector<std::pair<int, int>> edges;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            edges.push_back({ i,j });
    std::shuffle(edges.begin(), edges.end(), rng);
    edges.resize(m);
    Graph g(n);
    for (auto [i, j] : edges)
        g.add_edge(i, j);
    return g;
}

Graph random_isomorphic(const Graph& g1)
{
    int n = g1.size();
    Graph g2(n);
    std::vector<int> p(n);
    for (int i = 0; i < n; i++)
        p[i] = i;
    std::shuffle(p.begin(), p.end(), rng);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (g1[p[i]][p[j]])
                g2.add_edge(i, j);
    return g2;
}

Graph random_same_nm(const Graph& g1)
{
    int n = g1.size();
    int m = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            m += g1[i][j];
    m /= 2;
    return random_graph(n, m);
}

Graph random_given_degrees_impl(std::vector<int> degrees)
{
    int n = degrees.size();
    Graph g(n);
    int edges = std::accumulate(degrees.begin(), degrees.end(), 0) / 2;
    while (true)
    {
        int mx = std::max_element(degrees.begin(), degrees.end()) - degrees.begin();
        int deg = degrees[mx];
        degrees[mx] = 0;
        if (deg == 0)
            break;
        if (deg > edges)
            return Graph(0);
        edges -= deg;
        std::vector<int> options;
        for (int i = 0; i < n; i++)
            if (degrees[i] > edges)
                g.add_edge(i, mx),
                degrees[i]--,
                deg--;
            else if (degrees[i] > 0)
                options.push_back(i);
        std::shuffle(options.begin(), options.end(), rng);
        if (options.size() < deg)
            return Graph(0);
        options.resize(deg);
        for (int i : options)
            g.add_edge(i, mx),
            degrees[i]--;
    }
    return g;
}

Graph random_given_degrees(const std::vector<int>& degrees)
{
    for (int i = 1;; i++)
    {
        Graph g = random_given_degrees_impl(degrees);
        if (!g.empty())
            return g;
    }
}

Graph random_same_degrees(const Graph& g1)
{
    int n = g1.size();
    std::vector<int> degrees(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            degrees[i] += g1[i][j];
    std::shuffle(degrees.begin(), degrees.end(), rng);
    return random_given_degrees(degrees);
}

Graph random_regular(int n, int k)
{
    while (n * k % 2 != 0 || k < 0 || k >= n)
        k = rng() % n;
    std::vector<int> degrees(n, k);
    return random_given_degrees(degrees);
}
