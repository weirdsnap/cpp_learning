#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 1563 examples") {
    std::vector<int> a{6, 2, 3, 4, 5, 5};
    CHECK(lc1563::Solution().stoneGameV(a) == 18);

    std::vector<int> b{7, 7, 7, 7, 7, 7, 7};
    CHECK(lc1563::Solution().stoneGameV(b) == 28);

    std::vector<int> c{4};
    CHECK(lc1563::Solution().stoneGameV(c) == 0);
}

TEST_CASE("LC 1563 interval DP vs brute") {
    std::mt19937 rng(1563);
    for (int it = 0; it < 300; it++) {
        int n = 1 + rng() % 12;                   // n ∈ [1, 12]
        std::vector<int> v(n);
        for (auto& x : v) x = 1 + rng() % 20;     // 正分值
        auto copy = v, copy2 = v, copy3 = v, copy4 = v;
        CAPTURE(n);
        CHECK(lc1563::Solution().stoneGameV(v)
              == lc1563::SolutionBrute().stoneGameV(copy));
        CHECK(lc1563::SolutionHalfOpen().stoneGameV(copy2)
              == lc1563::SolutionBrute().stoneGameV(v));
        CHECK(lc1563::SolutionFast().stoneGameV(copy3)
              == lc1563::SolutionBrute().stoneGameV(v));
        CHECK(lc1563::SolutionFastTable().stoneGameV(copy4)
              == lc1563::SolutionBrute().stoneGameV(v));
    }
}

TEST_CASE("LC 1563 equal-sum splits prefer better subtree") {
    // 等和分裂时左右都可保留，取 dp 较大者——构造等和场景验证分支
    std::vector<int> v{3, 3, 3, 3};   // [3,3]|[3,3] 等和分裂
    auto a = v, b = v, c = v, d = v;
    CHECK(lc1563::Solution().stoneGameV(v) == lc1563::SolutionBrute().stoneGameV(a));
    CHECK(lc1563::Solution().stoneGameV(b) == 9);   // 6 + dp([3,3])=6+3
    CHECK(lc1563::SolutionFast().stoneGameV(c) == 9);
    CHECK(lc1563::SolutionFastTable().stoneGameV(d) == 9);
}

TEST_CASE("LC 1563 fast (O(n^2)) vs cubic, n up to 120") {
    std::mt19937 rng(1564);
    for (int it = 0; it < 100; it++) {
        int n = 2 + rng() % 119;                  // n ∈ [2, 120]
        std::vector<int> v(n);
        for (auto& x : v) x = 1 + rng() % 100;
        auto copy = v;
        CAPTURE(n);
        CHECK(lc1563::SolutionFast().stoneGameV(v)
              == lc1563::Solution().stoneGameV(copy));
        CHECK(lc1563::SolutionFastTable().stoneGameV(copy)
              == lc1563::Solution().stoneGameV(v));
    }
}
