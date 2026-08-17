#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 2213 examples") {
    {
        std::string s = "babacc", qc = "bcb";
        std::vector<int> qi{1, 3, 3};
        CHECK(lc2213::Solution().longestRepeating(s, qc, qi)
              == std::vector<int>({3, 3, 4}));
    }
    {
        std::string s = "abyzz", qc = "aa";
        std::vector<int> qi{2, 1};
        CHECK(lc2213::Solution().longestRepeating(s, qc, qi)
              == std::vector<int>({2, 3}));
    }
}

TEST_CASE("LC 2213 single char") {
    std::string s = "a", qc = "b";
    std::vector<int> qi{0};
    CHECK(lc2213::Solution().longestRepeating(s, qc, qi) == std::vector<int>({1}));
}

TEST_CASE("LC 2213 segment tree vs brute force") {
    std::mt19937 rng(2213);
    for (int round = 0; round < 200; round++) {
        int n = 1 + rng() % 40;
        std::string s(n, 'a');
        for (auto& c : s) c = 'a' + rng() % 4;  // 小字符集，连续段多
        int q = 1 + rng() % 30;
        std::string qc(q, 'a');
        std::vector<int> qi(q);
        for (int i = 0; i < q; i++) {
            qc[i] = 'a' + rng() % 4;
            qi[i] = rng() % n;
        }
        auto s2 = s, qc2 = qc;
        auto qi2 = qi;
        CHECK(lc2213::Solution().longestRepeating(s, qc, qi)
              == lc2213::SolutionBrute().longestRepeating(s2, qc2, qi2));
    }
}
