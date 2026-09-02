#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 266 examples") {
    CHECK(lc266::Solution().canPermutePalindrome("code") == false);
    CHECK(lc266::Solution().canPermutePalindrome("aab") == true);
    CHECK(lc266::Solution().canPermutePalindrome("carerac") == true);
}

TEST_CASE("LC 266 exhaustive strings over {a,b,c}, len <= 8") {
    for (int len = 1; len <= 8; len++) {
        int total = 1;
        for (int t = 0; t < len; t++) total *= 3;
        for (int mask = 0; mask < total; mask++) {
            std::string s(len, 'a');
            int m = mask;
            for (int p = 0; p < len; p++) { s[p] = 'a' + m % 3; m /= 3; }
            bool got = lc266::Solution().canPermutePalindrome(s);
            bool want = lc266::bruteForce(s);
            REQUIRE_MESSAGE(got == want, "s=", s, " got=", got, " want=", want);
        }
    }
}

TEST_CASE("LC 266 random strings: constructive verification when true") {
    // 字符集压小一些，让 true 答案频繁出现；true 时实际构造回文排列验证
    std::mt19937 rng(266);
    for (int it = 0; it < 500; it++) {
        int len = 1 + rng() % 200;
        int alpha = 1 + rng() % 5;   // 1~5 种字符
        std::string s(len, 'a');
        for (auto& c : s) c = 'a' + rng() % alpha;
        if (!lc266::Solution().canPermutePalindrome(s)) continue;
        // 从计数构造回文：前半 + 可选中心 + 前半反转
        int cnt[26] = {0};
        for (char c : s) cnt[c - 'a']++;
        std::string half, mid;
        for (int i = 0; i < 26; i++) {
            if (cnt[i] & 1) mid += char('a' + i);
            half += std::string(cnt[i] / 2, 'a' + i);
        }
        std::string pal = half + mid + std::string(half.rbegin(), half.rend());
        // 是 s 的排列且是回文
        auto sortedA = pal, sortedB = s;
        std::sort(sortedA.begin(), sortedA.end());
        std::sort(sortedB.begin(), sortedB.end());
        REQUIRE(sortedA == sortedB);
        CHECK(pal == std::string(pal.rbegin(), pal.rend()));
    }
}
