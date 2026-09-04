#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

using VI = std::vector<int>;

TEST_CASE("LC 484 examples") {
    CHECK(lc484::Solution().findPermutation("I") == VI({1, 2}));
    CHECK(lc484::Solution().findPermutation("DI") == VI({2, 1, 3}));
    CHECK(lc484::Solution().findPermutation("DDI") == VI({3, 2, 1, 4}));
    CHECK(lc484::Solution().findPermutation("D") == VI({2, 1}));
    CHECK(lc484::SolutionStack().findPermutation("I") == VI({1, 2}));
    CHECK(lc484::SolutionStack().findPermutation("DI") == VI({2, 1, 3}));
    CHECK(lc484::SolutionStack().findPermutation("DDI") == VI({3, 2, 1, 4}));
    CHECK(lc484::SolutionStack().findPermutation("D") == VI({2, 1}));
}

TEST_CASE("LC 484 exhaustive: all patterns {I,D} len <= 6") {
    for (int len = 1; len <= 6; len++) {
        for (int mask = 0; mask < (1 << len); mask++) {
            std::string s(len, 'I');
            for (int p = 0; p < len; p++)
                if ((mask >> p) & 1) s[p] = 'D';
            auto want = lc484::bruteForce(s);
            REQUIRE_MESSAGE(lc484::Solution().findPermutation(s) == want, "s=", s);
            REQUIRE_MESSAGE(lc484::SolutionStack().findPermutation(s) == want, "s=", s);
        }
    }
}

TEST_CASE("LC 484 random long patterns: property check") {
    // n 大没法暴力，校验性质：是 [1..n] 的排列 + 满足 s + 字典序局部最小性难验，验前两条
    std::mt19937 rng(484);
    for (int it = 0; it < 300; it++) {
        int len = 1 + rng() % 500;
        std::string s(len, 'I');
        for (auto& c : s) c = (rng() % 2) ? 'D' : 'I';
        auto got = lc484::Solution().findPermutation(s);
        auto gotStack = lc484::SolutionStack().findPermutation(s);
        REQUIRE(got == gotStack);  // 两写法等价
        auto sorted = got;
        std::sort(sorted.begin(), sorted.end());
        bool isPerm = true;
        for (int i = 0; i < (int)got.size(); i++)
            if (sorted[i] != i + 1) isPerm = false;
        REQUIRE(isPerm);
        for (int i = 0; i < len; i++) {
            if (s[i] == 'I') REQUIRE(got[i] < got[i + 1]);
            else REQUIRE(got[i] > got[i + 1]);
        }
    }
}
