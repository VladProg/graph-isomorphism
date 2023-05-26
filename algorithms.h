#pragma once

#include "graph.h"
#include <string>

using Algorithm = bool(const Graph&, const Graph&);

Algorithm naive, invariants, ullmann, mckay;

using NamedAlgorithm = std::pair<Algorithm*, std::string>;
#define NAMED_ALGORITHM(name) NamedAlgorithm{name, #name}
