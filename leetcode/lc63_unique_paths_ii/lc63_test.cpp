#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 63 examples") {
    std::vector<std::vector<int>> a{{0, 0, 0}, {0, 1, 0}, {0, 0, 0}};
    std::vector<std::vector<int>> b{{0, 1}, {0, 0}};
    std::vector<std::vector<int>> a2 = a, b2 = b;
    CHECK(lc63::Solution().uniquePathsWithObstacles(a) == 2);
    CHECK(lc63::Solution().uniquePathsWithObstacles(b) == 1);
    CHECK(lc63::Solution2D().uniquePathsWithObstacles(a2) == 2);
    CHECK(lc63::Solution2D().uniquePathsWithObstacles(b2) == 1);
}

TEST_CASE("LC 63 edge cases") {
    // 起点就是障碍物
    std::vector<std::vector<int>> s{{1, 0}, {0, 0}};
    CHECK(lc63::Solution().uniquePathsWithObstacles(s) == 0);
    // 终点是障碍物
    std::vector<std::vector<int>> e{{0, 0}, {0, 1}};
    CHECK(lc63::Solution().uniquePathsWithObstacles(e) == 0);
    // 第一列被障碍物截断
    std::vector<std::vector<int>> c{{0}, {1}, {0}};
    CHECK(lc63::Solution().uniquePathsWithObstacles(c) == 0);
    // 1x1 无障碍
    std::vector<std::vector<int>> one{{0}};
    CHECK(lc63::Solution().uniquePathsWithObstacles(one) == 1);
}

TEST_CASE("LC 63 random cross-check 1D vs 2D") {
    std::mt19937 rng(63);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 8, m = 1 + rng() % 8;
        std::vector<std::vector<int>> g(n, std::vector<int>(m, 0));
        for (auto& row : g)
            for (auto& x : row) x = (rng() % 4 == 0) ? 1 : 0;  // 1/4 概率障碍
        CHECK(lc63::Solution().uniquePathsWithObstacles(g)
              == lc63::Solution2D().uniquePathsWithObstacles(g));
    }
}
