#include "test.h"
#include "generate.h"
#include <type_traits>
#include <tuple>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>

std::string current_time()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::string now_string = std::ctime(&now_time_t);
    now_string.pop_back();
    return now_string;
}

void test(int n, std::vector<NamedAlgorithm> algorithms, int tests_count)
{
    std::cerr << current_time() << " : test(" << n << ", {";
    for (const auto& [algorithm, name] : algorithms)
        std::cerr << " " << name;
    std::cerr << " }";
    if (tests_count != std::numeric_limits<int>::max())
        std::cerr << ", " << tests_count;
    std::cerr << ")" << std::endl;

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::cout << std::fixed << std::setprecision(7) << std::boolalpha;
    for (int test_index = 0; test_index < tests_count; test_index++)
    {
        Graph g1 = (rng() % 2 ? random_graph(n) : random_regular(n));
        bool isomorphic = rng() % 2;
        Graph g2 = (isomorphic ? random_isomorphic(g1) :
            rng() % 2 ? random_same_nm(g1) :
            random_same_degrees(g1));
        std::vector<bool> answers;
        for (const auto& [algorithm, name] : algorithms)
        {
            const auto start = std::chrono::steady_clock::now();
            answers.push_back(algorithm(g1, g2));
            const auto end = std::chrono::steady_clock::now();
            const std::chrono::duration<double> diff = end - start;
            std::cout << name << " " << n << " -> " << diff.count() << " s" << std::endl;
        }
        bool expected = isomorphic || answers.front();
        bool correct = true;
        for (auto answer : answers)
            if (answer != expected)
                correct = false;
        if (!correct)
        {
            std::cerr << "\nGraph 1:\n";
            g1.print();
            std::cerr << "\nGraph 2:\n";
            g2.print();
            std::cerr << (isomorphic ?
                "\nThese graphs are isomorphic but return values are:\n" :
                "\nReturn values are different:\n");
            for (int i = 0; i < algorithms.size(); i++)
                std::cerr << algorithms[i].second << " -> " << answers[i] << "\n";
            std::exit(0);
        }
    }

    std::cerr << current_time() << " : finished\n" << std::endl;
}
