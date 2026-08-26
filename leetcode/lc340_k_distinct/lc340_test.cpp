#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 340 examples") {
    CHECK(lc340::Solution().lengthOfLongestSubstringKDistinct("eceba", 2) == 3);
    CHECK(lc340::Solution().lengthOfLongestSubstringKDistinct("aa", 1) == 2);
}

TEST_CASE("LC 340 edge cases") {
    // k = 0：任何非空窗口都超标 → 0
    CHECK(lc340::Solution().lengthOfLongestSubstringKDistinct("abc", 0) == 0);
    // 空串
    CHECK(lc340::Solution().lengthOfLongestSubstringKDistinct("", 2) == 0);
    // k 大于总种类数 → 整个串
    CHECK(lc340::Solution().lengthOfLongestSubstringKDistinct("abaccc", 4) == 6);
}

TEST_CASE("LC 340 exhaustive ternary strings (len <= 11, k = 0..4)") {
    for (int len = 0; len <= 11; len++) {
        int total = 1;
        for (int t = 0; t < len; t++) total *= 3;
        for (int mask = 0; mask < total; mask++) {
            std::string s(len, 'a');
            int m = mask;
            for (int p = 0; p < len; p++) {
                s[p] = 'a' + m % 3;
                m /= 3;
            }
            for (int k = 0; k <= 4; k++) {
                int got = lc340::Solution().lengthOfLongestSubstringKDistinct(s, k);
                int want = lc340::SolutionBrute().lengthOfLongestSubstringKDistinct(s, k);
                REQUIRE_MESSAGE(got == want,
                                "s=", s, " k=", k, " got=", got, " want=", want);
            }
        }
    }
}

TEST_CASE("LC 340 random long strings (alphabet 26)") {
    std::mt19937 rng(340);
    for (int it = 0; it < 200; it++) {
        int len = rng() % 300;
        std::string s(len, 'a');
        for (auto& c : s) c = 'a' + rng() % 26;
        int k = rng() % 28;   // 覆盖 k > 种类数 的情形
        CHECK(lc340::Solution().lengthOfLongestSubstringKDistinct(s, k) ==
              lc340::SolutionBrute().lengthOfLongestSubstringKDistinct(s, k));
    }
}
