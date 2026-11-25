#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "lambda_recursion.hpp"

TEST_CASE("三种递归 lambda 结果一致") {
    auto f1 = lambda_recursion::factorial_std_function();
    auto f2 = lambda_recursion::factorial_y_combinator();
    for (int n = 0; n <= 10; n++) {
        int expected = 1;
        for (int i = 2; i <= n; i++) expected *= i;
        CHECK(f1(n) == expected);
        CHECK(f2(n) == expected);
#ifdef LAMBDA_RECURSION_HAS_DEDUCING_THIS
        auto f3 = lambda_recursion::factorial_deducing_this();
        CHECK(f3(n) == expected);
#endif
    }
}

#ifdef LAMBDA_RECURSION_HAS_DEDUCING_THIS
TEST_CASE("deducing this 与捕获列表互不干扰") {
    auto acc = lambda_recursion::accumulator_with_capture(100);
    CHECK(acc(0) == 100);          // 递归到底返回捕获的 base
    CHECK(acc(3) == 100 + 3 + 2 + 1);
}
#endif

TEST_CASE("std::function 递归不依赖显式返回类型") {
    // std::function 是唯一允许省略 -> int 的方案：
    // 返回类型已在 std::function<int(int)> 声明里给出
    std::function<int(int)> f = lambda_recursion::factorial_std_function();
    CHECK(f(5) == 120);
}

#ifdef LAMBDA_RECURSION_HAS_DEDUCING_THIS
// 成员函数也能用 deducing this：一个模板收三种值类别
struct Logger {
    int log(this auto&& self) {
        using T = decltype(self);
        if constexpr (std::is_rvalue_reference_v<T&&>) return 3;      // &&
        if constexpr (std::is_const_v<std::remove_reference_t<T>>) return 2;  // const&
        return 1;                                                      // &
    }
};

TEST_CASE("deducing this 用于普通成员函数：合并值类别重载") {
    Logger lg;
    const Logger& clg = lg;
    CHECK(lg.log() == 1);
    CHECK(clg.log() == 2);
    CHECK(Logger().log() == 3);
}

// deducing this 简化 CRTP：不再需要 static_cast<Derived*>(this)
template <typename Derived>
struct CounterBase {
    void tick(this auto&& self) {
        self.count++;  // self 推导为 Derived，直接访问派生类成员
    }
};
struct Counter : CounterBase<Counter> {
    int count = 0;
};

TEST_CASE("deducing this 简化 CRTP：免 static_cast") {
    Counter c;
    c.tick();
    c.tick();
    CHECK(c.count == 2);
}

// 继承下的推导规则：推导看调用点表达式的静态类型
struct InhDerived;  // 前向声明，模板惰性实例化，调用时才需要完整定义
struct InhBase {
    // 返回 self 类型的类别码：1=Base，2=Derived
    int which(this auto&& self) {
        using T = std::remove_cvref_t<decltype(self)>;
        if constexpr (std::is_same_v<T, InhDerived>) return 2;
        return 1;
    }
};
struct InhDerived : InhBase {};

TEST_CASE("继承：Self 推导为调用点静态类型") {
    InhDerived d;
    CHECK(d.which() == 2);            // 直接调用 → Derived
    InhBase& b = d;
    CHECK(b.which() == 1);            // 经基类引用 → Base（静态类型决定）
}

// 按值写法的切片：Self 脱成纯类型，经基类引用调用时派生部分丢失
struct SliceBase {
    int base_member = 1;
    int size_of_self(this auto self) { return (int)sizeof(self); }
};
struct SliceDerived : SliceBase {
    int extra = 2;
};

TEST_CASE("按值推导 + 继承：经基类引用调用发生切片") {
    SliceDerived d;
    CHECK(d.size_of_self() == (int)sizeof(SliceDerived));  // 直接调用：完整
    SliceBase& b = d;
    CHECK(b.size_of_self() == (int)sizeof(SliceBase));     // 切片：只剩基类部分
}
#endif
