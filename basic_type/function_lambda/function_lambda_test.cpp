#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <functional>
#include <vector>

int add_one(int x) { return x + 1; }

struct Adder {
    int offset = 0;
    int operator()(int x) const { return x + offset; }
};

class SafeHandle {
    bool valid_;
public:
    explicit SafeHandle(bool v) : valid_(v) {}
    explicit operator bool() const { return valid_; }
};

int g_lambda_global = 1;

TEST_CASE("explicit operator bool 在条件中可用") {
    SafeHandle h(true);
    if (h) {
        CHECK(true);
    } else {
        CHECK(false);
    }
}

TEST_CASE("explicit operator bool 不能隐式转 int") {
    SafeHandle h(true);
    bool b = static_cast<bool>(h);
    CHECK(b);
    CHECK_FALSE((std::is_convertible_v<SafeHandle, int>));
}

TEST_CASE("函数指针与仿函数都能传给模板") {
    std::vector<int> v = {1, 2, 3};
    std::vector<int> result;

    auto apply = [&](auto f) {
        result.clear();
        for (int x : v) result.push_back(f(x));
    };

    apply(add_one);
    CHECK(result == std::vector<int>{2, 3, 4});

    Adder adder{10};
    apply(adder);
    CHECK(result == std::vector<int>{11, 12, 13});

    int factor = 2;
    apply([factor](int x) { return x * factor; });
    CHECK(result == std::vector<int>{2, 4, 6});
}

TEST_CASE("Lambda 按值捕获默认 const") {
    int x = 10;
    auto f = [x]() { return x; };
    CHECK(f() == 10);
}

TEST_CASE("按值捕获是副本，外部变化不影响 lambda") {
    int x = 10;
    auto f = [x]() { return x; };
    x = 100;
    CHECK(f() == 10);
}

TEST_CASE("mutable lambda 修改内部副本") {
    int x = 10;
    auto f = [x]() mutable { x = 20; return x; };
    CHECK(f() == 20);
    CHECK(x == 10);  // 外部不变
}

TEST_CASE("按引用捕获会修改外部变量") {
    int y = 10;
    auto f = [&y]() { y = 30; };
    f();
    CHECK(y == 30);
}

TEST_CASE("全局变量不需要也不能捕获，lambda 中直接引用") {
    auto f = [](int b) { return g_lambda_global + b; };
    CHECK(f(4) == 5);
}

TEST_CASE("隐式捕获 [=] 按值，[&] 按引用") {
    int a = 1, b = 2;
    auto f_value = [=]() { return a + b; };
    auto f_ref = [&]() { a++; b++; };

    CHECK(f_value() == 3);
    f_ref();
    CHECK(a == 2);
    CHECK(b == 3);
}

TEST_CASE("按值捕获指针，指针本身 const 但指向内容可变") {
    int value = 10;
    int* p = &value;

    auto f = [p]() {
        // p = nullptr;  // 编译错误：指针本身是 const
        *p = 20;         // OK：指向的内容可以改
    };
    f();
    CHECK(value == 20);
}

#if __cplusplus >= 201703L
TEST_CASE("C++17 *this 按值捕获对象") {
    struct Foo {
        int value = 42;
        auto get_lambda() {
            return [*this]() { return value; };  // 复制整个对象到闭包
        }
    };

    auto f = Foo().get_lambda();
    CHECK(f() == 42);
}
#endif
