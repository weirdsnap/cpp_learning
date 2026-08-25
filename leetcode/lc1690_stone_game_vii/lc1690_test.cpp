#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 1690 examples") {
    std::vector<int> a{5, 3, 1, 4, 2};
    CHECK(lc1690::Solution().stoneGameVII(a) == 6);

    std::vector<int> b{7, 90, 5, 1, 100, 10, 10, 2};
    CHECK(lc1690::Solution().stoneGameVII(b) == 122);
}

TEST_CASE("LC 1690 interval DP vs brute") {
    std::mt19937 rng(1690);
    for (int it = 0; it < 300; it++) {
        int n = 2 + rng() % 11;                   // n ∈ [2, 12]
        std::vector<int> v(n);
        for (auto& x : v) x = 1 + rng() % 100;
        auto copy = v, c2 = v, c3 = v;
        int expected = lc1690::SolutionBrute().stoneGameVII(copy);
        CAPTURE(n);
        CHECK(lc1690::Solution().stoneGameVII(v) == expected);
        CHECK(lc1690::SolutionRolling().stoneGameVII(c2) == expected);
        CHECK(lc1690::SolutionLen().stoneGameVII(c3) == expected);
    }
}
