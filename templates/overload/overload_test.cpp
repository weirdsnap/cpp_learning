#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>

int last_called = 0;

void f(int) { last_called = 1; }
template<typename T> void f(T) { last_called = 2; }

TEST_CASE("精确匹配时非模板优先于模板") {
    int x = 0;
    f(x);
    CHECK(last_called == 1);
}

TEST_CASE("模板精确匹配优于非模板标准转换") {
    short s = 0;
    f(s);  // 非模板 short->int 是标准转换；模板推导 T=short 是精确匹配
    CHECK(last_called == 2);
}

TEST_CASE("f<>() 强制只考虑模板候选") {
    int x = 0;
    f<>(x);
    CHECK(last_called == 2);
}

// 重载决议三阶段：候选 -> 可行 -> 最佳匹配
// 模板推导失败会静默移除候选（SFINAE）
template<typename T>
typename T::type func(T) { return {}; }  // 只有 T 有 type 成员时才可行

int func(double) { return 0; }

TEST_CASE("SFINAE 推导失败静默移除候选") {
    CHECK(func(3.14) == 0);  // 模板对 double 推导失败，只剩 int func(double)
}
