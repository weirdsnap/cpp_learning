#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <type_traits>

class Base {
public:
    virtual int value() const { return 1; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    int extra = 42;
    int value() const override { return 2; }
};

TEST_CASE("通过基类指针/引用调用虚函数") {
    Derived d;
    Base* p = &d;
    Base& r = d;

    CHECK(p->value() == 2);
    CHECK(r.value() == 2);
}

TEST_CASE("按值赋值导致对象切片") {
    Derived d;
    Base b = d;  // 切片
    CHECK(b.value() == 1);  // 不再是 Derived 的实现
}

TEST_CASE("enum class 禁止隐式转 int") {
    enum class Color { Red, Green, Blue };
    Color c = Color::Red;

    // int x = c;  // 编译错误
    int x = static_cast<int>(c);
    CHECK(x == 0);

    CHECK_FALSE((std::is_convertible_v<Color, int>));
}

TEST_CASE("enum class 可指定底层类型") {
    enum class SmallColor : unsigned char { Red };
    CHECK(sizeof(SmallColor) == sizeof(unsigned char));
}

TEST_CASE("传统 enum 可隐式转 int") {
    enum OldColor { Red, Green };
    int x = Red;
    CHECK(x == 0);
}
