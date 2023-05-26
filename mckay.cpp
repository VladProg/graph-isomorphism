#include "algorithms.h"
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cassert>

using OrderedPartition = std::vector<std::set<int>>;
using UnorderedPartition = std::set<std::set<int>>;
using PartitionNest = std::vector<OrderedPartition>;
using Permutation = std::vector<int>;

OrderedPartition divide_X_by_degree_wrt_Y(const Graph& g, const std::set<int>& X, const std::set<int>& Y)
{
    std::map<int, std::set<int>> with_degree;
    for (int x : X)
    {
        int d = 0;
        for (int y : Y)
            d += g[x][y];
        with_degree[d].insert(x);
    }
    OrderedPartition res;
    for (auto& [d, xs] : with_degree)
        res.push_back(xs);
    return res;
}

OrderedPartition refinement(const Graph& g, OrderedPartition pi, OrderedPartition alpha)
{
    const int n = g.size();
    while (!alpha.empty() && pi.size() < n)
    {
        std::set<int> W = alpha.back();
        alpha.pop_back();
        OrderedPartition new_pi;
        for (const std::set<int>& cell : pi)
        {
            OrderedPartition by_degree = divide_X_by_degree_wrt_Y(g, cell, W);
            auto mx = std::max_element(by_degree.begin(), by_degree.end(),
                [](const std::set<int>& x, const std::set<int>& y) {return x.size() < y.size(); });
            if (auto it = find(alpha.begin(), alpha.end(), cell); it != alpha.end())
                *it = *mx;
            for (auto it = by_degree.begin(); it != by_degree.end(); it++)
                if (it != mx)
                    alpha.push_back(*it);
            std::move(by_degree.begin(), by_degree.end(), back_inserter(new_pi));
        }
        pi = std::move(new_pi);
    }
    return pi;
}

OrderedPartition refinement_with_one_vertex(const Graph& g, const OrderedPartition& pi, int v)
{
    OrderedPartition new_pi;
    for (const std::set<int>& cell : pi)
        if (cell.count(v))
            if (cell.size() == 1)
                return pi;
            else
            {
                new_pi.push_back({ v });
                new_pi.push_back(cell);
                new_pi.back().erase(v);
            }
        else
            new_pi.push_back(cell);
    return refinement(g, std::move(new_pi), { {v} });
}

OrderedPartition unit_partition(int n)
{
    std::set<int> s;
    for (int i = 0; i < n; i++)
        s.insert(i);
    return { s };
}

UnorderedPartition discrete_partition(int n)
{
    UnorderedPartition pi;
    for (int i = 0; i < n; i++)
        pi.insert({ i });
    return pi;
}

std::set<int> first_non_trivial_smallest_cell(const OrderedPartition& pi)
{
    int sz = INT_MAX;
    int ind = -1;
    for (int i = 0; i < pi.size(); i++)
        if (pi[i].size() > 1 && pi[i].size() < sz)
            sz = pi[i].size(),
            ind = i;
    return pi[ind];
}

int indicator(const Graph& g, const OrderedPartition& pi)
{
    constexpr unsigned BASE = 31;
    unsigned res = 1;
    for (const std::set<int>& cell : pi)
    {
        unsigned cnt = 0;
        int x = *cell.begin();
        for (int y : cell)
            if (g[x][y])
                cnt++;
        res = res * BASE + cell.size();
        res = res * BASE + cnt;
    }
    return abs((int)res);
}

Permutation discrete_partition_to_permutation(const OrderedPartition& pi)
{
    const int n = pi.size();
    Permutation res(n);
    for (int i = 0; i < n; i++)
        res[*pi[i].begin()] = i;
    return res;
}

Graph permute_graph(Graph g, Permutation p)
{
    const int n = g.size();
    Graph res(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (g[i][j])
                res.add_edge(p[i], p[j]);
    return res;
}

Permutation multiply_permutations(const Permutation& p, const Permutation& q)
{
    const int n = p.size();
    Permutation res(p.size());
    for (int i = 0; i < n; i++)
        res[i] = q[p[i]];
    return res;
}

Permutation inverse_permutation(const Permutation& p)
{
    const int n = p.size();
    Permutation res(p.size());
    for (int i = 0; i < n; i++)
        res[p[i]] = i;
    return res;
}

Permutation divide_permutations(const Permutation& p, const Permutation& q)
{
    return multiply_permutations(p, inverse_permutation(q));
}

UnorderedPartition cycles(const Permutation& p)
{
    const int n = p.size();
    std::vector<bool> used(n);
    UnorderedPartition res;
    for (int i = 0; i < n; i++)
        if (!used[i])
        {
            std::set<int> cell;
            int j = i;
            while (!used[j])
                cell.insert(j),
                used[j] = true,
                j = p[j];
            res.insert(std::move(cell));
        }
    return res;
}

template<class Partition>
std::set<int> fix(const Partition& pi)
{
    std::set<int> res;
    for (const std::set<int>& cell : pi)
        if (cell.size() == 1)
            res.insert(*cell.begin());
    return res;
}

template<class Partition>
std::set<int> mcr(const Partition& pi)
{
    std::set<int> res;
    for (const std::set<int>& cell : pi)
        res.insert(*cell.begin());
    return res;
}

UnorderedPartition unite(UnorderedPartition a, const UnorderedPartition& b)
{
    for (std::set<int> cell : b)
    {
        for (auto it = a.begin(); it != a.end();)
        {
            bool found = false;
            for (int x : *it)
                if (cell.count(x))
                {
                    found = true;
                    break;
                }
            if (found)
                std::move(it->begin(), it->end(), inserter(cell, cell.end())),
                a.erase(it++);
            else
                it++;
        }
        a.insert(std::move(cell));
    }
    return a;
}

std::set<int> intersect(const std::set<int>& x, const std::set<int>& y)
{
    std::set<int> res;
    std::set_intersection(x.begin(), x.end(), y.begin(), y.end(), std::inserter(res, res.end()));
    return res;
}

int non_trivial_cells(const OrderedPartition& pi)
{
    int res = 0;
    for (const std::set<int>& cell : pi)
        res += cell.size() > 1;
    return res;
}

Graph canonical(const Graph& g)
{
    const int n = g.size();
    PartitionNest nu, rho, sigma;
    std::vector<int> lambda_nu, lambda_rho, lambda_sigma;
    Permutation p_rho, p_sigma;
    Graph g_rho(0), g_sigma(0);
    UnorderedPartition theta = discrete_partition(n);
    std::vector<int> v;
    std::vector<std::pair<std::set<int>, std::set<int>>> psi;
    const int L = 100;
    int lowest_equal_lambda_nu_sigma = 0;
    int lowest_equal_lambda_nu_rho = 0;
    std::set<int> mcr_theta = mcr(theta);
    auto rec = [&](auto& self, const OrderedPartition& pi, int lca_nu_sigma)->void
    {
        int depth = nu.size();
        bool ancestor_of_sigma = sigma.empty();
        nu.push_back(pi);
        lambda_nu.push_back(indicator(g, pi));
        if (lowest_equal_lambda_nu_sigma >= depth - 1)
            if (ancestor_of_sigma || lambda_sigma.size() > depth && lambda_nu[depth] == lambda_sigma[depth])
                lowest_equal_lambda_nu_sigma = depth;
            else
                lowest_equal_lambda_nu_sigma = depth - 1;
        if (lowest_equal_lambda_nu_rho >= depth - 1)
            if (ancestor_of_sigma || lambda_rho.size() > depth && lambda_nu[depth] == lambda_rho[depth])
                lowest_equal_lambda_nu_rho = depth;
            else
                lowest_equal_lambda_nu_rho = depth - 1;
        if (lowest_equal_lambda_nu_sigma == depth || lowest_equal_lambda_nu_rho == depth ||
            lambda_nu[lowest_equal_lambda_nu_rho + 1] > lambda_rho[lowest_equal_lambda_nu_rho + 1])
        {
            if (pi.size() == n)
            {
                Permutation p_nu = discrete_partition_to_permutation(pi);
                Graph g_nu = permute_graph(g, p_nu);
                if (sigma.empty())
                    sigma = rho = nu,
                    lambda_sigma = lambda_rho = lambda_nu,
                    p_sigma = p_rho = p_nu,
                    g_sigma = g_rho = g_nu;
                else
                {
                    Permutation gamma;
                    if (tie(lambda_nu, g_nu) == tie(lambda_sigma, g_sigma))
                        gamma = divide_permutations(p_nu, p_sigma);
                    else if (tie(lambda_nu, g_nu) == tie(lambda_rho, g_rho))
                        gamma = divide_permutations(p_nu, p_rho);
                    else if (tie(lambda_nu, g_nu) > tie(lambda_rho, g_rho))
                        rho = nu,
                        lambda_rho = lambda_nu,
                        p_rho = p_nu,
                        g_rho = g_nu,
                        lowest_equal_lambda_nu_rho = depth;
                    if (!gamma.empty())
                    {
                        UnorderedPartition orb = cycles(gamma);
                        theta = unite(theta, orb);
                        mcr_theta = mcr(theta);
                        if (psi.size() == L)
                            psi.pop_back();
                        psi.push_back({ fix(orb),mcr(orb) });
                    }
                }
            }
            else
            {
                std::set<int> working_cell = first_non_trivial_smallest_cell(pi);
                v.emplace_back();
                int psi_index = 0;
                bool all_descendants_are_equivalend = n <= pi.size() + std::max(4, non_trivial_cells(pi) + 1);
                if (all_descendants_are_equivalend)
                {
                    if (psi.size() == L)
                        psi.pop_back();
                    psi.push_back({ fix(pi),mcr(pi) });
                }
                while (!working_cell.empty())
                {
                    v.back() = *working_cell.begin();
                    working_cell.erase(working_cell.begin());
                    if (ancestor_of_sigma && !mcr_theta.count(v.back()))
                        continue;
                    self(self, refinement_with_one_vertex(g, pi, v.back()), ancestor_of_sigma ? depth : lca_nu_sigma);
                    if (all_descendants_are_equivalend || depth != 0 && !mcr_theta.count(v[lca_nu_sigma]))
                        break;
                    if (psi_index == L)
                        psi_index--;
                    for (; psi_index < psi.size(); psi_index++)
                        if (std::all_of(v.begin(), v.end() - 1, [&](int x) {return psi[psi_index].first.count(x); }))
                            working_cell = intersect(working_cell, psi[psi_index].second);
                }
                v.pop_back();
            }
        }
        nu.pop_back();
        lambda_nu.pop_back();
    };
    rec(rec, refinement(g, unit_partition(n), unit_partition(n)), -1);
    return g_rho;
}

bool mckay(const Graph& g1, const Graph& g2)
{
    Graph c1 = canonical(g1);
    Graph c2 = canonical(g2);
    return c1 == c2;
}
