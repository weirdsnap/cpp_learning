#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <random>

#include "solution.hpp"

using VS = std::vector<std::string>;

TEST_CASE("LC 616 examples") {
    VS w1{"abc", "123"};
    CHECK(lc616::Solution().addBoldTag("abcxyz123", w1) == "<b>abc</b>xyz<b>123</b>");
    VS w2{"aa", "b"};
    CHECK(lc616::Solution().addBoldTag("aaabbb", w2) == "<b>aaabbb</b>");
    VS w3{"aaa", "aab", "bc"};
    CHECK(lc616::Solution().addBoldTag("aaabbcc", w3) == "<b>aaabbc</b>c");
}

TEST_CASE("LC 616 exhaustive: s over {a,b} len<=8, word subsets (pool 14) size<=3") {
    VS pool;
    for (int len = 1; len <= 3; len++) {
        for (int mask = 0; mask < (1 << len); mask++) {
            std::string w(len, 'a');
            for (int p = 0; p < len; p++) w[p] = 'a' + ((mask >> p) & 1);
            pool.push_back(w);
        }
    }
    // 词表：空集 + 全部 1/2/3 元子集
    std::vector<VS> wordSets{{}};
    for (size_t i = 0; i < pool.size(); i++) {
        wordSets.push_back({pool[i]});
        for (size_t j = i + 1; j < pool.size(); j++) {
            wordSets.push_back({pool[i], pool[j]});
            for (size_t k = j + 1; k < pool.size(); k++) {
                wordSets.push_back({pool[i], pool[j], pool[k]});
            }
        }
    }
    for (int len = 1; len <= 8; len++) {
        for (int mask = 0; mask < (1 << len); mask++) {
            std::string s(len, 'a');
            for (int p = 0; p < len; p++) s[p] = 'a' + ((mask >> p) & 1);
            for (auto& ws : wordSets) {
                auto words = ws;
                REQUIRE(lc616::Solution().addBoldTag(s, words) ==
                        lc616::bruteForce(s, words));
            }
        }
    }
}

TEST_CASE("LC 616 random: letters+digits, longer s and word lists") {
    std::mt19937 rng(616);
    for (int it = 0; it < 400; it++) {
        int n = 1 + rng() % 60;
        std::string s(n, 'a');
        for (auto& c : s) {
            int r = rng() % 8;
            c = r < 6 ? char('a' + rng() % 4) : char('0' + rng() % 3);
        }
        VS words;
        int nw = rng() % 8;
        for (int t = 0; t < nw; t++) {
            int len = 1 + rng() % 6;
            std::string w(len, 'a');
            for (auto& c : w) {
                int r = rng() % 8;
                c = r < 6 ? char('a' + rng() % 4) : char('0' + rng() % 3);
            }
            words.push_back(w);
        }
        CHECK(lc616::Solution().addBoldTag(s, words) == lc616::bruteForce(s, words));
    }
}
