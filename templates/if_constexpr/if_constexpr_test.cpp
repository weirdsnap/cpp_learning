#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <sstream>
#include <string>
#include <type_traits>

// 按类型分派返回不同值，但返回类型 auto 本身依赖 T，安全
template<typename T>
auto describe(T t) {
    if constexpr (std::is_integral_v<T>) {
        return std::to_string(t) + " is integral";
    } else {
        return std::string(t) + " is not integral";
    }
}

// 用 if constexpr 做编译期递归
template<int N>
constexpr int factorial() {
    if constexpr (N <= 1) {
        return 1;
    } else {
        return N * factorial<N - 1>();
    }
}

// 替代可变参数模板的递归终止
template<typename T, typename... Rest>
void print_all(T value, Rest... rest) {
    std::cout << value;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << " ";
        print_all(rest...);
    } else {
        std::cout << "\n";
    }
}

// 返回类型固定，两个分支都返回合法类型
template<typename T>
int safe_f() {
    if constexpr (std::is_same_v<T, int>) { return 0; }
    else { return 42; }
}

TEST_CASE("if constexpr 按类型返回不同值") {
    CHECK(describe(42) == "42 is integral");
    CHECK(describe(std::string{"hello"}) == "hello is not integral");
}

TEST_CASE("if constexpr 编译期递归") {
    static_assert(factorial<0>() == 1);
    static_assert(factorial<1>() == 1);
    static_assert(factorial<5>() == 120);
    CHECK(factorial<5>() == 120);
}

TEST_CASE("if constexpr 替代可变参数递归终止") {
    std::ostringstream oss;
    auto* old = std::cout.rdbuf(oss.rdbuf());
    print_all(1, 2.5, std::string{"end"});
    std::cout.rdbuf(old);
    CHECK(oss.str() == "1 2.5 end\n");
}

TEST_CASE("if constexpr 两个分支返回兼容类型") {
    CHECK(safe_f<int>() == 0);
    CHECK(safe_f<double>() == 42);
}
