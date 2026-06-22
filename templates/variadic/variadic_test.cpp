#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <utility>

// C++11 递归求和
template<typename T>
T sum_recursive(T value) {
    return value;
}

template<typename T, typename... Rest>
T sum_recursive(T value, Rest... rest) {
    return value + sum_recursive(rest...);
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

TEST_CASE("C++11 递归展开可变参数") {
    CHECK(sum_recursive(1, 2, 3, 4, 5) == 15);
    CHECK(sum_recursive(1.5, 2.5, 3.0) == 7.0);
}

TEST_CASE("C++17 折叠表达式求和") {
    CHECK(sum_fold(1, 2, 3, 4, 5) == 15);
}

TEST_CASE("sizeof... 获取参数包大小") {
    CHECK(count_args(1, 2, 3) == 3);
    CHECK(count_args("a", 1, 2.0, 'x') == 4);
    CHECK(count_args() == 0);
}
