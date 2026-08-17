#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 3090 examples") {
    CHECK(lc3090::Solution().maximumLengthSubstring("bcbbbcba") == 4);
    CHECK(lc3090::Solution().maximumLengthSubstring("aaaa") == 2);
}

TEST_CASE("LC 3090 edge cases") {
    CHECK(lc3090::Solution().maximumLengthSubstring("a") == 1);
    CHECK(lc3090::Solution().maximumLengthSubstring("ab") == 2);
    CHECK(lc3090::Solution().maximumLengthSubstring("aaab") == 3);  // "aab"
    CHECK(lc3090::Solution().maximumLengthSubstring("baaa") == 3);  // "baa"
}

TEST_CASE("LC 3090 sliding window vs brute force") {
    std::mt19937 rng(3090);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 30;
        std::string s(n, 'a');
        for (auto& c : s) c = 'a' + rng() % 6;  // 小字符集，容易撞出 >2 次
        CHECK(lc3090::Solution().maximumLengthSubstring(s)
              == lc3090::SolutionBrute().maximumLengthSubstring(s));
    }
}
