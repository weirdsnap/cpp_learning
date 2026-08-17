#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 877 examples") {
    std::vector<int> a{5, 3, 4, 5};
    CHECK(lc877::Solution().stoneGame(a) == true);

    std::vector<int> b{3, 7, 2, 3};
    CHECK(lc877::Solution().stoneGame(b) == true);
}

TEST_CASE("LC 877 odd n: DP vs brute") {
    std::mt19937 rng(877);
    for (int it = 0; it < 300; it++) {
        int n = 3 + 2 * (rng() % 4);              // 奇数堆 3, 5, 7, 9
        std::vector<int> piles(n);
        for (auto& x : piles) x = 1 + rng() % 20;
        auto c1 = piles, c2 = piles;
        int net = lc877::SolutionBrute().stoneGameNet(c1);
        CAPTURE(n);
        CHECK(lc877::SolutionDp().stoneGameNet(c2) == net);
        CHECK(lc877::Solution().stoneGame(piles) == (net >= 0));
    }
}

TEST_CASE("LC 877 even n: first player never loses") {
    // 性质验证：偶数堆时先手总能拿到奇偶下标两组中较多的那组，净胜分 >= 0
    std::mt19937 rng(878);
    for (int it = 0; it < 300; it++) {
        int n = 2 + 2 * (rng() % 5);              // 偶数堆 2, 4, 6, 8, 10
        std::vector<int> piles(n);
        for (auto& x : piles) x = 1 + rng() % 20;
        auto c1 = piles, c2 = piles;
        CHECK(lc877::Solution().stoneGame(piles) == true);
        CHECK(lc877::SolutionBrute().stoneGameNet(c1) >= 0);
        CHECK(lc877::SolutionDp().stoneGameNet(c2) >= 0);
    }
}
