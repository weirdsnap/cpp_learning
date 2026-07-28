#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <functional>
#include <random>

#include "solution.hpp"

TEST_CASE("LC 198 examples") {
    std::vector<int> a = {1, 2, 3, 1};
    CHECK(lc198::SolutionMemo().rob(a) == 4);
    std::vector<int> b = {2, 7, 9, 3, 1};
    CHECK(lc198::SolutionIterative().rob(b) == 12);
    std::vector<int> c = {2, 7, 9, 3, 1};
    CHECK(lc198::SolutionExplicit2D().rob(c) == 12);
    std::vector<int> d = {1, 2, 3, 1};
    CHECK(lc198::SolutionRolling().rob(d) == 4);
}

TEST_CASE("LC 198 edge cases") {
    std::vector<int> one = {5};
    CHECK(lc198::SolutionRolling().rob(one) == 5);
    std::vector<int> two = {3, 8};
    CHECK(lc198::SolutionMemo().rob(two) == 8);
}

TEST_CASE("LC 198 random cross-check: four solutions agree with brute force") {
    std::mt19937 rng(42);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 12;
        std::vector<int> v(n);
        for (auto& x : v) x = rng() % 20;

        // 暴力：递归枚举偷/不偷
        std::function<int(int)> brute = [&](int i) -> int {
            if (i >= n) return 0;
            return std::max(brute(i + 1), v[i] + brute(i + 2));
        };
        int expect = brute(0);

        auto a = v, b = v, c = v, d = v;
        CHECK(lc198::SolutionMemo().rob(a) == expect);
        CHECK(lc198::SolutionIterative().rob(b) == expect);
        CHECK(lc198::SolutionExplicit2D().rob(c) == expect);
        CHECK(lc198::SolutionRolling().rob(d) == expect);
    }
}
