#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 3718 examples") {
    std::vector<int> a{8, 2, 3, 1};
    CHECK(lc3718::Solution().missingMultiple(a, 2) == 4);

    std::vector<int> b{1, 4, 7, 10, 15};
    CHECK(lc3718::Solution().missingMultiple(b, 5) == 5);
}

TEST_CASE("LC 3718 edge cases") {
    // k 本身就不在数组里 → 直接返回 k
    std::vector<int> a{1, 2, 3};
    CHECK(lc3718::Solution().missingMultiple(a, 5) == 5);

    // k = 1，数组是 [1,2,...,n] → 答案是 n+1
    std::vector<int> b(100);
    for (int i = 0; i < 100; i++) b[i] = i + 1;
    CHECK(lc3718::Solution().missingMultiple(b, 1) == 101);

    // 数组全是 k 的倍数（含重复）→ 答案 = k * (去重后的最大倍数序号 + 1)
    std::vector<int> c{3, 3, 6, 9, 3, 6};
    CHECK(lc3718::Solution().missingMultiple(c, 3) == 12);
}

TEST_CASE("LC 3718 set version vs linear scan (random)") {
    std::mt19937 rng(3718);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 100;                 // 1~100
        int k = 1 + rng() % 100;                 // 1~100
        std::vector<int> nums(n);
        for (auto& x : nums) x = 1 + rng() % 100; // 1~100，可重复
        auto copy = nums;
        CHECK(lc3718::Solution().missingMultiple(nums, k) ==
              lc3718::SolutionLinear().missingMultiple(copy, k));
    }
}

TEST_CASE("LC 3718 pigeonhole bound: answer <= k*(n+1)") {
    // 鸽巢：k, 2k, ..., (n+1)k 共 n+1 个互不相同的倍数，
    // nums 最多 n 个元素（去重后 ≤ n 个不同值），至少一个倍数缺席。
    std::mt19937 rng(42);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 100;
        int k = 1 + rng() % 100;
        std::vector<int> nums(n);
        for (auto& x : nums) x = 1 + rng() % 100;

        int ans = lc3718::Solution().missingMultiple(nums, k);
        CHECK(ans % k == 0);
        CHECK(ans <= k * (n + 1));
    }
}
