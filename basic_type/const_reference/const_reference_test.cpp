#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <type_traits>

TEST_CASE("const 引用绑定临时对象产生独立拷贝") {
    double d = 3.14;
    const int& r = d;  // 产生 int 临时对象

    CHECK(r == 3);

    d = 2.71;
    CHECK(r == 3);  // r 绑定的是原来的临时 int，不是 d
}

TEST_CASE("std::max 返回引用绑定原对象") {
    int a = 10, b = 20;
    const int& m = std::max(a, b);
    CHECK(&m == &b);  // 返回 b 的引用
}

TEST_CASE("std::max initializer_list 版本返回值") {
    auto m = std::max({1, 2, 3});
    CHECK(m == 3);
    CHECK((std::is_same_v<decltype(m), int>));
}
