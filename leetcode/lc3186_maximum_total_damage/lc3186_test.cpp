#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <map>
#include <random>

#include "solution.hpp"

// 暴力：枚举不同伤害值的子集，检查两两差 > 2
static long long brute(std::vector<int>& power) {
    std::map<int, long long> cnt;
    for (int p : power) cnt[p] += p;
    std::vector<std::pair<int, long long>> items(cnt.begin(), cnt.end());
    int m = items.size();
    long long best = 0;
    for (int mask = 0; mask < (1 << m); mask++) {
        long long sum = 0;
        int prev = -100;
        bool ok = true;
        for (int i = 0; i < m && ok; i++) {
            if (mask & (1 << i)) {
                if (items[i].first - prev <= 2) ok = false;
                else {
                    sum += items[i].second;
                    prev = items[i].first;
                }
            }
        }
        if (ok) best = std::max(best, sum);
    }
    return best;
}

TEST_CASE("LC 3186 examples") {
    std::vector<int> a = {1, 1, 3, 4};
    CHECK(lc3186::SolutionTwoPointers().maximumTotalDamage(a) == 6);
    std::vector<int> b = {7, 1, 6, 6};
    CHECK(lc3186::SolutionBinarySearch().maximumTotalDamage(b) == 13);
    std::vector<int> c = {5, 9, 2, 10, 2, 7, 10, 9, 3, 8};
    CHECK(lc3186::SolutionRollingO1().maximumTotalDamage(c) == 31);
}

TEST_CASE("LC 3186 edge cases") {
    std::vector<int> one = {1};
    CHECK(lc3186::SolutionRollingO1().maximumTotalDamage(one) == 1);
    std::vector<int> dup = {2, 2};
    CHECK(lc3186::SolutionTwoPointers().maximumTotalDamage(dup) == 4);
    std::vector<int> consec = {1, 2, 3, 4, 5};
    CHECK(lc3186::SolutionBinarySearch().maximumTotalDamage(consec) == 7);
}

TEST_CASE("LC 3186 random cross-check: three solutions agree with subset brute force") {
    std::mt19937 rng(123);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 15;
        std::vector<int> v(n);
        for (auto& x : v) x = 1 + rng() % 10;
        long long expect = brute(v);
        auto a = v, b = v, c = v;
        CHECK(lc3186::SolutionTwoPointers().maximumTotalDamage(a) == expect);
        CHECK(lc3186::SolutionBinarySearch().maximumTotalDamage(b) == expect);
        CHECK(lc3186::SolutionRollingO1().maximumTotalDamage(c) == expect);
    }
}
