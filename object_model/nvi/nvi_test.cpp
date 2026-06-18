#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <memory>

class Shape {
public:
    std::string draw() const {
        return prefix() + do_draw() + suffix();
    }
    virtual ~Shape() = default;

protected:
    virtual std::string do_draw() const = 0;

private:
    std::string prefix() const { return "["; }
    std::string suffix() const { return "]"; }
};

class Circle : public Shape {
protected:
    std::string do_draw() const override { return "Circle"; }
};

class Square : public Shape {
protected:
    std::string do_draw() const override { return "Square"; }
};

TEST_CASE("NVI：基类非虚接口控制流程") {
    Circle c;
    CHECK(c.draw() == "[Circle]");

    Square s;
    CHECK(s.draw() == "[Square]");
}

TEST_CASE("NVI：多态调用仍保持基类控制逻辑") {
    std::unique_ptr<Shape> p = std::make_unique<Circle>();
    CHECK(p->draw() == "[Circle]");
}
