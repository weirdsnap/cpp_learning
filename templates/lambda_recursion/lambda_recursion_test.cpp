#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "lambda_recursion.hpp"

TEST_CASE("三种递归 lambda 结果一致") {
    auto f1 = lambda_recursion::factorial_std_function();
    auto f2 = lambda_recursion::factorial_y_combinator();
    for (int n = 0; n <= 10; n++) {
        int expected = 1;
        for (int i = 2; i <= n; i++) expected *= i;
        CHECK(f1(n) == expected);
        CHECK(f2(n) == expected);
#ifdef LAMBDA_RECURSION_HAS_DEDUCING_THIS
        auto f3 = lambda_recursion::factorial_deducing_this();
        CHECK(f3(n) == expected);
#endif
    }
}

#ifdef LAMBDA_RECURSION_HAS_DEDUCING_THIS
TEST_CASE("deducing this 与捕获列表互不干扰") {
    auto acc = lambda_recursion::accumulator_with_capture(100);
    CHECK(acc(0) == 100);          // 递归到底返回捕获的 base
    CHECK(acc(3) == 100 + 3 + 2 + 1);
}
#endif

TEST_CASE("std::function 递归不依赖显式返回类型") {
    // std::function 是唯一允许省略 -> int 的方案：
    // 返回类型已在 std::function<int(int)> 声明里给出
    std::function<int(int)> f = lambda_recursion::factorial_std_function();
    CHECK(f(5) == 120);
}
