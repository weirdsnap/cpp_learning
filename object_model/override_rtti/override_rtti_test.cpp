#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <typeinfo>

// ============================================================
// 1. override 与虚函数重写
// ============================================================

class Shape {
public:
    virtual ~Shape() = default;
    virtual std::string name() const { return "Shape"; }
    virtual std::string describe(const std::string& prefix = "I am a") const {
        return prefix + " " + name();
    }
    virtual std::string kind() const final { return "geometric shape"; }
};

class Circle : public Shape {
public:
    std::string name() const override { return "Circle"; }
    std::string describe(const std::string& prefix = "It is a") const override final {
        return prefix + " " + name();
    }
};

class SmallCircle final : public Circle {
public:
    std::string name() const override { return "SmallCircle"; }
};

// ============================================================
// 2. typeid 与 RTTI
// ============================================================

struct NonPolymorphic {
    int x = 42;
};

struct PolymorphicBase {
    virtual ~PolymorphicBase() = default;
    virtual std::string tag() const { return "PolymorphicBase"; }
};

struct PolymorphicDerived : PolymorphicBase {
    std::string tag() const override { return "PolymorphicDerived"; }
};

// ============================================================
// 3. dynamic_cast
// ============================================================

class Animal {
public:
    virtual ~Animal() = default;
    virtual std::string sound() const { return "???"; }
};

class Dog : public Animal {
public:
    std::string sound() const override { return "Woof"; }
};

class Cat : public Animal {
public:
    std::string sound() const override { return "Meow"; }
};

// 菱形继承：验证 dynamic_cast 跨分支转换
class InterfaceA {
public:
    virtual ~InterfaceA() = default;
    virtual std::string a() const { return "A"; }
};

class InterfaceB {
public:
    virtual ~InterfaceB() = default;
    virtual std::string b() const { return "B"; }
};

class DiamondDerived : public InterfaceA, public InterfaceB {
public:
    std::string a() const override { return "A from Diamond"; }
    std::string b() const override { return "B from Diamond"; }
};

// ============================================================
// 测试用例
// ============================================================

TEST_CASE("override 确保派生类正确覆盖基类虚函数") {
    Circle c;
    Shape& ref = c;
    CHECK(ref.name() == "Circle");
}

TEST_CASE("final 方法禁止进一步覆盖") {
    Circle c;
    Shape& ref = c;
    CHECK(ref.kind() == "geometric shape");
}

TEST_CASE("final 类可以正常使用") {
    SmallCircle sc;
    Shape& ref = sc;
    CHECK(ref.name() == "SmallCircle");
}

TEST_CASE("override + final 组合使用") {
    SmallCircle sc;
    Shape& ref = sc;
    // describe 在 Circle 中标记 final，SmallCircle 无法覆盖
    CHECK(ref.describe() == "I am a SmallCircle");  // 默认参数静态绑定到 Shape
}

TEST_CASE("typeid(类型) 编译期确定") {
    CHECK(typeid(int) == typeid(int));
    CHECK(typeid(double) != typeid(int));
}

TEST_CASE("typeid(多态表达式) 返回动态类型") {
    PolymorphicDerived pd;
    PolymorphicBase& ref = pd;
    CHECK(typeid(ref) == typeid(PolymorphicDerived));
}

TEST_CASE("typeid(非多态表达式) 返回静态类型") {
    NonPolymorphic np;
    NonPolymorphic& ref = np;
    CHECK(typeid(ref) == typeid(NonPolymorphic));
}

TEST_CASE("typedef/using 别名不影响 typeid") {
    using MyInt = int;
    CHECK(typeid(int) == typeid(MyInt));
}

TEST_CASE("dynamic_cast 向上转换编译期完成") {
    Dog dog;
    Animal& ref = dog;  // 隐式向上转换
    CHECK(ref.sound() == "Woof");
}

TEST_CASE("dynamic_cast 向下转换成功") {
    Animal* a = new Dog();
    Dog* d = dynamic_cast<Dog*>(a);
    CHECK(d != nullptr);
    CHECK(d->sound() == "Woof");
    delete a;
}

TEST_CASE("dynamic_cast 向下转换失败返回 nullptr") {
    Animal* a = new Cat();
    Dog* d = dynamic_cast<Dog*>(a);
    CHECK(d == nullptr);
    delete a;
}

TEST_CASE("dynamic_cast 引用失败抛出 bad_cast") {
    Animal* a = new Cat();
    Cat& catRef = static_cast<Cat&>(*a);
    CHECK_THROWS_AS(dynamic_cast<Dog&>(catRef), std::bad_cast);
    delete a;
}

TEST_CASE("dynamic_cast 菱形继承正确计算偏移") {
    DiamondDerived diamond;
    InterfaceB* pb = &diamond;
    DiamondDerived* pDiamond = dynamic_cast<DiamondDerived*>(pb);
    CHECK(pDiamond != nullptr);
    CHECK(pDiamond->a() == "A from Diamond");
    CHECK(pDiamond->b() == "B from Diamond");
}
