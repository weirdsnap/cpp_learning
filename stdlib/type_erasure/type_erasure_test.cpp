// 类型擦除行为的 doctest 断言
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <array>
#include <functional>
#include <memory>
#include <string>
#include <vector>

// 三段式骨架的最小实现（与演示代码同构）
struct Concept {
    virtual int call(int) = 0;
    virtual ~Concept() = default;
};

template <class T>
struct Model : Concept {
    T fn;
    explicit Model(T f) : fn(std::move(f)) {}
    int call(int x) override { return fn(x); }
};

struct Func {
    std::unique_ptr<Concept> p;
    template <class T>
    Func(T f) : p(std::make_unique<Model<T>>(std::move(f))) {}
    int operator()(int x) { return p->call(x); }
};

int addOne(int x) { return x + 1; }

struct Doubler {
    int operator()(int x) const { return x * 2; }
};

TEST_CASE("同一持有者类型装入异构可调用对象") {
    std::vector<Func> funcs;
    funcs.emplace_back([](int x) { return x + 10; });  // lambda
    funcs.emplace_back(&addOne);                        // 函数指针
    funcs.emplace_back(Doubler{});                      // 仿函数

    CHECK(funcs[0](1) == 11);
    CHECK(funcs[1](1) == 2);
    CHECK(funcs[2](1) == 2);
}

TEST_CASE("被擦除的类型行为保持各自语义") {
    int captured = 5;
    Func withCapture([&captured](int x) { return x + captured; });
    CHECK(withCapture(1) == 6);
    captured = 100;  // 引用捕获：擦除后仍看到外部变化
    CHECK(withCapture(1) == 101);
}

TEST_CASE("std::function 空调用抛 bad_function_call 而非 UB") {
    std::function<void()> empty;
    CHECK_THROWS_AS(empty(), std::bad_function_call);
}

TEST_CASE("超出内联缓冲的大捕获对象仍可正常工作") {
    auto big = [arr = std::array<long long, 8>{1, 2, 3, 4, 5, 6, 7, 8}]() mutable {
        long long sum = 0;
        for (auto v : arr) sum += v;
        return sum;
    };
    std::function<long long()> sf = big;
    CHECK(sf() == 36);
}
