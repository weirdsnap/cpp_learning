#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 3742 examples") {
    std::vector<std::vector<int>> g1{{0, 1}, {2, 0}};
    std::vector<std::vector<int>> g2{{0, 1}, {1, 2}};
    CHECK(lc3742::Solution3D().maxPathScore(g1, 1) == 2);
    CHECK(lc3742::Solution3D().maxPathScore(g2, 1) == -1);
    CHECK(lc3742::SolutionRollingFixed().maxPathScore(g1, 1) == 2);
    CHECK(lc3742::SolutionRollingFixed().maxPathScore(g2, 1) == -1);
}

TEST_CASE("LC 3742 rolling parity check") {
    // rows=2 时用户原版 lastRow 选 prev（上一行），样例 1 会出错
    std::vector<std::vector<int>> g1{{0, 1}, {2, 0}};
    CHECK(lc3742::SolutionRolling().maxPathScore(g1, 1) != 2);  // 证实 bug
}

TEST_CASE("LC 3742 random cross-check") {
    std::mt19937 rng(3742);
    int mismatches = 0;
    for (int it = 0; it < 500; it++) {
        int rows = 1 + rng() % 8, cols = 1 + rng() % 8;
        int k = rng() % (rows + cols);
        std::vector<std::vector<int>> g(rows, std::vector<int>(cols));
        for (auto& row : g)
            for (auto& x : row) x = rng() % 3;
        g[0][0] = 0;  // 题意保证
        int expected = lc3742::Solution3D().maxPathScore(g, k);
        CHECK(lc3742::SolutionRollingFixed().maxPathScore(g, k) == expected);
#if LC3742_HAS_DEDUCING_THIS
        CHECK(lc3742::SolutionMemo().maxPathScore(g, k) == expected);
#endif
        if (lc3742::SolutionRolling().maxPathScore(g, k) != expected) mismatches++;
    }
    MESSAGE("rolling 原版(奇偶选择) 500 组中出错 " << mismatches << " 组");
}
