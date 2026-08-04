#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <random>

#include "solution.hpp"

// 参照实现：排序后找相邻 gap
static std::vector<int> sortBased(std::vector<int> nums) {
    std::sort(nums.begin(), nums.end());
    std::vector<int> res;
    for (int i = 1; i < (int)nums.size(); i++)
        for (int x = nums[i - 1] + 1; x < nums[i]; x++)
            res.push_back(x);
    return res;
}

TEST_CASE("LC 3731 examples") {
    std::vector<int> a{1, 4, 2, 5}, b{7, 8, 6, 9}, c{5, 1};
    std::vector<int> a2 = a, b2 = b, c2 = c;
    CHECK(lc3731::Solution().findMissingElements(a) == std::vector<int>{3});
    CHECK(lc3731::Solution().findMissingElements(b).empty());
    CHECK(lc3731::Solution().findMissingElements(c) == std::vector<int>{2, 3, 4});
    CHECK(lc3731::SolutionArray().findMissingElements(a2) == std::vector<int>{3});
    CHECK(lc3731::SolutionArray().findMissingElements(b2).empty());
    CHECK(lc3731::SolutionArray().findMissingElements(c2) == std::vector<int>{2, 3, 4});
}

TEST_CASE("LC 3731 random cross-check with sort-based reference") {
    std::mt19937 rng(3731);
    for (int it = 0; it < 500; it++) {
        // 构造：随机取一个 [lo, hi] 区间，删掉若干元素（保留端点）
        int lo = 1 + rng() % 50, len = 2 + rng() % 50;
        int hi = lo + len - 1;
        std::vector<int> nums;
        for (int x = lo; x <= hi; x++)
            if (x == lo || x == hi || rng() % 3 != 0)
                nums.push_back(x);
        std::shuffle(nums.begin(), nums.end(), rng);
        auto expected = sortBased(nums);
        CHECK(lc3731::Solution().findMissingElements(nums) == expected);
        CHECK(lc3731::SolutionArray().findMissingElements(nums) == expected);
    }
}
