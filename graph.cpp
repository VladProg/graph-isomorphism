#include "graph.h"
#include <iostream>

Graph::Graph(int n) :std::vector<std::vector<bool>>(n, std::vector<bool>(n)) {}

const std::vector<bool>& Graph::operator[](size_t v) const
{
    return base()[v];
}

void Graph::add_edge(int u, int v)
{
    if (u != v)
        base()[u][v] = base()[v][u] = true;
}

void Graph::print() const
{
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < size(); j++)
            if (i < j && base()[i][j])
                std::cout << i << " " << j << "\n";
    }
}

Graph::Base& Graph::base()
{
    return *this;
}

const Graph::Base& Graph::base() const
{
    return *this;
}

Graph read_graph()
{
    std::cout << "vertices count: ";
    int n;
    std::cin >> n;
    std::cout << "edges count: ";
    int m;
    std::cin >> m;
    Graph g(n);
    std::cout << "edges:\n";
    for (int i = 0; i < m; i++)
    {
        int u, v;
        std::cin >> u >> v;
        g.add_edge(u, v);
    }
    return g;
}
