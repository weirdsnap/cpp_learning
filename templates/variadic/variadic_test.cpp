#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <utility>

// C++11 递归求和：递归时显式指定 T
// 第一次调用后 T 被固定，后续参数都会转成 T
template<typename T>
T sum_fixed_t(T value) {
    return value;
}

template<typename T, typename... Rest>
T sum_fixed_t(T value, Rest... rest) {
    return value + sum_fixed_t<T>(rest...);
}

// C++11 递归求和：不固定 T，每次递归重新推导
template<typename T>
T sum_deduced(T value) {
    return value;
}

template<typename T, typename... Rest>
auto sum_deduced(T value, Rest... rest) {
    return value + sum_deduced(rest...);
}

// C++17 折叠表达式
template<typename... Args>
auto sum_fold(Args... args) {
    return (args + ...);
}

// 计算参数个数
template<typename... Args>
constexpr std::size_t count_args(Args...) {
    return sizeof...(Args);
}

TEST_CASE("C++11 递归展开：显式固定 T 的类型陷阱") {
    // T 被推导为 double，所有参数都按 double 计算
    CHECK(sum_fixed_t(0.5, 1, 0.5, 1) == 3.0);

    // T 被推导为 int，0.5 传给 int 形参时小数部分被截断
    CHECK(sum_fixed_t(1, 0.5, 1, 0.5) == 2);
}

TEST_CASE("C++11 递归展开：不固定 T，每次递归重新推导") {
    // 每次递归 T 根据当前第一个参数重新推导
    CHECK(sum_deduced(1, 0.5, 1, 0.5) == 3.0);
}

TEST_CASE("C++17 折叠表达式求和") {
    CHECK(sum_fold(1, 2, 3, 4, 5) == 15);
    CHECK(sum_fold(1, 0.5, 1, 0.5) == 3.0);
}

TEST_CASE("sizeof... 获取参数包大小") {
    CHECK(count_args(1, 2, 3) == 3);
    CHECK(count_args("a", 1, 2.0, 'x') == 4);
    CHECK(count_args() == 0);
}
