#pragma once

#include <vector>

struct Graph :std::vector<std::vector<bool>>
{
    Graph(int n);
    const std::vector<bool>& operator[](size_t v) const;
    void add_edge(int u, int v);
    void print() const;
private:
    using Base = std::vector<std::vector<bool>>;
    Base& base();
    const Base& base() const;
};

Graph read_graph();
