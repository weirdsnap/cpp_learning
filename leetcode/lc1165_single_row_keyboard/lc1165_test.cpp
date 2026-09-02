#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <random>

#include "solution.hpp"

TEST_CASE("LC 1165 examples") {
    CHECK(lc1165::Solution().calculateTime("abcdefghijklmnopqrstuvwxyz", "cba") == 4);
    CHECK(lc1165::Solution().calculateTime("pqrstuvwxyzabcdefghijklmno", "leetcode") == 73);
}

TEST_CASE("LC 1165 fixed keyboards x exhaustive words over {a,b,c}, len <= 6") {
    std::string id = "abcdefghijklmnopqrstuvwxyz";
    std::string rev(id.rbegin(), id.rend());
    std::string rot = "pqrstuvwxyzabcdefghijklmno";
    for (const std::string& kb : {id, rev, rot}) {
        for (int len = 1; len <= 6; len++) {
            int total = 1;
            for (int t = 0; t < len; t++) total *= 3;
            for (int mask = 0; mask < total; mask++) {
                std::string w(len, 'a');
                int m = mask;
                for (int p = 0; p < len; p++) { w[p] = 'a' + m % 3; m /= 3; }
                REQUIRE(lc1165::Solution().calculateTime(kb, w) ==
                        lc1165::bruteForce(kb, w));
            }
        }
    }
}

TEST_CASE("LC 1165 random keyboards and words") {
    std::mt19937 rng(1165);
    for (int it = 0; it < 400; it++) {
        std::string kb = "abcdefghijklmnopqrstuvwxyz";
        std::shuffle(kb.begin(), kb.end(), rng);
        int len = 1 + rng() % 100;
        std::string w(len, 'a');
        for (auto& c : w) c = 'a' + rng() % 26;
        CHECK(lc1165::Solution().calculateTime(kb, w) ==
              lc1165::bruteForce(kb, w));
    }
}
