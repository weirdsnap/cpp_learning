#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 486 examples") {
    std::vector<int> a{1, 5, 2}, b{1, 5, 233, 7}, tie{1, 1};
    std::vector<int> a2 = a, b2 = b, tie2 = tie;
    std::vector<int> a3 = a, b3 = b, tie3 = tie;
    CHECK(lc486::SolutionDP().predictTheWinner(a) == false);
    CHECK(lc486::SolutionDP().predictTheWinner(b) == true);
    // 平局算玩家 1 赢
    CHECK(lc486::SolutionDP().predictTheWinner(tie) == true);
    CHECK(lc486::SolutionDPOpt().predictTheWinner(a3) == false);
    CHECK(lc486::SolutionDPOpt().predictTheWinner(b3) == true);
    CHECK(lc486::SolutionDPOpt().predictTheWinner(tie3) == true);
#if LC486_HAS_DEDUCING_THIS
    CHECK(lc486::Solution().predictTheWinner(a2) == false);
    CHECK(lc486::Solution().predictTheWinner(b2) == true);
    CHECK(lc486::Solution().predictTheWinner(tie2) == true);
#endif
}

TEST_CASE("LC 486 cross-check: recursion / 2D DP / 1D DP") {
    std::mt19937 rng(486);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 12;  // 小 n，纯递归 O(2^n) 也能跑
        std::vector<int> nums(n);
        for (auto& x : nums) x = rng() % 50;
        bool expected = lc486::SolutionDP().predictTheWinner(nums);
        CHECK(lc486::SolutionDPOpt().predictTheWinner(nums) == expected);
#if LC486_HAS_DEDUCING_THIS
        CHECK(lc486::Solution().predictTheWinner(nums) == expected);
#endif
    }
}
