// 验证 ch02 NVI（Non-Virtual Interface）惯用法：
// 公开非虚函数控制流程，内部委托给私有/保护虚函数实现。

#include <iostream>
#include <string>

class Shape {
public:
    // 公开非虚接口，控制调用前后逻辑
    std::string draw() const {
        std::string result = prepare();
        result += do_draw();
        result += cleanup();
        return result;
    }

    virtual ~Shape() = default;

protected:
    virtual std::string do_draw() const = 0;

private:
    std::string prepare() const { return "pre-"; }
    std::string cleanup() const { return "-post"; }
};

class Circle : public Shape {
protected:
    std::string do_draw() const override { return "Circle"; }
};

class Square : public Shape {
protected:
    std::string do_draw() const override { return "Square"; }
};

int main() {
    Circle c;
    Square s;
    std::cout << c.draw() << std::endl;  // pre-Circle-post
    std::cout << s.draw() << std::endl;  // pre-Square-post
    return 0;
}
