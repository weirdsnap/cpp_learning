#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 3734 examples") {
    CHECK(lc3734::Solution().lexPalindromicPermutation("baba", "abba") == "baab");
    CHECK(lc3734::Solution().lexPalindromicPermutation("baba", "bbaa") == "");
    CHECK(lc3734::Solution().lexPalindromicPermutation("abc", "abb") == "");
    CHECK(lc3734::Solution().lexPalindromicPermutation("aac", "abb") == "aca");
}

TEST_CASE("LC 3734 exhaustive pairs over {a,b,c}, len <= 7") {
    for (int len = 1; len <= 7; len++) {
        int total = 1;
        for (int t = 0; t < len; t++) total *= 3;
        for (int ms = 0; ms < total; ms++) {
            std::string s(len, 'a');
            int m = ms;
            for (int p = 0; p < len; p++) { s[p] = 'a' + m % 3; m /= 3; }
            for (int mt = 0; mt < total; mt++) {
                std::string tg(len, 'a');
                m = mt;
                for (int p = 0; p < len; p++) { tg[p] = 'a' + m % 3; m /= 3; }
                std::string got = lc3734::Solution().lexPalindromicPermutation(s, tg);
                std::string want = lc3734::SolutionBrute().lexPalindromicPermutation(s, tg);
                REQUIRE_MESSAGE(got == want, "s=", s, " target=", tg,
                                " got=", got, " want=", want);
            }
        }
    }
}

TEST_CASE("LC 3734 random pairs, alphabet 26, len <= 13") {
    std::mt19937 rng(3734);
    for (int it = 0; it < 400; it++) {
        int len = 1 + rng() % 13;
        std::string s(len, 'a'), tg(len, 'a');
        for (auto& c : s) c = 'a' + rng() % 26;
        for (auto& c : tg) c = 'a' + rng() % 26;
        CHECK(lc3734::Solution().lexPalindromicPermutation(s, tg) ==
              lc3734::SolutionBrute().lexPalindromicPermutation(s, tg));
    }
}
