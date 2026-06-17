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
    // bool b = h;        // 编译错误
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

TEST_CASE("mutable lambda 修改内部副本") {
    int x = 10;
    auto f = [x]() mutable { x = 20; return x; };
    CHECK(f() == 20);
    CHECK(x == 10);  // 外部不变
}
