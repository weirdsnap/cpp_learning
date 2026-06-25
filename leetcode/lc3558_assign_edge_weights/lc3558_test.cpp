#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "solution.hpp"

TEST_CASE("LC 3558 single node") {
    std::vector<std::vector<int>> edges;
    CHECK(lc3558::Solution().assignEdgeWeights(edges) == 0);
}

TEST_CASE("LC 3558 chain of length 2") {
    std::vector<std::vector<int>> edges = {{1, 2}, {2, 3}};
    CHECK(lc3558::Solution().assignEdgeWeights(edges) == 2);
}

TEST_CASE("LC 3558 star") {
    std::vector<std::vector<int>> edges = {{1, 2}, {1, 3}, {1, 4}};
    CHECK(lc3558::Solution().assignEdgeWeights(edges) == 1);
}

TEST_CASE("LC 3558 mixed tree") {
    std::vector<std::vector<int>> edges = {{1, 2}, {1, 3}, {2, 4}};
    CHECK(lc3558::Solution().assignEdgeWeights(edges) == 2);
}
