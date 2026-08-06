#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>
#include <vector>

#include "solution.hpp"

TEST_CASE("LC 3418 examples") {
    std::vector<std::vector<int>> a{{0, 1, -1}, {1, -2, 3}, {2, -3, 4}};
    std::vector<std::vector<int>> b{{10, 10, 10}, {10, 10, 10}};
    std::vector<std::vector<int>> a2 = a, b2 = b, a3 = a, b3 = b;
    CHECK(lc3418::Solution().maximumAmount(a) == 8);
    CHECK(lc3418::Solution().maximumAmount(b) == 40);
    CHECK(lc3418::Solution3D().maximumAmount(a2) == 8);
    CHECK(lc3418::Solution3D().maximumAmount(b2) == 40);
    CHECK(lc3418::SolutionPlain().maximumAmount(a3) == 8);
    CHECK(lc3418::SolutionPlain().maximumAmount(b3) == 40);
}

TEST_CASE("LC 3418 edge cases") {
    // 全是劫匪：两次感化用在最狠的两格
    std::vector<std::vector<int>> all_neg{{-5, -1}, {-2, -3}};
    std::vector<std::vector<int>> an2 = all_neg;
    CHECK(lc3418::Solution().maximumAmount(all_neg) == lc3418::Solution3D().maximumAmount(an2));
    // 单格负数：感化后为 0
    std::vector<std::vector<int>> one{{-7}};
    CHECK(lc3418::Solution().maximumAmount(one) == 0);
    // 总收益可以为负（劫匪超过 2 格）
    std::vector<std::vector<int>> deep{{-5, -5, -5}};
    CHECK(lc3418::Solution().maximumAmount(deep) == -5);
}

TEST_CASE("LC 3418 rolling vs 3D randomized") {
    std::mt19937 rng(3418);
    for (int iter = 0; iter < 300; iter++) {
        int n = 1 + rng() % 6, m = 1 + rng() % 6;
        std::vector<std::vector<int>> g(n, std::vector<int>(m));
        for (auto& row : g)
            for (auto& x : row) x = static_cast<int>(rng() % 21) - 10;  // [-10, 10]
        auto g2 = g, g3 = g;
        int expected = lc3418::Solution3D().maximumAmount(g2);
        CHECK(lc3418::Solution().maximumAmount(g) == expected);
        CHECK(lc3418::SolutionPlain().maximumAmount(g3) == expected);
    }
}
