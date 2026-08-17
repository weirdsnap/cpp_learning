#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 2958 examples") {
    std::vector<int> a{1, 2, 3, 1, 2, 3, 1, 2};
    CHECK(lc2958::Solution().maxSubarrayLength(a, 2) == 6);

    std::vector<int> b{1, 2, 1, 2, 1, 2, 1, 2};
    CHECK(lc2958::Solution().maxSubarrayLength(b, 1) == 2);

    std::vector<int> c{5, 5, 5, 5, 5, 5, 5};
    CHECK(lc2958::Solution().maxSubarrayLength(c, 4) == 4);
}

TEST_CASE("LC 2958 edge cases") {
    std::vector<int> one{42};
    CHECK(lc2958::Solution().maxSubarrayLength(one, 1) == 1);
    // k >= n 时整个数组都合法
    std::vector<int> v{1, 1, 1};
    CHECK(lc2958::Solution().maxSubarrayLength(v, 3) == 3);
}

TEST_CASE("LC 2958 sliding window vs brute force") {
    std::mt19937 rng(2958);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 30;
        int k = 1 + rng() % 4;
        std::vector<int> g(n);
        for (auto& x : g) x = rng() % 6;  // 小值域，容易撞出超 k 次
        auto g2 = g;
        CHECK(lc2958::Solution().maxSubarrayLength(g, k)
              == lc2958::SolutionBrute().maxSubarrayLength(g2, k));
    }
}
