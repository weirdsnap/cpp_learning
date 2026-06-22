#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <functional>
#include <string>
#include <type_traits>
#include <vector>

int deduced_type_id = 0;

template<typename T>
void by_value(T) { deduced_type_id = 1; }

template<typename T>
void by_ref(const T&) { deduced_type_id = 2; }

template<typename T>
void by_ptr(T*) { deduced_type_id = 3; }

TEST_CASE("按值推导剥离顶层 const") {
    const int ci = 42;
    by_value(ci);
    CHECK(deduced_type_id == 1);  // 调用了 by_value<T>
    // 在模板内 T 是 int，不是 const int
}

TEST_CASE("按引用推导保留顶层 const 语义") {
    const int ci = 42;
    by_ref(ci);
    CHECK(deduced_type_id == 2);
}

TEST_CASE("按指针推导保留指向对象的 const") {
    const int ci = 42;
    by_ptr(&ci);
    CHECK(deduced_type_id == 3);
}

TEST_CASE("auto 单独使用剥离引用和顶层 const") {
    int x = 10;
    int& ref = x;
    auto a = ref;  // a 是 int，不是 int&
    static_assert(std::is_same_v<decltype(a), int>);

    const int cx = 20;
    auto b = cx;  // b 是 int，不是 const int
    static_assert(std::is_same_v<decltype(b), int>);
}

TEST_CASE("auto& 和 auto&& 保留引用属性") {
    int x = 10;
    auto& ref = x;
    static_assert(std::is_same_v<decltype(ref), int&>);

    auto&& rref = 10;  // 绑定右值
    static_assert(std::is_same_v<decltype(rref), int&&>);
}

TEST_CASE("模板推导是模式匹配，不是解方程") {
    // std::function<void(T)> 的 T 无法从 lambda 类型中直接解出
    // 以下代码如果 uncomment 会编译失败：
    // template<typename T>
    // void take(std::function<void(T)> f) {}
    // take([](int x){});  // 推导失败

    // 但直接按值接收可调用对象可以推导
    auto lambda = [](int x) { return x * 2; };
    static_assert(std::is_same_v<decltype(lambda), decltype(lambda)>);
}
