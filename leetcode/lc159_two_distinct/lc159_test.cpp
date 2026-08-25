#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 159 examples") {
    CHECK(lc159::Solution().lengthOfLongestSubstringTwoDistinct("eceba") == 3);
    CHECK(lc159::Solution().lengthOfLongestSubstringTwoDistinct("ccaabbb") == 5);
}

TEST_CASE("LC 159 exhaustive small alphabet (len <= 11, {a,b,c})") {
    // 穷举 {a,b,c} 上所有长度 ≤ 11 的串，共 (3^12 - 1) / 2 = 265720 个
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
            int got = lc159::Solution().lengthOfLongestSubstringTwoDistinct(s);
            int want = lc159::SolutionRef().lengthOfLongestSubstringTwoDistinct(s);
            REQUIRE_MESSAGE(got == want, "s=", s, " got=", got, " want=", want);
        }
    }
}

TEST_CASE("LC 159 random long strings (alphabet 26)") {
    std::mt19937 rng(159);
    for (int it = 0; it < 300; it++) {
        int len = rng() % 500;
        std::string s(len, 'a');
        for (auto& c : s) c = 'a' + rng() % 26;
        CHECK(lc159::Solution().lengthOfLongestSubstringTwoDistinct(s) ==
              lc159::SolutionRef().lengthOfLongestSubstringTwoDistinct(s));
    }
}

TEST_CASE("LC 159 single-char strings and pairs") {
    for (char c = 'a'; c <= 'z'; c++) {
        std::string s1(1, c);
        CHECK(lc159::Solution().lengthOfLongestSubstringTwoDistinct(s1) == 1);
        std::string s2(2, c);
        CHECK(lc159::Solution().lengthOfLongestSubstringTwoDistinct(s2) == 2);
    }
    // 全同字符长串：走完 init 的 while 后 j == size，提前返回
    std::string s(1000, 'x');
    CHECK(lc159::Solution().lengthOfLongestSubstringTwoDistinct(s) == 1000);
}
