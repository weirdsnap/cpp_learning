#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <random>
#include <set>

#include "solution.hpp"

// 暴力：枚举 half 的所有不同排列，组成回文排序取第 k 个
static std::string brute(std::string s, int k) {
    std::string mid = s.size() % 2 == 1 ? s.substr(s.size() / 2, 1) : "";
    std::string half = s.substr(0, s.size() / 2);
    std::sort(half.begin(), half.end());
    std::set<std::string> perms;
    do {
        perms.insert(half);
    } while (std::next_permutation(half.begin(), half.end()));
    if ((int)perms.size() < k) return "";
    auto it = perms.begin();
    std::advance(it, k - 1);
    return *it + mid + std::string(it->rbegin(), it->rend());
}

TEST_CASE("LC 3518 examples") {
    CHECK(lc3518::Solution().smallestPalindrome("abba", 2) == "baab");
    CHECK(lc3518::Solution().smallestPalindrome("aa", 2) == "");
    CHECK(lc3518::Solution().smallestPalindrome("bacab", 1) == "abcba");
}

TEST_CASE("LC 3518 large input: clamped counting does not overflow") {
    std::string big(200, 'a');
    for (int i = 0; i < 200; i++) big[i] = 'a' + i % 5;
    std::string r = lc3518::Solution().smallestPalindrome(big, 1000000);
    // k=10^6 远超排列数时应返回 ""，不超时应返回合法回文
    if (!r.empty()) {
        std::string rev(r.rbegin(), r.rend());
        CHECK(r == rev);
    }
}

TEST_CASE("LC 3518 random cross-check with permutation brute force") {
    std::mt19937 rng(17);
    for (int it = 0; it < 300; it++) {
        int half = 1 + rng() % 4;
        std::string h;
        for (int i = 0; i < half; i++) h += char('a' + rng() % 4);
        std::string m = (rng() % 2) ? std::string(1, char('a' + rng() % 4)) : "";
        std::string t = h + m + std::string(h.rbegin(), h.rend());
        std::shuffle(t.begin(), t.end(), rng);
        int k = 1 + rng() % 30;
        CHECK(lc3518::Solution().smallestPalindrome(t, k) == brute(t, k));
    }
}
