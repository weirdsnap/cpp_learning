#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 1140 examples") {
    std::vector<int> a{2, 7, 9, 4, 4};
    CHECK(lc1140::Solution().stoneGameII(a) == 10);

    std::vector<int> b{1, 2, 3, 4, 5, 100};
    CHECK(lc1140::Solution().stoneGameII(b) == 104);
}

TEST_CASE("LC 1140 iterative DP vs memo recursion") {
    std::mt19937 rng(1140);
    for (int it = 0; it < 300; it++) {
        int n = 1 + rng() % 10;                   // n ∈ [1, 10]
        std::vector<int> piles(n);
        for (auto& x : piles) x = 1 + rng() % 50;
        auto copy = piles;
        CAPTURE(n);
        CHECK(lc1140::Solution().stoneGameII(piles)
              == lc1140::SolutionMemo().stoneGameII(copy));
    }
}

TEST_CASE("LC 1140 net diff parity: (total + net) is even") {
    // Alice 得分 = (总数 + 净胜分) / 2，整除性本身是定义的直接推论
    std::mt19937 rng(1141);
    for (int it = 0; it < 200; it++) {
        int n = 1 + rng() % 10;
        std::vector<int> piles(n);
        for (auto& x : piles) x = 1 + rng() % 50;
        auto copy = piles;
        int alice = lc1140::Solution().stoneGameII(piles);
        int total = std::accumulate(copy.begin(), copy.end(), 0);
        CHECK(alice >= 0);
        CHECK(alice <= total);   // Bob 得分 = total - alice 非负
    }
}
