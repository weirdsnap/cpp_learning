#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 487 examples") {
    std::vector<int> a{1, 0, 1, 1, 0};
    CHECK(lc487::Solution().findMaxConsecutiveOnes(a) == 4);
    std::vector<int> b{1, 0, 1, 1, 0, 1};
    CHECK(lc487::Solution().findMaxConsecutiveOnes(b) == 4);
}

TEST_CASE("LC 487 edge cases") {
    std::vector<int> all1(100, 1);
    CHECK(lc487::Solution().findMaxConsecutiveOnes(all1) == 100);  // 全 1，不能虚报 101
    std::vector<int> all0(100, 0);
    CHECK(lc487::Solution().findMaxConsecutiveOnes(all0) == 1);    // 全 0，翻转一个
    std::vector<int> one{1};
    CHECK(lc487::Solution().findMaxConsecutiveOnes(one) == 1);
    std::vector<int> zero{0};
    CHECK(lc487::Solution().findMaxConsecutiveOnes(zero) == 1);
}

TEST_CASE("LC 487 exhaustive binary arrays (len <= 12)") {
    for (int len = 1; len <= 12; len++) {
        int total = 1 << len;
        for (int mask = 0; mask < total; mask++) {
            std::vector<int> nums(len);
            for (int p = 0; p < len; p++)
                nums[p] = (mask >> p) & 1;
            int got = lc487::Solution().findMaxConsecutiveOnes(nums);
            int want = lc487::SolutionSW().findMaxConsecutiveOnes(nums);
            REQUIRE_MESSAGE(got == want, "mask=", mask, " len=", len,
                            " got=", got, " want=", want);
        }
    }
}

TEST_CASE("LC 487 random long arrays") {
    std::mt19937 rng(487);
    for (int it = 0; it < 300; it++) {
        int len = 1 + rng() % 1000;
        std::vector<int> nums(len);
        for (auto& x : nums) x = rng() % 2;
        CHECK(lc487::Solution().findMaxConsecutiveOnes(nums) ==
              lc487::SolutionSW().findMaxConsecutiveOnes(nums));
    }
}
