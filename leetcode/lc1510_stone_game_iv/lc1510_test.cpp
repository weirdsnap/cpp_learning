#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "solution.hpp"

TEST_CASE("LC 1510 examples") {
    CHECK(lc1510::Solution().winnerSquareGame(1) == true);
    CHECK(lc1510::Solution().winnerSquareGame(2) == false);
    CHECK(lc1510::Solution().winnerSquareGame(4) == true);
    CHECK(lc1510::Solution().winnerSquareGame(7) == false);
}

TEST_CASE("LC 1510 DP vs memo vs brute, small n = 0..25") {
    // 无记忆化暴力是指数级，只对拍小规模
    for (int n = 0; n <= 25; n++) {
        CAPTURE(n);
        bool expected = lc1510::SolutionBrute().winnerSquareGame(n);
        CHECK(lc1510::Solution().winnerSquareGame(n) == expected);
        CHECK(lc1510::SolutionMemo().winnerSquareGame(n) == expected);
    }
}

TEST_CASE("LC 1510 DP vs memo, exhaustive n = 0..5000") {
    for (int n = 0; n <= 5000; n++) {
        CAPTURE(n);
        CHECK(lc1510::Solution().winnerSquareGame(n)
              == lc1510::SolutionMemo().winnerSquareGame(n));
    }
}
