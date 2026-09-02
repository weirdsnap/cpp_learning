#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

using VC = std::vector<char>;
using VVC = std::vector<VC>;

TEST_CASE("LC 531 examples") {
    VVC a{{'W', 'W', 'B'}, {'W', 'B', 'W'}, {'B', 'W', 'W'}};
    CHECK(lc531::Solution().findLonelyPixel(a) == 3);
    VVC b{{'B', 'B', 'B'}, {'B', 'B', 'W'}, {'B', 'B', 'B'}};
    CHECK(lc531::Solution().findLonelyPixel(b) == 0);
    VVC c{{'B', 'B', 'B'}, {'B', 'B', 'B'}, {'B', 'B', 'B'}};
    CHECK(lc531::Solution().findLonelyPixel(c) == 0);
}

TEST_CASE("LC 531 exhaustive: all 2^(n*m) grids, n*m <= 16") {
    // 1x1..2x2..3x3..4x4 等组合，穷举全部 01 网格
    for (int n = 1; n <= 4; n++) {
        for (int m = 1; m <= 4; m++) {
            if (n * m > 16) continue;
            long long total = 1LL << (n * m);
            for (long long mask = 0; mask < total; mask++) {
                VVC g(n, VC(m, 'W'));
                for (int i = 0; i < n; i++)
                    for (int j = 0; j < m; j++)
                        if ((mask >> (i * m + j)) & 1) g[i][j] = 'B';
                REQUIRE(lc531::Solution().findLonelyPixel(g) ==
                        lc531::bruteForce(g));
            }
        }
    }
}

TEST_CASE("LC 531 random grids up to 30x30") {
    std::mt19937 rng(531);
    for (int it = 0; it < 300; it++) {
        int n = 1 + rng() % 30, m = 1 + rng() % 30;
        int density = rng() % 3 + 1;  // 1/3, 1/2, 2/3 三档密度
        VVC g(n, VC(m, 'W'));
        for (auto& row : g)
            for (auto& c : row)
                if ((int)(rng() % 3) < density) c = 'B';
        CHECK(lc531::Solution().findLonelyPixel(g) == lc531::bruteForce(g));
    }
}
