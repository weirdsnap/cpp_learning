#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <climits>
#include <random>

#include "solution.hpp"

using VI = std::vector<int>;
using VVI = std::vector<VI>;

TEST_CASE("LC 163 examples") {
    VI a{0, 1, 3, 50, 75};
    VVI want{{2, 2}, {4, 49}, {51, 74}, {76, 99}};
    CHECK(lc163::Solution().findMissingRanges(a, 0, 99) == want);
    VI b{-1};
    VVI want2{};
    CHECK(lc163::Solution().findMissingRanges(b, -1, -1) == want2);
}

TEST_CASE("LC 163 edge: empty nums / full coverage / int boundaries") {
    VI e{};
    VVI w1{{5, 9}};
    CHECK(lc163::Solution().findMissingRanges(e, 5, 9) == w1);
    VI full{1, 2, 3};
    VVI w2{};
    CHECK(lc163::Solution().findMissingRanges(full, 1, 3) == w2);
    // 约束边界：-1e9 ~ 1e9，nums 含端点
    VI c{(int)-1e9, 0, (int)1e9};
    VVI w3{{(int)-1e9 + 1, -1}, {1, (int)1e9 - 1}};
    CHECK(lc163::Solution().findMissingRanges(c, (int)-1e9, (int)1e9) == w3);
    // INT_MIN / INT_MAX 极端（num+1 / start 溢出测试）
    VI d{INT_MIN, INT_MAX};
    VVI w4{{INT_MIN + 1, INT_MAX - 1}};
    CHECK(lc163::Solution().findMissingRanges(d, INT_MIN, INT_MAX) == w4);
}

TEST_CASE("LC 163 exhaustive: lower,upper in [-4,4], all subsets of in-range ints") {
    for (int lower = -4; lower <= 4; lower++) {
        for (int upper = lower; upper <= 4; upper++) {
            int sz = upper - lower + 1;  // 1..9
            int total = 1 << sz;
            for (int mask = 0; mask < total; mask++) {
                VI nums;
                for (int b = 0; b < sz; b++)
                    if ((mask >> b) & 1) nums.push_back(lower + b);
                auto got = lc163::Solution().findMissingRanges(nums, lower, upper);
                REQUIRE_MESSAGE(lc163::validRanges(nums, lower, upper, got),
                                "lower=", lower, " upper=", upper, " mask=", mask);
            }
        }
    }
}

TEST_CASE("LC 163 random wide ranges") {
    std::mt19937 rng(163);
    for (int it = 0; it < 400; it++) {
        long long lower = (long long)rng() % 2000000001LL - 1000000000LL;
        long long span = rng() % 2000;
        long long upper = std::min(lower + span, 1000000000LL);
        int cnt = rng() % 50;
        VI nums;
        std::vector<char> used(upper - lower + 1, 0);
        for (int t = 0; t < cnt; t++) {
            long long v = lower + rng() % (upper - lower + 1);
            if (!used[v - lower]) { used[v - lower] = 1; nums.push_back((int)v); }
        }
        std::sort(nums.begin(), nums.end());
        CHECK(lc163::validRanges(nums, (int)lower, (int)upper,
                                 lc163::Solution().findMissingRanges(nums, (int)lower, (int)upper)));
    }
}
