#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>
#include <utility>
#include <vector>

#include "solution.hpp"

// 参照实现：pair<char,int> 降序排序，同一公式
static int pairVersion(const std::string& word) {
    int cnt[26] = {0};
    for (char c : word) cnt[c - 'a']++;
    std::vector<std::pair<char, int>> v;
    for (int i = 0; i < 26; i++)
        if (cnt[i]) v.push_back({'a' + i, cnt[i]});
    std::sort(v.begin(), v.end(),
              [](auto& a, auto& b) { return a.second > b.second; });
    int res = 0;
    for (int i = 0; i < (int)v.size(); i++) res += v[i].second * (i / 8 + 1);
    return res;
}

TEST_CASE("LC 3014 examples") {
    CHECK(lc3014::Solution().minimumPushes("abcde") == 5);
    CHECK(lc3014::Solution().minimumPushes("xyzxyzxyzxyz") == 12);
    CHECK(lc3014::Solution().minimumPushes("aabbccddeeffgghhiiiiii") == 24);
}

TEST_CASE("LC 3014 random cross-check with pair-based reference") {
    std::mt19937 rng(23);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 40;
        std::string w(n, 'a');
        for (auto& c : w) c = char('a' + rng() % 26);
        CHECK(lc3014::Solution().minimumPushes(w) == pairVersion(w));
    }
}
