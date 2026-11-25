// Lambda 递归的三种写法 — 博客 cpp/practices/lambda_01.md
#pragma once

#include <functional>

namespace lambda_recursion {

// 方案一：std::function 类型擦除（C++11）
// 返回类型在 std::function 声明里写死，不需要显式写 -> int
inline auto factorial_std_function() {
    std::function<int(int)> factorial = [&](int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    };
    return factorial;
}

// 方案二：Y-combinator 自传递（C++14，零开销）
// self 是模板参数，递归要 self(self, ...)，外层包一层 f(f, n)
inline auto factorial_y_combinator() {
    return [](int n) {
        auto f = [](auto&& self, int n) -> int {
            return n <= 1 ? 1 : n * self(self, n - 1);
        };
        return f(f, n);
    };
}

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
#define LAMBDA_RECURSION_HAS_DEDUCING_THIS 1

// 方案三：C++23 deducing this（零开销，写法最自然）
// this auto&& self 把闭包显式声明为第一个参数，递归写 self(n - 1)
inline auto factorial_deducing_this() {
    return [](this auto&& self, int n) -> int {
        return n <= 1 ? 1 : n * self(n - 1);
    };
}

// deducing this 与捕获列表互不干扰：self 指闭包本身
inline auto accumulator_with_capture(int base) {
    return [base](this auto&& self, int n) -> int {
        return n <= 0 ? base : n + self(n - 1);
    };
}
#endif

} // namespace lambda_recursion
