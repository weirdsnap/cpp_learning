#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <random>

#include "solution.hpp"

static bool isPal(const std::string& s) {
    for (size_t i = 0, j = s.size() - 1; i < j; i++, j--)
        if (s[i] != s[j]) return false;
    return true;
}

// 暴力：全排列找最小回文
static std::string brute(std::string s) {
    std::sort(s.begin(), s.end());
    std::string best;
    do {
        if (isPal(s) && (best.empty() || s < best)) best = s;
    } while (std::next_permutation(s.begin(), s.end()));
    return best;
}

TEST_CASE("LC 3517 examples") {
    CHECK(lc3517::SolutionMap().smallestPalindrome("z") == "z");
    CHECK(lc3517::SolutionMap().smallestPalindrome("babab") == "abbba");
    CHECK(lc3517::SolutionArray().smallestPalindrome("babab") == "abbba");
    CHECK(lc3517::SolutionArray().smallestPalindrome("daccad") == "acddca");
}

TEST_CASE("LC 3517 random cross-check with permutation brute force") {
    std::mt19937 rng(7);
    for (int it = 0; it < 200; it++) {
        int half = 1 + rng() % 3;
        std::string h;
        for (int i = 0; i < half; i++) h += char('a' + rng() % 4);
        std::string m = (rng() % 2) ? std::string(1, char('a' + rng() % 4)) : "";
        std::string t = h + m + std::string(h.rbegin(), h.rend());
        std::shuffle(t.begin(), t.end(), rng);

        std::string expect = brute(t);
        CHECK(lc3517::SolutionMap().smallestPalindrome(t) == expect);
        CHECK(lc3517::SolutionArray().smallestPalindrome(t) == expect);
    }
}
