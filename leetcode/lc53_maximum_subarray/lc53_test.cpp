#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 53 example: [-2,1,-3,4,-1,2,1,-5,4] -> 6") {
    std::vector<int> a = {-2, 1, -3, 4, -1, 2, 1, -5, 4}, b = a, c = a;
    CHECK(lc53::SolutionDP().maxSubArray(a) == 6);
    CHECK(lc53::SolutionRolling().maxSubArray(b) == 6);
    CHECK(lc53::SolutionPrefixSum().maxSubArray(c) == 6);
}

TEST_CASE("LC 53 edge cases") {
    std::vector<int> one = {1};
    CHECK(lc53::SolutionRolling().maxSubArray(one) == 1);

    std::vector<int> neg = {-3, -1, -2};
    CHECK(lc53::SolutionDP().maxSubArray(neg) == -1);
    std::vector<int> neg2 = {-2, -1};
    CHECK(lc53::SolutionPrefixSum().maxSubArray(neg2) == -1);
}

TEST_CASE("LC 53 random cross-check: three solutions agree with brute force") {
    std::mt19937 rng(5);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 20;
        std::vector<int> v(n);
        for (auto& x : v) x = (int)(rng() % 21) - 10;

        // 暴力：枚举所有子数组
        int brute = INT_MIN;
        for (int i = 0; i < n; i++) {
            int sum = 0;
            for (int j = i; j < n; j++) {
                sum += v[j];
                brute = std::max(brute, sum);
            }
        }

        auto a = v, b = v, c = v;
        CHECK(lc53::SolutionDP().maxSubArray(a) == brute);
        CHECK(lc53::SolutionRolling().maxSubArray(b) == brute);
        CHECK(lc53::SolutionPrefixSum().maxSubArray(c) == brute);
    }
}
