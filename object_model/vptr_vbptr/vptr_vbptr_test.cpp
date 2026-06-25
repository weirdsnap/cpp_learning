#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

struct Plain {
    int a;
    int b;
};

struct WithVirtual {
    int a;
    int b;
    virtual void foo() {}
    virtual ~WithVirtual() = default;
};

struct Base {
    int x;
};

struct Derived : virtual Base {
    int y;
};

struct VirtualBase {
    int a;
    virtual void foo() {}
    virtual ~VirtualBase() = default;
};

struct VirtualDerived : virtual VirtualBase {
    int b;
    void foo() override {}
};

TEST_CASE("无虚函数无虚继承：最小对象") {
    CHECK(sizeof(Plain) == sizeof(int) * 2);
}

TEST_CASE("有虚函数：含 vptr，大小增加") {
    // vptr 通常占 8 字节（64位）
    CHECK(sizeof(WithVirtual) > sizeof(Plain));
}

TEST_CASE("有虚继承：含 vbptr") {
    // Derived 含 vbptr + y + Base::x
    CHECK(sizeof(Derived) > sizeof(Base));
}

TEST_CASE("同时有虚函数和虚继承") {
    CHECK(sizeof(VirtualDerived) > sizeof(VirtualBase));
}
