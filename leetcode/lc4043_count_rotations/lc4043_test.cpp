#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>
#include <string>

#include "solution.hpp"

TEST_CASE("LC 4043 official examples") {
    CHECK(lc4043::Solution().countRotations("aab", 1) == 2);
    CHECK(lc4043::Solution().countRotations("abca", 0) == 1);
}

TEST_CASE("LC 4043 edge cases") {
    // 单字符：唯一旋转有 0 对相邻
    CHECK(lc4043::Solution().countRotations("a", 0) == 1);
    CHECK(lc4043::Solution().countRotations("a", 1) == 0);
    // 全相同字符：每个旋转都有 n-1 对相等
    CHECK(lc4043::Solution().countRotations("aaaa", 3) == 4);
    CHECK(lc4043::Solution().countRotations("aaaa", 4) == 0);
    // 全不同字符：任何旋转都 0 对相等
    CHECK(lc4043::Solution().countRotations("abcd", 0) == 4);
    CHECK(lc4043::Solution().countRotations("abcd", 1) == 0);
}

TEST_CASE("LC 4043 exhaustive: all binary strings len <= 12, all k") {
    for (int len = 1; len <= 12; len++) {
        for (int mask = 0; mask < (1 << len); mask++) {
            std::string s(len, 'a');
            for (int p = 0; p < len; p++)
                if ((mask >> p) & 1) s[p] = 'b';
            for (int k = 0; k <= len; k++) {
                int got = lc4043::Solution().countRotations(s, k);
                int want = lc4043::bruteForce(s, k);
                REQUIRE_MESSAGE(got == want, "s=", s, " k=", k);
            }
        }
    }
}

TEST_CASE("LC 4043 random strings over larger alphabet") {
    std::mt19937 rng(4043);
    for (int it = 0; it < 500; it++) {
        int len = 1 + rng() % 200;
        std::string s(len, 'a');
        for (auto& c : s) c = 'a' + rng() % 6;
        for (int k = 0; k <= len; k += 1 + rng() % 7) {
            int got = lc4043::Solution().countRotations(s, k);
            int want = lc4043::bruteForce(s, k);
            REQUIRE_MESSAGE(got == want, "s=", s, " k=", k);
        }
    }
}
