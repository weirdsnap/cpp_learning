#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 1386 examples") {
    std::vector<std::vector<int>> a{{1, 2}, {1, 3}, {1, 8}, {2, 6}, {3, 1}, {3, 10}};
    int n1 = 3;
    CHECK(lc1386::Solution().maxNumberOfFamilies(n1, a) == 4);

    std::vector<std::vector<int>> b{{2, 1}, {1, 8}, {2, 6}};
    int n2 = 2;
    CHECK(lc1386::Solution().maxNumberOfFamilies(n2, b) == 2);

    std::vector<std::vector<int>> c{{4, 3}, {1, 4}, {4, 6}, {1, 7}};
    int n3 = 4;
    CHECK(lc1386::Solution().maxNumberOfFamilies(n3, c) == 4);
}

TEST_CASE("LC 1386 popcount version vs direct block check") {
    std::mt19937 rng(1386);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 8;                    // 1~8 行
        int m = rng() % (10 * n + 1);             // 0~10n 个预订
        std::vector<std::vector<int>> seats;
        for (int i = 0; i < m; i++)
            seats.push_back({(int)(rng() % n) + 1, (int)(rng() % 10) + 1});
        auto copy = seats;
        CHECK(lc1386::Solution().maxNumberOfFamilies(n, seats)
              == lc1386::SolutionRef().maxNumberOfFamilies(n, copy));
    }
}

TEST_CASE("LC 1386 exhaustive single-row masks") {
    // 单行 8 个相关位全枚举 256 种，盯住每个分支
    for (int mask = 0; mask < 256; mask++) {
        int n = 1;
        std::vector<std::vector<int>> seats;
        for (int b = 0; b < 8; b++)
            if (mask & (1 << b)) seats.push_back({1, 9 - b});  // bit b → 座位 9-b
        auto copy = seats;
        CAPTURE(mask);
        CHECK(lc1386::Solution().maxNumberOfFamilies(n, seats)
              == lc1386::SolutionRef().maxNumberOfFamilies(n, copy));
    }
}
