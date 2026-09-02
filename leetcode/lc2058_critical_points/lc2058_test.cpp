#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 2058 examples") {
    auto* a = lc2058::build({3, 1});
    CHECK(lc2058::Solution().nodesBetweenCriticalPoints(a) == std::vector<int>({-1, -1}));
    auto* b = lc2058::build({5, 3, 1, 2, 5, 1, 2});
    CHECK(lc2058::Solution().nodesBetweenCriticalPoints(b) == std::vector<int>({1, 3}));
    auto* c = lc2058::build({1, 3, 2, 2, 3, 2, 2, 2, 7});
    CHECK(lc2058::Solution().nodesBetweenCriticalPoints(c) == std::vector<int>({3, 3}));
}

TEST_CASE("LC 2058 exhaustive lists over {1,2,3}, len <= 10") {
    for (int len = 2; len <= 10; len++) {
        int total = 1;
        for (int t = 0; t < len; t++) total *= 3;
        for (int mask = 0; mask < total; mask++) {
            std::vector<int> v(len);
            int m = mask;
            for (int p = 0; p < len; p++) { v[p] = m % 3 + 1; m /= 3; }
            auto* head = lc2058::build(v);
            auto got = lc2058::Solution().nodesBetweenCriticalPoints(head);
            auto want = lc2058::SolutionRef().nodesBetweenCriticalPoints(head);
            REQUIRE_MESSAGE(got == want, "mask=", mask, " len=", len);
        }
    }
}

TEST_CASE("LC 2058 random lists, values 1..100, len <= 500") {
    std::mt19937 rng(2058);
    for (int it = 0; it < 300; it++) {
        int len = 2 + rng() % 499;
        std::vector<int> v(len);
        for (auto& x : v) x = 1 + rng() % 100;
        auto* head = lc2058::build(v);
        CHECK(lc2058::Solution().nodesBetweenCriticalPoints(head) ==
              lc2058::SolutionRef().nodesBetweenCriticalPoints(head));
    }
}
