#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdlib>
#include <random>

#include "solution.hpp"

static int brute(const std::vector<int>& nums) {
    int best = 0;
    for (size_t i = 0; i < nums.size(); i++) {
        int sum = 0;
        for (size_t j = i; j < nums.size(); j++) {
            sum += nums[j];
            best = std::max(best, std::abs(sum));
        }
    }
    return best;
}

TEST_CASE("LC 1749 examples") {
    std::vector<int> a = {1, -3, 2, 3, -4};
    CHECK(lc1749::Solution().maxAbsoluteSum(a) == 5);
    std::vector<int> b = {2, -5, 1, -4, 3, -2};
    CHECK(lc1749::Solution().maxAbsoluteSum(b) == 8);
}

TEST_CASE("LC 1749 edge cases") {
    std::vector<int> neg = {-7};
    CHECK(lc1749::Solution().maxAbsoluteSum(neg) == 7);
    std::vector<int> zero = {0, 0};
    CHECK(lc1749::Solution().maxAbsoluteSum(zero) == 0);
}

TEST_CASE("LC 1749 random cross-check with brute force") {
    std::mt19937 rng(12);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 12;
        std::vector<int> v(n);
        for (auto& x : v) x = (int)(rng() % 21) - 10;
        CHECK(lc1749::Solution().maxAbsoluteSum(v) == brute(v));
    }
}
