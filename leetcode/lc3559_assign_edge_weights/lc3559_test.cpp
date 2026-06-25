#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "solution.hpp"

TEST_CASE("LC 3559 basic tree") {
    std::vector<std::vector<int>> edges = {{1, 2}, {1, 3}, {2, 4}};
    std::vector<std::vector<int>> queries = {{4, 3}, {4, 4}, {2, 3}};
    lc3559::Solution sol;
    auto ans = sol.assignEdgeWeights(edges, queries);
    REQUIRE(ans.size() == 3);
    CHECK(ans[0] == 4);
    CHECK(ans[1] == 0);
    CHECK(ans[2] == 2);
}

TEST_CASE("LC 3559 single node") {
    std::vector<std::vector<int>> edges;
    std::vector<std::vector<int>> queries = {{1, 1}};
    lc3559::Solution sol;
    auto ans = sol.assignEdgeWeights(edges, queries);
    REQUIRE(ans.size() == 1);
    CHECK(ans[0] == 0);
}

TEST_CASE("LC 3559 chain") {
    std::vector<std::vector<int>> edges = {{1, 2}, {2, 3}, {3, 4}};
    std::vector<std::vector<int>> queries = {{1, 4}};
    lc3559::Solution sol;
    auto ans = sol.assignEdgeWeights(edges, queries);
    REQUIRE(ans.size() == 1);
    CHECK(ans[0] == 4);
}
