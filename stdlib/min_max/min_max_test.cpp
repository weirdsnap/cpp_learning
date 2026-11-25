#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <vector>

#include "solution.hpp"

TEST_CASE("max 双参数版 vs initializer_list 版") {
    CHECK(stl_minmax::max_two(1, 2) == 2);
    CHECK(stl_minmax::max_list({3, 1, 4, 1, 5}) == 5);
    // 列表版返回值，双参数版返回 const 引用
    static_assert(std::is_same_v<decltype(std::max({1, 2})), int>);
    static_assert(std::is_same_v<decltype(std::max(1, 2)), const int&>);
}

TEST_CASE("max_element 求容器最大值，空容器返回 end()") {
    std::vector<int> v = {3, 1, 4, 1, 5};
    CHECK(stl_minmax::max_elem(v) == 5);

    std::vector<int> empty;
    CHECK(stl_minmax::max_elem_empty_returns_end(empty));
}

#if __cplusplus >= 202002L
TEST_CASE("ranges::max 直接吃 range，返回值") {
    std::vector<int> v = {3, 1, 4, 1, 5};
    CHECK(stl_minmax::ranges_max(v) == 5);
    // ranges::max 支持投影
    struct P { int x; };
    std::vector<P> ps = {{1}, {9}, {3}};
    CHECK(std::ranges::max(ps, {}, &P::x).x == 9);
}
#endif

TEST_CASE("min 家族规则对称") {
    CHECK(std::min({3, 1, 4}) == 1);
    std::vector<int> v = {3, 1, 4};
    CHECK(*std::min_element(v.begin(), v.end()) == 1);
}
