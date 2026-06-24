// 验证 ch05/04 运行时多态概述 + ch05/05 vtable 与虚指针布局
//
// ch05/04:
// 1. 运行时多态 vs 编译期多态
// 2. 动态绑定的三个必要条件
// 3. 纯虚函数与抽象基类
// 4. 虚析构函数
// 5. 对象切片
//
// ch05/05:
// 6. vtable 与 vptr 布局验证
// 7. 构造/析构期间 vptr 行为
// 8. 多继承下的 vptr

#include <iostream>
#include <string>
#include <cassert>

// ============================================================
// ch05/04-1: 运行时多态 vs 编译期多态
// ============================================================

// 运行时多态：虚函数
class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() const { std::cout << "  Animal speaks\n"; }
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "  Dog barks\n"; }
};

class Cat : public Animal {
public:
    void speak() const override { std::cout << "  Cat meows\n"; }
};

void makeSound(Animal& a) {
    a.speak();  // 运行时决定
}

// 编译期多态：模板
template <typename T>
void makeSoundTemplate(T& t) {
    t.speak();  // 编译期展开
}

// ============================================================
// ch05/04-2: 动态绑定的三个必要条件
// ============================================================

void demo_dynamic_binding_conditions() {
    std::cout << "=== 动态绑定的三个必要条件 ===\n";

    Dog dog;
    Animal animal;

    // 条件1: 通过指针或引用调用
    Animal* p = &dog;
    std::cout << "  Animal* p = &dog; p->speak(): ";
    p->speak();  // ✅ Dog barks

    Animal& r = dog;
    std::cout << "  Animal& r = dog; r.speak(): ";
    r.speak();   // ✅ Dog barks

    // 条件2: 调用的函数是虚函数 — speak() 是 virtual

    // 条件3: 动态类型与静态类型不同
    // p 的静态类型是 Animal*，动态类型是 Dog*

    // 值传递触发对象切片 — 不满足条件1
    Animal a = dog;  // 切片
    std::cout << "  Animal a = dog; a.speak(): ";
    a.speak();       // Animal speaks（静态绑定）
}

// ============================================================
// ch05/04-3: 纯虚函数与抽象基类
// ============================================================

class Shape {
public:
    virtual double area() const = 0;  // 纯虚函数
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
};

// ============================================================
// ch05/04-4: 虚析构函数
// ============================================================

struct BaseNonVirtual {
    ~BaseNonVirtual() { std::cout << "  ~BaseNonVirtual\n"; }
};

struct DerivedNonVirtual : BaseNonVirtual {
    int* data = new int[100];
    ~DerivedNonVirtual() {
        delete[] data;
        std::cout << "  ~DerivedNonVirtual\n";
    }
};

struct BaseVirtual {
    virtual ~BaseVirtual() { std::cout << "  ~BaseVirtual\n"; }
};

struct DerivedVirtual : BaseVirtual {
    int* data = new int[100];
    ~DerivedVirtual() override {
        delete[] data;
        std::cout << "  ~DerivedVirtual\n";
    }
};

// ============================================================
// ch05/05-6: vtable 与 vptr 布局验证
// ============================================================

// 通过检查对象大小和内存布局来推断 vptr 的存在
class NoVirtual {
    int x;
public:
    void foo() {}
};

class WithVirtual {
    int x;
public:
    virtual ~WithVirtual() = default;
    virtual void foo() {}
};

class WithTwoVirtual {
    int x;
public:
    virtual ~WithTwoVirtual() = default;
    virtual void foo() {}
    virtual void bar() {}
};

// 验证 vtable 槽位：通过函数指针访问
class VtableDemo {
public:
    virtual ~VtableDemo() = default;
    virtual int func1() { return 1; }
    virtual int func2() { return 2; }
    virtual int func3() { return 3; }
};

class VtableOverride : public VtableDemo {
public:
    int func2() override { return 20; }  // 只覆盖 func2
    virtual int func4() { return 40; }   // 新增虚函数
};

// ============================================================
// ch05/05-7: 构造/析构期间 vptr 行为
// ============================================================

class Base {
public:
    Base() {
        std::cout << "  Base constructor: ";
        identify();  // 调用 Base::identify（vptr 指向 Base vtable）
    }
    virtual ~Base() {
        std::cout << "  Base destructor: ";
        identify();  // 调用 Base::identify（vptr 已切回 Base vtable）
    }
    virtual void identify() const {
        std::cout << "Base\n";
    }
};

class Derived : public Base {
public:
    Derived() {
        std::cout << "  Derived constructor: ";
        identify();  // 调用 Derived::identify（vptr 已切换）
    }
    ~Derived() override {
        std::cout << "  Derived destructor: ";
        identify();  // 调用 Derived::identify（vptr 还在 Derived vtable）
    }
    void identify() const override {
        std::cout << "Derived\n";
    }
};

// ============================================================
// ch05/05-8: 多继承下的 vptr
// ============================================================

struct IA {
    virtual ~IA() = default;
    virtual void fa() { std::cout << "  IA::fa\n"; }
};

struct IB {
    virtual ~IB() = default;
    virtual void fb() { std::cout << "  IB::fb\n"; }
};

struct MultiDerived : IA, IB {
    void fa() override { std::cout << "  MultiDerived::fa\n"; }
    void fb() override { std::cout << "  MultiDerived::fb\n"; }
};

// ============================================================
// 演示函数
// ============================================================

void demo_runtime_polymorphism() {
    std::cout << "\n=== ch05/04-1: 运行时多态 ===\n";

    Dog dog;
    Cat cat;
    makeSound(dog);  // Dog barks
    makeSound(cat);  // Cat meows

    // 编译期多态对比
    std::cout << "  --- 编译期多态（模板）---\n";
    makeSoundTemplate(dog);
    makeSoundTemplate(cat);
}

void demo_abstract_base() {
    std::cout << "\n=== ch05/04-3: 纯虚函数与抽象基类 ===\n";

    Circle c(5.0);
    Rectangle r(3.0, 4.0);

    Shape* shapes[] = {&c, &r};
    for (auto* s : shapes) {
        std::cout << "  area = " << s->area() << "\n";
    }
}

void demo_virtual_destructor() {
    std::cout << "\n=== ch05/04-4: 虚析构函数 ===\n";

    std::cout << "  非虚析构（UB/泄漏）:\n";
    BaseNonVirtual* p1 = new DerivedNonVirtual();
    delete p1;  // 只调 ~BaseNonVirtual，data 泄漏

    std::cout << "  虚析构（正确）:\n";
    BaseVirtual* p2 = new DerivedVirtual();
    delete p2;  // 先调 ~DerivedVirtual，再调 ~BaseVirtual
}

void demo_vtable_layout() {
    std::cout << "\n=== ch05/05-6: vtable 与 vptr 布局 ===\n";

    // 通过 sizeof 推断 vptr 存在
    std::cout << "  sizeof(NoVirtual) = " << sizeof(NoVirtual)
              << " (int only)\n";
    std::cout << "  sizeof(WithVirtual) = " << sizeof(WithVirtual)
              << " (int + vptr)\n";
    std::cout << "  sizeof(WithTwoVirtual) = " << sizeof(WithTwoVirtual)
              << " (int + vptr, same as WithVirtual)\n";

    // 验证 vtable 槽位
    VtableOverride obj;
    VtableDemo& baseRef = obj;

    // 通过虚函数调用验证 vtable 覆盖
    std::cout << "  baseRef.func1() = " << baseRef.func1() << " (not overridden)\n";
    std::cout << "  baseRef.func2() = " << baseRef.func2() << " (overridden)\n";
    std::cout << "  baseRef.func3() = " << baseRef.func3() << " (not overridden)\n";
    // func4 只能通过派生类指针调用
    std::cout << "  obj.func4() = " << obj.func4() << " (new in derived)\n";
}

void demo_ctor_dtor_vptr() {
    std::cout << "\n=== ch05/05-7: 构造/析构期间 vptr 行为 ===\n";

    std::cout << "  Creating Derived:\n";
    Derived d;
    std::cout << "  Destroying Derived:\n";
    // d 析构时自动触发
}

void demo_multi_inheritance() {
    std::cout << "\n=== ch05/05-8: 多继承下的 vptr ===\n";

    MultiDerived md;

    IA* pa = &md;
    IB* pb = &md;

    std::cout << "  pa = " << pa << ", pb = " << pb
              << ", &md = " << &md << "\n";
    std::cout << "  (pa == pb) = " << (static_cast<void*>(pa) == static_cast<void*>(pb) ? "true" : "false")
              << " (different vptr offsets)\n";

    pa->fa();
    pb->fb();
}

int main() {
    demo_dynamic_binding_conditions();
    demo_runtime_polymorphism();
    demo_abstract_base();
    demo_virtual_destructor();
    demo_vtable_layout();
    demo_ctor_dtor_vptr();
    demo_multi_inheritance();

    return 0;
}
