#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 62 examples") {
    CHECK(lc62::Solution().uniquePaths(3, 7) == 28);
    CHECK(lc62::Solution().uniquePaths(3, 2) == 3);
    CHECK(lc62::Solution().uniquePaths(1, 1) == 1);
    CHECK(lc62::Solution().uniquePaths(1, 100) == 1);
    CHECK(lc62::Solution().uniquePaths(100, 1) == 1);
}

TEST_CASE("LC 62 cross-check: rolling / 2D / combinatorial") {
    std::mt19937 rng(62);
    for (int it = 0; it < 500; it++) {
        int m = 1 + rng() % 30, n = 1 + rng() % 30;
        int expected = lc62::Solution2D().uniquePaths(m, n);
        CHECK(lc62::Solution().uniquePaths(m, n) == expected);
        CHECK(lc62::SolutionMath().uniquePaths(m, n) == expected);
        // 对称性：m×n 与 n×m 答案相同（swap 优化的前提）
        CHECK(lc62::Solution().uniquePaths(n, m) == expected);
    }
}
