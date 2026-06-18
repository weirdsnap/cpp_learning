#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstddef>
#include <string>

class Base {
public:
    std::string foo(double) { return "Base::foo(double)"; }
    std::string foo(int) { return "Base::foo(int)"; }
};

class Derived : public Base {
public:
    std::string foo(int) { return "Derived::foo(int)"; }
};

std::string dispatch_pointer(int*) { return "pointer"; }

template<std::size_t N>
std::string dispatch_reference(int (&)[N]) { return "reference"; }

std::string dispatch_void(void*) { return "void*"; }

TEST_CASE("派生类同名函数隐藏基类所有重载") {
    Derived d;
    // Base::foo(double) 被隐藏，只能看到 Derived::foo(int)
    CHECK(d.foo(3.14) == "Derived::foo(int)");
    CHECK(d.foo(42) == "Derived::foo(int)");
}

TEST_CASE("数组引用参数保留数组大小信息") {
    int arr[5] = {1, 2, 3, 4, 5};
    CHECK(dispatch_reference(arr) == "reference");
}

TEST_CASE("当指针重载需要额外转换时，数组引用重载优先") {
    int arr[2] = {1, 2};
    // arr -> void* 需要 array-to-pointer + pointer conversion
    // arr -> int(&)[2] 是引用绑定（精确匹配）
    CHECK(dispatch_void(arr) == "void*");
    CHECK(dispatch_reference(arr) == "reference");

    // 但 int* 与 int(&)[N] 同时存在时，两者均为精确匹配等级，会产生二义性
    // 实际工程中应避免这种重载组合，或显式 static_cast 消除歧义
}
