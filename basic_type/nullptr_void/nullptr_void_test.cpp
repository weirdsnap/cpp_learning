#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstddef>
#include <type_traits>

void foo(int) {}
void foo(char*) {}

void only_nullptr(std::nullptr_t) {}

TEST_CASE("nullptr 匹配指针重载") {
    // 0 匹配 int 版本
    // nullptr 匹配指针版本
    // 这里只验证编译期行为：能编译通过就说明重载解析正确
    foo(nullptr);  // 匹配 foo(char*)，而非 foo(int)
    only_nullptr(nullptr);
    CHECK(true);
}

TEST_CASE("nullptr 类型是 std::nullptr_t") {
    CHECK((std::is_same_v<decltype(nullptr), std::nullptr_t>));
}

TEST_CASE("void* 必须显式转换") {
    int x = 42;
    void* vp = &x;

    // char* cp = vp;  // 编译错误
    char* cp = static_cast<char*>(vp);
    CHECK(cp != nullptr);
}
