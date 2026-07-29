#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <climits>
#include <random>

#include "solution.hpp"

static int brute(const std::vector<int>& nums) {
    int best = INT_MIN;
    for (size_t i = 0; i < nums.size(); i++) {
        long long p = 1;
        for (size_t j = i; j < nums.size(); j++) {
            p *= nums[j];
            if (p > best) best = (int)p;
        }
    }
    return best;
}

TEST_CASE("LC 152 examples") {
    std::vector<int> a = {2, 3, -2, 4};
    CHECK(lc152::Solution().maxProduct(a) == 6);
    std::vector<int> b = {-2, 0, -1};
    CHECK(lc152::Solution().maxProduct(b) == 0);
}

TEST_CASE("LC 152 edge cases") {
    std::vector<int> neg = {-2};
    CHECK(lc152::Solution().maxProduct(neg) == -2);
    std::vector<int> zero = {0, 2};
    CHECK(lc152::Solution().maxProduct(zero) == 2);
}

TEST_CASE("LC 152 random cross-check with brute force") {
    std::mt19937 rng(8);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 10;
        std::vector<int> v(n);
        for (auto& x : v) x = (int)(rng() % 7) - 3;
        CHECK(lc152::Solution().maxProduct(v) == brute(v));
    }
}

TEST_CASE("std::max/min initializer_list overload (C++11)") {
    CHECK(std::max({3, 1, 4, 1, 5}) == 5);
    CHECK(std::min({3, 1, 4, 1, 5}) == 1);
    // 列表版本返回值，双参数版本返回 const 引用
    static_assert(std::is_same_v<decltype(std::max({1, 2})), int>);
}
