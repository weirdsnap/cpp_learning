#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <numeric>
#include <random>

#include "solution.hpp"

TEST_CASE("LC 3513 examples") {
    std::vector<int> a{1, 2};
    CHECK(lc3513::Solution().uniqueXorTriplets(a) == 2);

    std::vector<int> b{1, 2, 3, 4};
    CHECK(lc3513::Solution().uniqueXorTriplets(b) == 8);

    std::vector<int> c{1, 2, 3};
    CHECK(lc3513::Solution().uniqueXorTriplets(c) == 4);
}

TEST_CASE("LC 3513 formula vs brute, exhaustive n = 1..64") {
    for (int n = 1; n <= 64; n++) {
        std::vector<int> nums(n);
        std::iota(nums.begin(), nums.end(), 1);   // [1, 2, ..., n]
        CAPTURE(n);
        CHECK(lc3513::Solution().uniqueXorTriplets(nums)
              == lc3513::SolutionBrute().uniqueXorTriplets(nums));
    }
}

TEST_CASE("LC 3513 brute confirms full coverage [0, 2^k)") {
    // n ≥ 3 时，暴力集合恰好是 [0, 大于n的最小2的幂 - 1]
    for (int n = 3; n <= 40; n++) {
        std::vector<int> nums(n);
        std::iota(nums.begin(), nums.end(), 1);
        std::unordered_set<int> seen;
        for (int i = 0; i < n; i++)
            for (int j = i; j < n; j++)
                for (int k = j; k < n; k++)
                    seen.insert(nums[i] ^ nums[j] ^ nums[k]);
        int m = 1;
        while (m <= n) m <<= 1;
        CAPTURE(n);
        CHECK(static_cast<int>(seen.size()) == m);
        for (int v = 0; v < m; v++) CHECK(seen.count(v) == 1);
    }
}

TEST_CASE("LC 3513 permutation order irrelevant") {
    // nums 是排列，打乱顺序不影响答案
    std::mt19937 rng(3513);
    for (int it = 0; it < 50; it++) {
        int n = 1 + rng() % 40;
        std::vector<int> nums(n);
        std::iota(nums.begin(), nums.end(), 1);
        std::shuffle(nums.begin(), nums.end(), rng);
        auto sorted = nums;
        std::sort(sorted.begin(), sorted.end());
        CHECK(lc3513::Solution().uniqueXorTriplets(nums)
              == lc3513::Solution().uniqueXorTriplets(sorted));
    }
}
