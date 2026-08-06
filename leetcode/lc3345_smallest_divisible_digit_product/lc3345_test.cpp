#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "solution.hpp"

TEST_CASE("LC 3345 examples") {
    CHECK(lc3345::Solution().smallestNumber(10, 2) == 10);  // 1*0=0
    CHECK(lc3345::Solution().smallestNumber(15, 3) == 16);  // 1*6=6
}

TEST_CASE("LC 3345 edge cases") {
    // n 本身就满足
    CHECK(lc3345::Solution().smallestNumber(21, 2) == 21);  // 2*1=2
    // 含 0 的数立即命中：数位积 0 可被任何 t 整除
    CHECK(lc3345::Solution().smallestNumber(19, 7) == 20);
    // 个位凑 0：任意 n 答案都不超过下一个整十数
    for (int n = 1; n <= 200; n++) {
        for (int t = 1; t <= 10; t++) {
            int ans = lc3345::Solution().smallestNumber(n, t);
            CHECK(ans >= n);
            CHECK(ans <= (n / 10 + 1) * 10);
        }
    }
}

TEST_CASE("LC 3345 digit-sum variant is wrong") {
    // 数位和版本在这些用例上给出不同（错误）答案
    CHECK(lc3345::SolutionSum().smallestNumber(10, 2) == 11);  // 期望 10
    CHECK(lc3345::SolutionSum().smallestNumber(21, 2) == 22);  // 期望 21
}
