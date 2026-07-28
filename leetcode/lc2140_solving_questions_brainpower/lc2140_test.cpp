#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <functional>
#include <random>

#include "solution.hpp"

TEST_CASE("LC 2140 examples") {
    std::vector<std::vector<int>> a = {{3, 2}, {4, 3}, {4, 4}, {2, 5}};
    CHECK(lc2140::SolutionIterative().mostPoints(a) == 5);
    std::vector<std::vector<int>> b = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
    CHECK(lc2140::SolutionIterative().mostPoints(b) == 7);
#if LC2140_HAS_MEMO
    std::vector<std::vector<int>> c = {{3, 2}, {4, 3}, {4, 4}, {2, 5}};
    CHECK(lc2140::SolutionMemo().mostPoints(c) == 5);
#endif
}

TEST_CASE("LC 2140 random cross-check with naive memo recursion") {
    std::mt19937 rng(99);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 12;
        std::vector<std::vector<int>> q(n, std::vector<int>(2));
        for (auto& p : q) {
            p[0] = 1 + rng() % 10;
            p[1] = rng() % 6;
        }

        // 朴素记忆化递归作为基准（C++17 写法，不受编译器限制）
        std::vector<long long> memo(n + 1, -1);
        std::function<long long(int)> f = [&](int i) -> long long {
            if (i >= n) return 0;
            if (memo[i] != -1) return memo[i];
            return memo[i] = std::max(f(i + 1),
                                      q[i][0] + f(i + q[i][1] + 1));
        };
        long long expect = f(0);

        auto a = q;
        CHECK(lc2140::SolutionIterative().mostPoints(a) == expect);
#if LC2140_HAS_MEMO
        auto b = q;
        CHECK(lc2140::SolutionMemo().mostPoints(b) == expect);
#endif
    }
}
