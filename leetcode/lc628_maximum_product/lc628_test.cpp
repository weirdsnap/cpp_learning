#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 628 examples") {
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{1, 2, 3, 4};
    std::vector<int> c{-1, -2, -3};
    CHECK(lc628::Solution().maximumProduct(a) == 6);
    CHECK(lc628::Solution().maximumProduct(b) == 24);
    CHECK(lc628::Solution().maximumProduct(c) == -6);
}

TEST_CASE("LC 628 negatives matter") {
    // 两个负数 × 最大正数 > 三个最大正数
    std::vector<int> v{-100, -98, -1, 2, 3, 4};
    CHECK(lc628::Solution().maximumProduct(v) == 39200);       // -100*-98*4
    CHECK(lc628::SolutionScan().maximumProduct(v) == 39200);
    // 全是负数：取绝对值最小的三个（即排序后最大的三个）
    std::vector<int> w{-5, -4, -3, -2, -1};
    CHECK(lc628::Solution().maximumProduct(w) == -6);          // -1*-2*-3
    CHECK(lc628::SolutionScan().maximumProduct(w) == -6);
}

TEST_CASE("LC 628 sort vs scan vs brute") {
    std::mt19937 rng(628);
    for (int it = 0; it < 300; it++) {
        int n = 3 + rng() % 12;
        std::vector<int> v(n);
        for (auto& x : v) x = static_cast<int>(rng() % 2001) - 1000;
        auto v2 = v, v3 = v;
        int expected = lc628::SolutionBrute().maximumProduct(v);
        CHECK(lc628::Solution().maximumProduct(v2) == expected);
        CHECK(lc628::SolutionScan().maximumProduct(v3) == expected);
    }
}
