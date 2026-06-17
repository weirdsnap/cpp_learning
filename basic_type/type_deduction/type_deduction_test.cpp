#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <type_traits>

TEST_CASE("auto 剥离引用和顶层 const") {
    int x = 42;
    int& rx = x;
    const int cx = 100;

    auto a1 = x;    // int
    auto a2 = rx;   // int（引用剥离）
    auto a3 = cx;   // int（顶层 const 剥离）

    CHECK((std::is_same_v<decltype(a1), int>));
    CHECK((std::is_same_v<decltype(a2), int>));
    CHECK((std::is_same_v<decltype(a3), int>));
}

TEST_CASE("auto& 保留 const") {
    const int cx = 100;
    auto& r = cx;  // const int&
    CHECK((std::is_same_v<decltype(r), const int&>));
}

TEST_CASE("auto 让数组退化为指针") {
    auto arr = "hi";  // const char*
    CHECK((std::is_same_v<decltype(arr), const char*>));
}

TEST_CASE("decltype 标识符表达式保留类型") {
    int i = 42;
    decltype(i) x = i;  // int
    CHECK((std::is_same_v<decltype(x), int>));
}

TEST_CASE("decltype((i)) 左值表达式推导为引用") {
    int i = 42;
    decltype((i)) r = i;  // int&
    CHECK((std::is_same_v<decltype(r), int&>));
}

TEST_CASE("decltype(auto) 保留引用") {
    int i = 42;
    int& ri = i;

    auto a = ri;                    // int
    decltype(auto) b = ri;          // int&
    decltype(auto) c = "abc";       // const char (&)[4]

    CHECK((std::is_same_v<decltype(a), int>));
    CHECK((std::is_same_v<decltype(b), int&>));
    CHECK((std::is_same_v<decltype(c), const char(&)[4]>));
}

TEST_CASE("std::decay 执行完整类型退化") {
    CHECK((std::is_same_v<std::decay_t<int[5]>, int*>));
}
