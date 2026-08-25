#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 1686 examples") {
    std::vector<int> a1{1, 3}, b1{2, 1};
    CHECK(lc1686::Solution().stoneGameVI(a1, b1) == 1);

    std::vector<int> a2{1, 2}, b2{3, 1};
    CHECK(lc1686::Solution().stoneGameVI(a2, b2) == 0);

    std::vector<int> a3{2, 4, 3}, b3{1, 6, 7};
    CHECK(lc1686::Solution().stoneGameVI(a3, b3) == -1);
}

TEST_CASE("LC 1686 greedy vs bitmask game search") {
    std::mt19937 rng(1686);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 10;                   // n ∈ [1, 10]
        std::vector<int> av(n), bv(n);
        for (int i = 0; i < n; i++) {
            av[i] = 1 + rng() % 50;
            bv[i] = 1 + rng() % 50;
        }
        auto ac = av, bc = bv;
        CAPTURE(n);
        CHECK(lc1686::Solution().stoneGameVI(av, bv)
              == lc1686::SolutionBrute().stoneGameVI(ac, bc));
    }
}
