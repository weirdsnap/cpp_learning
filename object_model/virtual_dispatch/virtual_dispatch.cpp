// 验证 ch02 虚函数相关主题：
// 1. vtable/vptr 支持动态分派
// 2. 构造/析构期间虚函数绑定到当前类
// 3. 虚函数默认参数静态绑定
// 4. final 阻止进一步覆盖

#include <iostream>
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

int main() {
    Base* p = new Derived();
    std::cout << p->name() << std::endl;          // Derived
    std::cout << p->greet() << std::endl;         // Hello Derived（默认参数静态绑定）
    delete p;

    Base* f = new FinalDerived();
    std::cout << f->name() << std::endl;          // FinalDerived
    delete f;

    CtorTracker::log().clear();
    { DerivedTracker dt; }
    std::cout << CtorTracker::log() << std::endl;

    return 0;
}
