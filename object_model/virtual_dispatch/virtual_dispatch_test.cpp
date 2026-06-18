#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>

class Base {
public:
    virtual ~Base() = default;
    virtual std::string name() const { return "Base"; }
    virtual std::string greet(const std::string& prefix = "Hello") const {
        return prefix + " " + name();
    }
};

class Derived : public Base {
public:
    std::string name() const override { return "Derived"; }
    std::string greet(const std::string& prefix = "Hi") const override {
        return prefix + " " + name();
    }
};

class FinalDerived final : public Base {
public:
    std::string name() const override { return "FinalDerived"; }
};

class CtorTracker {
public:
    static std::string& log() { static std::string s; return s; }
    CtorTracker() { log() += "CtorTracker(" + current() + ") "; }
    virtual ~CtorTracker() { log() += "~CtorTracker(" + current() + ") "; }
    virtual std::string current() const { return "base"; }
};

class DerivedTracker : public CtorTracker {
public:
    DerivedTracker() { log() += "DerivedTracker(" + current() + ") "; }
    ~DerivedTracker() { log() += "~DerivedTracker(" + current() + ") "; }
    std::string current() const override { return "derived"; }
};

TEST_CASE("虚函数通过 vtable 动态分派") {
    Derived d;
    Base& ref = d;
    CHECK(ref.name() == "Derived");

    Base* ptr = new Derived();
    CHECK(ptr->name() == "Derived");
    delete ptr;
}

TEST_CASE("虚函数默认参数静态绑定") {
    Derived d;
    Base& ref = d;
    // 默认参数由声明类型 Base 决定，使用 "Hello"
    CHECK(ref.greet() == "Hello Derived");
    // 显式传参不受静态绑定影响
    CHECK(ref.greet("Hey") == "Hey Derived");
}

TEST_CASE("final 类可以正常覆盖虚函数") {
    FinalDerived fd;
    Base& ref = fd;
    CHECK(ref.name() == "FinalDerived");
}

TEST_CASE("构造/析构期间 vptr 切换导致虚函数绑定到当前类") {
    CtorTracker::log().clear();
    {
        DerivedTracker dt;
    }
    CHECK(CtorTracker::log() == "CtorTracker(base) DerivedTracker(derived) ~DerivedTracker(derived) ~CtorTracker(base) ");
}
