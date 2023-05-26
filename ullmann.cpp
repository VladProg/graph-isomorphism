#include "algorithms.h"
#include <algorithm>
#include <numeric>
#include "boost/dynamic_bitset.hpp"

using BitsetRow = boost::dynamic_bitset<uint64_t>;
using BitsetMatrix = std::vector<BitsetRow>;

void refinement(const BitsetMatrix& g1, const BitsetMatrix& g2, BitsetMatrix& match_matrix)
{
    const int n = g1.size();
    BitsetMatrix transposed_matrix(n, BitsetRow(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            transposed_matrix[j][i] = match_matrix[i][j];
    while (true)
    {
        bool changed = false;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (match_matrix[i][j])
                    for (int x = 0; x < n; x++)
                        if (g1[i][x] && !g2[j].intersects(match_matrix[x]))
                        {
                            match_matrix[i][j] = false;
                            transposed_matrix[j][i] = false;
                            changed = true;
                            break;
                        }
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (match_matrix[i][j])
                    for (int y = 0; y < n; y++)
                        if (g2[j][y] && !g1[i].intersects(transposed_matrix[y]))
                        {
                            match_matrix[i][j] = false;
                            transposed_matrix[j][i] = false;
                            changed = true;
                            break;
                        }
        if (!changed)
            break;
    }
}

bool has_empty_row_or_column(const BitsetMatrix& match_matrix)
{
    const int n = match_matrix.size();
    for (int i = 0; i < n; i++)
        if (match_matrix[i].none())
            return true;
    for (int i = 0; i < n; i++)
        if (std::none_of(match_matrix.begin(), match_matrix.end(), [i](const BitsetRow& row) {return row[i]; }))
            return true;
    return false;
}

int first_row_with_several_elements(const BitsetMatrix& match_matrix)
{
    const int n = match_matrix.size();
    int row = -1, cnt = n + 1;
    for (int i = 0; i < n; i++)
    {
        int cur = match_matrix[i].count();
        if (cur > 1 && cur < cnt)
            row = i,
            cnt = cur;
    }
    return row;
}

BitsetMatrix set_matching(BitsetMatrix match_matrix, int i, int j)
{
    const int n = match_matrix.size();
    match_matrix[i].reset();
    for (int k = 0; k < n; k++)
        match_matrix[k][j] = false;
    match_matrix[i][j] = true;
    return match_matrix;
}

bool rec(const BitsetMatrix& g1, const BitsetMatrix& g2, BitsetMatrix match_matrix)
{
    const int n = g1.size();
    refinement(g1, g2, match_matrix);
    if (has_empty_row_or_column(match_matrix))
        return false;
    int r = first_row_with_several_elements(match_matrix);
    if (r == -1)
        return true;
    for (int j = 0; j < n; j++)
        if (match_matrix[r][j])
            if (rec(g1, g2, set_matching(match_matrix, r, j)))
                return true;
    return false;
}

bool ullmann(const Graph& g1, const Graph& g2)
{
    if (g1.size() != g2.size())
        return false;
    const int n = g1.size();

    std::vector<int> d1(n);
    for (int i = 0; i < n; i++)
        d1[i] = accumulate(g1[i].begin(), g1[i].end(), 0);

    BitsetMatrix b1(n, BitsetRow(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            b1[i][j] = g1[i][j];

    std::vector<int> d2(n);
    for (int i = 0; i < n; i++)
        d2[i] = accumulate(g2[i].begin(), g2[i].end(), 0);

    BitsetMatrix b2(n, BitsetRow(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            b2[i][j] = g2[i][j];

    BitsetMatrix match_matrix(n, BitsetRow(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            match_matrix[i][j] = d1[i] == d2[j];

    return rec(b1, b2, match_matrix);
}
