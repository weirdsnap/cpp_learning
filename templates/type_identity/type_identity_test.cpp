#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <functional>
#include <string>
#include <type_traits>

// 与 type_identity.cpp 相同的修复版
template <typename T>
void call_with(std::type_identity_t<std::function<void(T)>> f, T val) {
    f(val);
}

template <typename T>
T clamp_same(T v, std::type_identity_t<T> lo, std::type_identity_t<T> hi) {
    return v < lo ? lo : (hi < v ? hi : v);
}

template <typename T>
constexpr size_t size_of_tag(std::type_identity<T>) {
    return sizeof(T);
}

TEST_CASE("type_identity fixes lambda -> std::function deduction") {
    std::string out;
    auto print = [&](int x) { out += std::to_string(x); };
    call_with(print, 42);
    CHECK(out == "42");
}

TEST_CASE("type_identity forces same-type arguments") {
    CHECK(clamp_same(5, 1, 3) == 3);
    CHECK(clamp_same(0, 1, 3) == 1);
    CHECK(clamp_same(2, 1, 3) == 2);
    // clamp_same(5, 1L, 3L) 编译失败：T 已从 5 推为 int，
    // 第二参数类型被固定为 const int&，long 实参不参与推导
}

TEST_CASE("type_identity as tag") {
    CHECK(size_of_tag(std::type_identity<double>{}) == sizeof(double));
    CHECK(size_of_tag(std::type_identity<char>{}) == 1);
}

TEST_CASE("lambda closure types are unique") {
    auto l1 = [](int x) { return x + 1; };
    auto l2 = [](int x) { return x + 1; };
    static_assert(!std::is_same_v<decltype(l1), decltype(l2)>);

    std::function<int(int)> f1 = l1, f2 = l2;
    static_assert(std::is_same_v<decltype(f1), decltype(f2)>);

    // 单向转换：lambda -> std::function 可以，反之不行
    static_assert(std::is_convertible_v<decltype(l1), std::function<int(int)>>);
    static_assert(!std::is_convertible_v<std::function<int(int)>, decltype(l1)>);
}

TEST_CASE("std::function size is fixed regardless of capture") {
    auto small = [] { return 1; };
    int buf[8] = {};
    auto big = [buf] { return buf[0]; };

    std::function<int()> fs = small, fb = big;
    static_assert(sizeof(fs) == sizeof(fb));  // 类型固定 -> 大小固定
    CHECK(sizeof(small) <= sizeof(fb));      // 空捕获 lambda 只有 1 字节
    CHECK(sizeof(big) == sizeof(buf));       // 按值捕获 32 字节
}
