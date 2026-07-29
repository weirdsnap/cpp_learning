#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <random>

#include "solution.hpp"

static int brute(const std::string& s, const std::string& chars, std::vector<int>& vals) {
    int values[26];
    for (int i = 0; i < 26; i++) values[i] = i + 1;
    for (int i = 0; i < (int)chars.size(); i++) values[chars[i] - 'a'] = vals[i];
    int best = 0;  // 空子串
    for (size_t i = 0; i < s.size(); i++) {
        int sum = 0;
        for (size_t j = i; j < s.size(); j++) {
            sum += values[s[j] - 'a'];
            best = std::max(best, sum);
        }
    }
    return best;
}

TEST_CASE("LC 2606 examples") {
    std::vector<int> v1 = {-1000};
    CHECK(lc2606::Solution().maximumCostSubstring("adaa", "d", v1) == 2);
    std::vector<int> v2 = {-1, -1, -1};
    CHECK(lc2606::Solution().maximumCostSubstring("abc", "abc", v2) == 0);
}

TEST_CASE("LC 2606 random cross-check with brute force") {
    std::mt19937 rng(3);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 15;
        std::string t(n, 'a');
        for (auto& c : t) c = char('a' + rng() % 6);
        std::string cs;
        std::vector<int> vs;
        for (int i = 0; i < 3; i++) {
            char c = char('a' + rng() % 6);
            if (cs.find(c) == std::string::npos) {
                cs += c;
                vs.push_back((int)(rng() % 21) - 10);
            }
        }
        CHECK(lc2606::Solution().maximumCostSubstring(t, cs, vs) == brute(t, cs, vs));
    }
}
