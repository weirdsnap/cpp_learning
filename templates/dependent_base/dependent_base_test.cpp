#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>

int global_func_called = 0;
void func() { global_func_called = 1; }

template<typename T>
struct Base {
    int base_func_called = 0;
    void func() { base_func_called = 1; }
    static constexpr int value = 42;
};

template<typename T>
struct Derived : Base<T> {
    void call_global() {
        func();  // 调用全局 func，因为 Base<T> 是依赖基类，不会参与非限定查找
    }

    void call_base() {
        this->func();  // 通过 this-> 强制到依赖基类查找
    }

    void call_base_qualified() {
        Base<T>::func();
    }

    int get_value() {
        return this->value;
    }
};

TEST_CASE("模板派生类中非限定名默认不去依赖基类查找") {
    Derived<int> d;
    global_func_called = 0;
    d.call_global();
    CHECK(global_func_called == 1);
    CHECK(d.base_func_called == 0);
}

TEST_CASE("this-> 强制在依赖基类中查找") {
    Derived<int> d;
    d.base_func_called = 0;
    d.call_base();
    CHECK(d.base_func_called == 1);
}

TEST_CASE("显式 Base<T>:: 限定也能访问基类成员") {
    Derived<int> d;
    d.base_func_called = 0;
    d.call_base_qualified();
    CHECK(d.base_func_called == 1);
}

TEST_CASE("依赖基类的静态成员也需要 this-> 或限定") {
    Derived<int> d;
    CHECK(d.get_value() == 42);
}
