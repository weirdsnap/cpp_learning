#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

using VS = std::vector<std::string>;

TEST_CASE("LC 3568 examples") {
    VS a{"S.", "XL"};
    CHECK(lc3568::Solution().minMoves(a, 2) == 2);
    VS b{"LS", "RL"};
    CHECK(lc3568::Solution().minMoves(b, 4) == 3);
    VS c{"L.S", "RXL"};
    CHECK(lc3568::Solution().minMoves(c, 3) == -1);
}

TEST_CASE("LC 3568 edge: no litter / litter at start-adjacent / energy exactly 1") {
    VS a{"S."};
    CHECK(lc3568::Solution().minMoves(a, 3) == 0);      // 没有垃圾，0 步
    VS b{"SL"};
    CHECK(lc3568::Solution().minMoves(b, 1) == 1);      // 能量恰好够一步
    VS c{"SX", "XL"};
    CHECK(lc3568::Solution().minMoves(c, 5) == -1);     // 被障碍堵死
}

TEST_CASE("LC 3568 random small grids vs exact-state BFS") {
    std::mt19937 rng(3568);
    const char cells[5] = {'.', '.', 'R', 'X', 'L'};  // L 故意低概率
    for (int it = 0; it < 600; it++) {
        int m = 1 + rng() % 4, n = 1 + rng() % 4;
        VS g(m, std::string(n, '.'));
        int lcnt = 0;
        for (auto& row : g)
            for (auto& c : row) {
                c = cells[rng() % 5];
                if (c == 'L') {
                    if (++lcnt > 4) c = '.';  // 控制 L 数量，保证 oracle 可跑
                }
            }
        int sx = rng() % m, sy = rng() % n;
        g[sx][sy] = 'S';
        int energy = 1 + rng() % 5;
        int want = lc3568::bruteForce(g, energy);
        int got = lc3568::Solution().minMoves(g, energy);
        REQUIRE_MESSAGE(got == want, "it=", it, " got=", got, " want=", want,
                        " energy=", energy);
    }
}
