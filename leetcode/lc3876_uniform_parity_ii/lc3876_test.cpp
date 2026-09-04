#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <numeric>
#include <random>

#include "solution.hpp"

using VI = std::vector<int>;

TEST_CASE("LC 3876 examples") {
    VI a{1, 4, 7};
    CHECK(lc3876::Solution().uniformArray(a) == true);
    CHECK(lc3876::SolutionV2().uniformArray(a) == true);
    VI b{2, 3};
    CHECK(lc3876::Solution().uniformArray(b) == false);
    CHECK(lc3876::SolutionV2().uniformArray(b) == false);
    VI c{4, 6};
    CHECK(lc3876::Solution().uniformArray(c) == true);
    CHECK(lc3876::SolutionV2().uniformArray(c) == true);
}

TEST_CASE("LC 3876 exhaustive: distinct values from 1..7, n <= 4 (ordered)") {
    // 有序排列：值域 {1..7} 选 n 个的全部排列
    for (int n = 1; n <= 4; n++) {
        VI idx(n);
        std::vector<char> used(8, 0);
        std::function<void(int)> gen = [&](int d) {
            if (d == n) {
                VI nums(n);
                for (int t = 0; t < n; t++) nums[t] = idx[t];
                bool got = lc3876::Solution().uniformArray(nums);
                bool got2 = lc3876::SolutionV2().uniformArray(nums);
                bool want = lc3876::bruteForce(nums);
                REQUIRE_MESSAGE(got == want, "n=", n, " got=", got, " want=", want);
                REQUIRE_MESSAGE(got2 == want, "v2 n=", n, " got=", got2, " want=", want);
                return;
            }
            for (int v = 1; v <= 7; v++) {
                if (used[v]) continue;
                used[v] = 1;
                idx[d] = v;
                gen(d + 1);
                used[v] = 0;
            }
        };
        gen(0);
    }
}

TEST_CASE("LC 3876 random: distinct values up to 1e9, n up to 8") {
    std::mt19937 rng(3876);
    for (int it = 0; it < 300; it++) {
        int n = 1 + rng() % 8;
        VI pool(300);
        std::iota(pool.begin(), pool.end(), 1);
        std::shuffle(pool.begin(), pool.end(), rng);
        VI nums(pool.begin(), pool.begin() + n);
        // 随机放大数值（保持互异）
        for (auto& x : nums) x = x * (int)(1 + rng() % 4000000);
        REQUIRE(lc3876::Solution().uniformArray(nums) == lc3876::bruteForce(nums));
        REQUIRE(lc3876::SolutionV2().uniformArray(nums) == lc3876::bruteForce(nums));
    }
}

TEST_CASE("LC 3875 always true") {
    VI a{2, 3};
    CHECK(lc3875::Solution().uniformArray(a) == true);
    VI b{4, 6};
    CHECK(lc3875::Solution().uniformArray(b) == true);
    VI c{1};
    CHECK(lc3875::Solution().uniformArray(c) == true);
    VI d{1, 2, 3, 4, 5, 6};
    CHECK(lc3875::Solution().uniformArray(d) == true);
}
