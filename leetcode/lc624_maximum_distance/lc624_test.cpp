#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 624 examples") {
    std::vector<std::vector<int>> a{{1, 2, 3}, {4, 5}, {1, 2, 3}};
    CHECK(lc624::Solution().maxDistance(a) == 4);        // 5 - 1，数组 1 和 0/2

    std::vector<std::vector<int>> b{{1}, {1}};
    CHECK(lc624::Solution().maxDistance(b) == 0);

    // 最值在同一个数组里，必须取"次优配对"
    std::vector<std::vector<int>> c{{-10}, {0, 100}};
    CHECK(lc624::Solution().maxDistance(c) == 110);
}

TEST_CASE("LC 624 greedy vs brute") {
    std::mt19937 rng(624);
    for (int it = 0; it < 500; it++) {
        int m = 2 + rng() % 6;                    // 2~7 个数组
        std::vector<std::vector<int>> arrays(m);
        for (auto& arr : arrays) {
            int len = 1 + rng() % 6;              // 每个数组 1~6 个元素
            arr.resize(len);
            for (auto& x : arr) x = static_cast<int>(rng() % 2001) - 1000;
            std::sort(arr.begin(), arr.end());    // 题目保证升序
        }
        auto copy = arrays;
        CHECK(lc624::Solution().maxDistance(arrays)
              == lc624::SolutionBrute().maxDistance(copy));
    }
}
