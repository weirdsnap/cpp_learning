#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 2996 examples") {
    std::vector<int> a{1, 2, 3, 2, 5};
    std::vector<int> b{3, 4, 5, 1, 12, 14, 13};
    CHECK(lc2996::Solution().missingInteger(a) == 6);
    CHECK(lc2996::Solution().missingInteger(b) == 15);
}

TEST_CASE("LC 2996 edge cases") {
    // 前缀只有 nums[0]，且 nums[0] 不在数组中重复... 但它自己就在数组里，所以 +1
    std::vector<int> single{5};
    CHECK(lc2996::Solution().missingInteger(single) == 6);
    // 前缀和本身不在数组中
    std::vector<int> not_present{1, 2, 7};
    CHECK(lc2996::Solution().missingInteger(not_present) == 3);
    // 前缀和之后连续命中多次
    std::vector<int> chain{2, 3, 4, 9, 10, 11, 12};
    CHECK(lc2996::Solution().missingInteger(chain) == 13);
}

TEST_CASE("LC 2996 linear scan vs hash set randomized") {
    std::mt19937 rng(2996);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 50;
        std::vector<int> g(n);
        for (auto& x : g) x = 1 + rng() % 50;
        auto g2 = g;
        CHECK(lc2996::Solution().missingInteger(g) == lc2996::SolutionSet().missingInteger(g2));
    }
}
