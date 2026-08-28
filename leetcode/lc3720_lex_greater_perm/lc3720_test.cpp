#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 3720 examples") {
    CHECK(lc3720::Solution().lexGreaterPermutation("abc", "bba") == "bca");
    CHECK(lc3720::Solution().lexGreaterPermutation("leet", "code") == "eelt");
    CHECK(lc3720::Solution().lexGreaterPermutation("baba", "bbaa") == "");
    CHECK(lc3720::SolutionV2().lexGreaterPermutation("abc", "bba") == "bca");
    CHECK(lc3720::SolutionV2().lexGreaterPermutation("leet", "code") == "eelt");
    CHECK(lc3720::SolutionV2().lexGreaterPermutation("baba", "bbaa") == "");
}

TEST_CASE("LC 3720 exhaustive pairs over {a,b,c}, len <= 6") {
    for (int len = 1; len <= 6; len++) {
        int total = 1;
        for (int t = 0; t < len; t++) total *= 3;
        for (int ms = 0; ms < total; ms++) {
            std::string s(len, 'a');
            int m = ms;
            for (int p = 0; p < len; p++) { s[p] = 'a' + m % 3; m /= 3; }
            for (int mt = 0; mt < total; mt++) {
                std::string t(len, 'a');
                m = mt;
                for (int p = 0; p < len; p++) { t[p] = 'a' + m % 3; m /= 3; }
                std::string want = lc3720::SolutionBrute().lexGreaterPermutation(s, t);
                std::string g1 = lc3720::Solution().lexGreaterPermutation(s, t);
                std::string g2 = lc3720::SolutionV2().lexGreaterPermutation(s, t);
                REQUIRE_MESSAGE(g1 == want, "v1 s=", s, " t=", t,
                                " got=", g1, " want=", want);
                REQUIRE_MESSAGE(g2 == want, "v2 s=", s, " t=", t,
                                " got=", g2, " want=", want);
            }
        }
    }
}

TEST_CASE("LC 3720 random pairs, alphabet 26, len <= 9") {
    std::mt19937 rng(3720);
    for (int it = 0; it < 300; it++) {
        int len = 1 + rng() % 9;
        std::string s(len, 'a'), t(len, 'a');
        for (auto& c : s) c = 'a' + rng() % 26;
        for (auto& c : t) c = 'a' + rng() % 26;
        std::string want = lc3720::SolutionBrute().lexGreaterPermutation(s, t);
        CHECK(lc3720::Solution().lexGreaterPermutation(s, t) == want);
        CHECK(lc3720::SolutionV2().lexGreaterPermutation(s, t) == want);
        CHECK(lc3720::SolutionV3().lexGreaterPermutation(s, t) == want);
    }
}

TEST_CASE("LC 3720 v3 single-array vs brute (exhaustive len <= 6)") {
    for (int len = 1; len <= 6; len++) {
        int total = 1;
        for (int t = 0; t < len; t++) total *= 3;
        for (int ms = 0; ms < total; ms++) {
            std::string s(len, 'a');
            int m = ms;
            for (int p = 0; p < len; p++) { s[p] = 'a' + m % 3; m /= 3; }
            for (int mt = 0; mt < total; mt++) {
                std::string t(len, 'a');
                m = mt;
                for (int p = 0; p < len; p++) { t[p] = 'a' + m % 3; m /= 3; }
                std::string want = lc3720::SolutionBrute().lexGreaterPermutation(s, t);
                std::string g3 = lc3720::SolutionV3().lexGreaterPermutation(s, t);
                REQUIRE_MESSAGE(g3 == want, "v3 s=", s, " t=", t,
                                " got=", g3, " want=", want);
            }
        }
    }
}
