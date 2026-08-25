#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 1927 examples") {
    CHECK(lc1927::Solution().sumGame("5023") == false);
    CHECK(lc1927::Solution().sumGame("25??") == true);
    CHECK(lc1927::Solution().sumGame("?3295???") == false);

    CHECK(lc1927::SolutionInt().sumGame("5023") == false);
    CHECK(lc1927::SolutionInt().sumGame("25??") == true);
    CHECK(lc1927::SolutionInt().sumGame("?3295???") == false);
}

TEST_CASE("LC 1927 formula vs game-tree brute") {
    std::mt19937 rng(1927);
    for (int it = 0; it < 400; it++) {
        int half = 1 + rng() % 3;                 // half ∈ [1,3]，总长 2/4/6
        std::string num(2 * half, '0');
        for (auto& c : num) {
            if (rng() % 2) c = '?';               // 一半概率是 '?'
            else c = '0' + rng() % 10;
        }
        int qcnt = 0;
        for (char c : num) qcnt += c == '?';
        if (qcnt > 4) continue;                   // 暴力只覆盖 '?' ≤ 4
        CAPTURE(num);
        bool expected = lc1927::SolutionBrute().sumGame(num);
        CHECK(lc1927::Solution().sumGame(num) == expected);
        CHECK(lc1927::SolutionInt().sumGame(num) == expected);
    }
}

TEST_CASE("LC 1927 exhaustive small space") {
    // 总长 4，每个位置 ∈ {'0','1','9','?'}，全量 4^4 = 256 组
    const char opts[] = {'0', '1', '9', '?'};
    for (int a = 0; a < 4; a++)
        for (int b = 0; b < 4; b++)
            for (int c = 0; c < 4; c++)
                for (int d = 0; d < 4; d++) {
                    std::string num{opts[a], opts[b], opts[c], opts[d]};
                    CAPTURE(num);
                    bool expected = lc1927::SolutionBrute().sumGame(num);
                    CHECK(lc1927::Solution().sumGame(num) == expected);
                }
}
