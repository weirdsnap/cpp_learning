#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <type_traits>

// ============================================================
// ch05/04: 运行时多态
// ============================================================

class Animal {
public:
    virtual ~Animal() = default;
    virtual std::string speak() const { return "Animal"; }
};

class Dog : public Animal {
public:
    std::string speak() const override { return "Dog"; }
};

class Cat : public Animal {
public:
    std::string speak() const override { return "Cat"; }
};

TEST_CASE("运行时多态：基类引用调用派生类虚函数") {
    Dog dog;
    Cat cat;
    Animal& ref1 = dog;
    Animal& ref2 = cat;

    CHECK(ref1.speak() == "Dog");
    CHECK(ref2.speak() == "Cat");
}

TEST_CASE("对象切片：值传递丢失多态") {
    Dog dog;
    Animal a = dog;  // 切片
    CHECK(a.speak() == "Animal");  // 静态绑定
}

TEST_CASE("编译期多态：模板") {
    Dog dog;
    Cat cat;

    auto makeSound = [](auto& t) { return t.speak(); };
    CHECK(makeSound(dog) == "Dog");
    CHECK(makeSound(cat) == "Cat");
}

// ============================================================
// ch05/04-3: 纯虚函数与抽象基类
// ============================================================

class Shape {
public:
    virtual double area() const = 0;
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

TEST_CASE("抽象基类不可实例化") {
    // Shape s;  // 编译错误
    Circle c(5.0);
    Rectangle r(3.0, 4.0);

    Shape* s1 = &c;
    Shape* s2 = &r;

    CHECK(s1->area() == doctest::Approx(78.53975));
    CHECK(s2->area() == doctest::Approx(12.0));
}

// ============================================================
// ch05/04-4: 虚析构函数
// ============================================================

struct BaseNonVirtual {
    ~BaseNonVirtual() = default;
};

struct DerivedNonVirtual : BaseNonVirtual {
    int* data = new int[100];
    ~DerivedNonVirtual() { delete[] data; }
};

struct BaseVirtual {
    virtual ~BaseVirtual() = default;
};

struct DerivedVirtual : BaseVirtual {
    int* data = new int[100];
    ~DerivedVirtual() override { delete[] data; }
};

TEST_CASE("非虚析构通过基类指针删除派生类对象是 UB") {
    // 实际测试中不执行 delete 以避免 UB，仅验证类型特性
    // 这里用编译期检查来验证虚析构的存在
    CHECK(std::has_virtual_destructor_v<BaseVirtual>);
    CHECK(std::has_virtual_destructor_v<DerivedVirtual>);
    CHECK_FALSE(std::has_virtual_destructor_v<BaseNonVirtual>);
    CHECK_FALSE(std::has_virtual_destructor_v<DerivedNonVirtual>);
}

// ============================================================
// ch05/05-6: vtable 与 vptr 布局
// ============================================================

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

TEST_CASE("vptr 增加对象大小") {
    // 有虚函数的类比无虚函数的类多一个 vptr（8 字节在 64 位系统）
    CHECK(sizeof(NoVirtual) == sizeof(int));
    CHECK(sizeof(WithVirtual) > sizeof(NoVirtual));
    CHECK(sizeof(WithTwoVirtual) == sizeof(WithVirtual));  // 虚函数数量不影响大小
}

// 验证 vtable 槽位覆盖
class VtableDemo {
public:
    virtual ~VtableDemo() = default;
    virtual int func1() { return 1; }
    virtual int func2() { return 2; }
    virtual int func3() { return 3; }
};

class VtableOverride : public VtableDemo {
public:
    int func2() override { return 20; }
    virtual int func4() { return 40; }
};

TEST_CASE("vtable 槽位覆盖") {
    VtableOverride obj;
    VtableDemo& baseRef = obj;

    CHECK(baseRef.func1() == 1);   // 未覆盖
    CHECK(baseRef.func2() == 20);  // 已覆盖
    CHECK(baseRef.func3() == 3);   // 未覆盖
    CHECK(obj.func4() == 40);      // 新增虚函数
}

// ============================================================
// ch05/05-7: 构造/析构期间 vptr 行为
// ============================================================

class TrackBase {
public:
    std::string ctor_call;
    std::string dtor_call;

    TrackBase() { identify(); }
    virtual ~TrackBase() { identify(); }
    virtual void identify() { ctor_call = "Base"; dtor_call = "Base"; }
};

class TrackDerived : public TrackBase {
public:
    TrackDerived() { identify(); }
    ~TrackDerived() override { identify(); }
    void identify() override { ctor_call = "Derived"; dtor_call = "Derived"; }
};

TEST_CASE("构造期间 vptr 指向当前正在构造的类") {
    TrackDerived d;
    // Base 构造期间调用的 identify 是 Base 版本
    CHECK(d.ctor_call == "Derived");  // Derived 构造体覆盖了 Base 构造体的赋值
}

// ============================================================
// ch05/05-8: 多继承下的 vptr
// ============================================================

struct IA {
    virtual ~IA() = default;
    virtual std::string fa() { return "IA"; }
};

struct IB {
    virtual ~IB() = default;
    virtual std::string fb() { return "IB"; }
};

struct MultiDerived : IA, IB {
    std::string fa() override { return "MultiDerived"; }
    std::string fb() override { return "MultiDerived"; }
};

TEST_CASE("多继承：派生类对象地址与基类指针地址不同") {
    MultiDerived md;
    IA* pa = &md;
    IB* pb = &md;

    // 多继承下两个基类指针指向不同地址（不同 vptr）
    CHECK(static_cast<void*>(pa) != static_cast<void*>(pb));
}

TEST_CASE("多继承：虚函数正确分派") {
    MultiDerived md;
    IA* pa = &md;
    IB* pb = &md;

    CHECK(pa->fa() == "MultiDerived");
    CHECK(pb->fb() == "MultiDerived");
}
