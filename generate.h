#pragma once

#include "graph.h"

Graph random_graph(int n, int m = -1);
Graph random_isomorphic(const Graph& g1);
Graph random_same_nm(const Graph& g1);
Graph random_given_degrees(const std::vector<int>& degrees);
Graph random_same_degrees(const Graph& g1);
Graph random_regular(int n, int k = -1);
