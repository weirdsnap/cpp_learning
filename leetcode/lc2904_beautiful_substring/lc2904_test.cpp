#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 2904 examples") {
    CHECK(lc2904::Solution().shortestBeautifulSubstring("100011001", 3) == "11001");
    CHECK(lc2904::Solution().shortestBeautifulSubstring("1011", 2) == "11");
    CHECK(lc2904::Solution().shortestBeautifulSubstring("000", 1) == "");
}

TEST_CASE("LC 2904 exhaustive binary strings (len <= 12)") {
    for (int len = 1; len <= 12; len++) {
        int total = 1 << len;
        for (int mask = 0; mask < total; mask++) {
            std::string s(len, '0');
            for (int p = 0; p < len; p++)
                if (mask >> p & 1) s[p] = '1';
            for (int k = 1; k <= len + 1; k++) {   // k > len 也要覆盖（无解分支）
                std::string got = lc2904::Solution().shortestBeautifulSubstring(s, k);
                std::string want = lc2904::SolutionRef().shortestBeautifulSubstring(s, k);
                REQUIRE_MESSAGE(got == want,
                                "s=", s, " k=", k, " got=", got, " want=", want);
            }
        }
    }
}

TEST_CASE("LC 2904 random long strings") {
    std::mt19937 rng(2904);
    for (int it = 0; it < 300; it++) {
        int len = 1 + rng() % 100;
        std::string s(len, '0');
        for (auto& c : s)
            if (rng() % 2) c = '1';
        int k = 1 + rng() % (len + 1);
        CHECK(lc2904::Solution().shortestBeautifulSubstring(s, k) ==
              lc2904::SolutionRef().shortestBeautifulSubstring(s, k));
    }
}
