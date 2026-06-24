// 验证 ch05 多态与面向对象设计相关主题：
// 1. override 与虚函数重写
// 2. typeid 与 RTTI
// 3. dynamic_cast

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <cassert>

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
    // final 方法：禁止派生类覆盖
    virtual std::string kind() const final { return "geometric shape"; }
};

class Circle : public Shape {
public:
    // override 检查：确保确实覆盖了基类虚函数
    std::string name() const override { return "Circle"; }
    // override + final 组合：覆盖基类，同时禁止进一步覆盖
    std::string describe(const std::string& prefix = "It is a") const override final {
        return prefix + " " + name();
    }
};

// final 类：禁止被继承
class SmallCircle final : public Circle {
public:
    std::string name() const override { return "SmallCircle"; }
    // 无法覆盖 describe()，因为 Circle 中已标记 final
    // std::string describe(...) const override; // 编译错误
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
    void fetch() const { /* 狗特有的行为 */ }
};

class Cat : public Animal {
public:
    std::string sound() const override { return "Meow"; }
    void purr() const { /* 猫特有的行为 */ }
};

// 菱形继承场景：验证 dynamic_cast 的 this 指针偏移
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
// 演示函数
// ============================================================

void demo_override() {
    std::cout << "=== 1. override 与虚函数重写 ===\n";

    Shape* s1 = new Circle();
    std::cout << "s1->name(): " << s1->name() << "\n";           // Circle
    std::cout << "s1->describe(): " << s1->describe() << "\n";   // I am a Circle（默认参数静态绑定）
    std::cout << "s1->kind(): " << s1->kind() << "\n";           // geometric shape（final 方法）

    Shape* s2 = new SmallCircle();
    std::cout << "s2->name(): " << s2->name() << "\n";           // SmallCircle
    std::cout << "s2->describe(): " << s2->describe() << "\n";   // I am a SmallCircle（Circle 中 final 的 describe）

    delete s1;
    delete s2;
}

void demo_typeid() {
    std::cout << "\n=== 2. typeid 与 RTTI ===\n";

    // typeid(类型) — 编译期确定
    std::cout << "typeid(int).name(): " << typeid(int).name() << "\n";
    std::cout << "typeid(double).name(): " << typeid(double).name() << "\n";
    std::cout << "typeid(std::string).name(): " << typeid(std::string).name() << "\n";

    // typeid(表达式) — 多态 glvalue → 动态类型
    PolymorphicDerived pd;
    PolymorphicBase& polyRef = pd;
    std::cout << "typeid(polyRef).name(): " << typeid(polyRef).name() << "\n";  // PolymorphicDerived

    // 非多态类型 → 静态类型
    NonPolymorphic np;
    NonPolymorphic& npRef = np;
    std::cout << "typeid(npRef).name(): " << typeid(npRef).name() << "\n";      // NonPolymorphic

    // type_info 比较必须用 operator==
    const std::type_info& ti1 = typeid(PolymorphicDerived);
    const std::type_info& ti2 = typeid(polyRef);
    assert(ti1 == ti2);  // ✅ 正确比较方式
    std::cout << "typeid(PolymorphicDerived) == typeid(polyRef): " << (ti1 == ti2 ? "true" : "false") << "\n";

    // typedef/using 别名不影响类型身份
    using MyInt = int;
    std::cout << "typeid(int) == typeid(MyInt): " << (typeid(int) == typeid(MyInt) ? "true" : "false") << "\n";
}

void demo_dynamic_cast() {
    std::cout << "\n=== 3. dynamic_cast ===\n";

    // 向上转换（派生→基）：编译期完成，不需要 RTTI
    Dog dog;
    Animal& animalRef = dog;  // 隐式向上转换
    std::cout << "animalRef.sound(): " << animalRef.sound() << "\n";  // Woof（虚函数分派）

    // 向下转换（基→派生）：运行时查 RTTI
    Animal* animals[] = {new Dog(), new Cat(), new Dog()};

    for (auto* animal : animals) {
        if (Dog* d = dynamic_cast<Dog*>(animal)) {
            std::cout << "This is a Dog, sound: " << d->sound() << "\n";
        } else if (Cat* c = dynamic_cast<Cat*>(animal)) {
            std::cout << "This is a Cat, sound: " << c->sound() << "\n";
        }
    }

    // dynamic_cast 失败返回 nullptr（指针）
    Animal* basePtr = new Cat();
    Dog* dogPtr = dynamic_cast<Dog*>(basePtr);
    std::cout << "dynamic_cast<Dog*>(Cat*) is nullptr: " << (dogPtr == nullptr ? "true" : "false") << "\n";

    // dynamic_cast 失败抛出 bad_cast（引用）
    try {
        Cat& catRef = static_cast<Cat&>(*basePtr);
        Dog& badDogRef = dynamic_cast<Dog&>(catRef);
        (void)badDogRef;
    } catch (const std::bad_cast& e) {
        std::cout << "dynamic_cast<Dog&>(Cat&) throws bad_cast: " << e.what() << "\n";
    }

    delete basePtr;

    // 菱形继承：dynamic_cast 正确计算 this 指针偏移
    DiamondDerived diamond;
    InterfaceB* pb = &diamond;
    // 从 InterfaceB* 向下转型到 DiamondDerived*，需要正确偏移
    DiamondDerived* pDiamond = dynamic_cast<DiamondDerived*>(pb);
    assert(pDiamond != nullptr);
    std::cout << "dynamic_cast<DiamondDerived*>(InterfaceB*) succeeds: "
              << (pDiamond != nullptr ? "true" : "false") << "\n";
    std::cout << "pDiamond->a(): " << pDiamond->a() << "\n";
    std::cout << "pDiamond->b(): " << pDiamond->b() << "\n";

    // 清理
    for (auto* animal : animals) {
        delete animal;
    }
}

int main() {
    demo_override();
    demo_typeid();
    demo_dynamic_cast();

    return 0;
}
