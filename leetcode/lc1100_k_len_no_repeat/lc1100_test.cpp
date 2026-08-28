#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 1100 examples") {
    CHECK(lc1100::Solution().numKLenSubstrNoRepeats("havefunonleetcode", 5) == 6);
    CHECK(lc1100::Solution().numKLenSubstrNoRepeats("home", 5) == 0);
}

TEST_CASE("LC 1100 exhaustive over {a,b,c}, len <= 11, k = 1..len+2") {
    for (int len = 1; len <= 11; len++) {
        int total = 1;
        for (int t = 0; t < len; t++) total *= 3;
        for (int mask = 0; mask < total; mask++) {
            std::string s(len, 'a');
            int m = mask;
            for (int p = 0; p < len; p++) { s[p] = 'a' + m % 3; m /= 3; }
            for (int k = 1; k <= len + 2; k++) {
                int got = lc1100::Solution().numKLenSubstrNoRepeats(s, k);
                int want = lc1100::SolutionRef().numKLenSubstrNoRepeats(s, k);
                REQUIRE_MESSAGE(got == want, "s=", s, " k=", k,
                                " got=", got, " want=", want);
            }
        }
    }
}

TEST_CASE("LC 1100 random, alphabet 26, len <= 500") {
    std::mt19937 rng(1100);
    for (int it = 0; it < 300; it++) {
        int len = 1 + rng() % 500;
        std::string s(len, 'a');
        for (auto& c : s) c = 'a' + rng() % 26;
        int k = 1 + rng() % 30;   // 覆盖 k > 26（必无解）与 k > len
        int want = lc1100::SolutionRef().numKLenSubstrNoRepeats(s, k);
        CHECK(lc1100::Solution().numKLenSubstrNoRepeats(s, k) == want);
        CHECK(lc1100::SolutionV2().numKLenSubstrNoRepeats(s, k) == want);
    }
}

TEST_CASE("LC 1100 v2 dup-counter exhaustive over {a,b,c}, len <= 11") {
    for (int len = 1; len <= 11; len++) {
        int total = 1;
        for (int t = 0; t < len; t++) total *= 3;
        for (int mask = 0; mask < total; mask++) {
            std::string s(len, 'a');
            int m = mask;
            for (int p = 0; p < len; p++) { s[p] = 'a' + m % 3; m /= 3; }
            for (int k = 1; k <= len + 2; k++) {
                CHECK(lc1100::SolutionV2().numKLenSubstrNoRepeats(s, k) ==
                      lc1100::SolutionRef().numKLenSubstrNoRepeats(s, k));
                CHECK(lc1100::SolutionV3().numKLenSubstrNoRepeats(s, k) ==
                      lc1100::SolutionRef().numKLenSubstrNoRepeats(s, k));
            }
        }
    }
}
