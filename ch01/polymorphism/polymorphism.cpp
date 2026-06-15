#include <cstdint>
#include <iostream>
#include <type_traits>

// 验证 ch01 中关于面向对象的知识点：
// 12 静态类型与动态类型
// 23 enum class vs enum
// 24 对象切片

// 12 静态类型与动态类型 + 24 对象切片
class Base {
public:
    virtual void foo() const {
        std::cout << "  Base::foo()\n";
    }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    int extra = 42;
    void foo() const override {
        std::cout << "  Derived::foo(), extra = " << extra << "\n";
    }
};

void by_value(Base b) {
    std::cout << "  按值传参触发切片，调用: ";
    b.foo();
}

void by_reference(const Base& b) {
    std::cout << "  按引用传参保留多态，调用: ";
    b.foo();
}

void test_polymorphism() {
    std::cout << "=== 12 静态类型与动态类型 ===\n";
    Derived d;
    Base* p = &d;       // 静态类型 Base*，动态类型 Derived*
    Base& ref = d;      // 静态类型 Base&，动态类型 Derived&

    std::cout << "Base* p = &d;  p->foo() -> ";
    p->foo();
    std::cout << "Base& ref = d; ref.foo() -> ";
    ref.foo();

    std::cout << "\n=== 24 对象切片 ===\n";
    std::cout << "Derived d; by_value(d):\n";
    by_value(d);        // 切片！Derived 部分丢失
    std::cout << "by_reference(d):\n";
    by_reference(d);    // 不切片，保留多态

    Base b = d;         // 赋值也切片
    std::cout << "Base b = d; b.foo() -> ";
    b.foo();
}

// 23 enum class vs enum
enum OldColor { Red, Green, Blue };          // 传统 enum，值泄漏到外层
enum class NewColor { Red, Green, Blue };    // enum class，强类型

void test_enum_class() {
    std::cout << "\n=== 23 enum class vs enum ===\n";

    int old = Red;  // 传统 enum 隐式转 int
    std::cout << "OldColor::Red 可隐式转 int: " << old << "\n";

    // int neu = NewColor::Red;  // 编译错误：enum class 禁止隐式转 int
    int neu = static_cast<int>(NewColor::Red);  // 必须显式转换
    std::cout << "NewColor::Red 必须 static_cast<int>: " << neu << "\n";

    // NewColor c = 1;  // 编译错误：int 不能隐式转 enum class
    std::cout << "enum class 阻止 if (color == 1) 这类脆弱代码\n";

    enum class SmallColor : std::uint8_t { Red, Green, Blue };
    std::cout << "sizeof(SmallColor) = " << sizeof(SmallColor)
              << " (可指定底层类型 uint8_t)\n";
}

int main() {
    test_polymorphism();
    test_enum_class();
    return 0;
}
