// 验证 ch02 类型转换函数主题：
// 1. explicit 转换函数阻止隐式转换但允许显式上下文
// 2. 转换函数 const 正确性
// 3. 多个转换函数可能产生二义性

#include <iostream>

class SafeBool {
public:
    explicit operator bool() const { return true; }
};

class Ambiguous {
public:
    operator int() const { return 1; }
    operator long() const { return 2L; }
};

class MutableOnly {
public:
    operator int() { return 42; }
};

std::string accept_int(int) { return "int"; }
std::string accept_long(long) { return "long"; }

int main() {
    SafeBool sb;
    if (sb) {  // explicit 转换函数允许布尔上下文
        std::cout << "true" << std::endl;
    }
    bool b = static_cast<bool>(sb);  // 显式转换允许
    std::cout << b << std::endl;

    Ambiguous a;
    // std::cout << accept_int(a) << std::endl;  // 二义性，无法编译
    std::cout << accept_int(static_cast<int>(a)) << std::endl;

    MutableOnly mo;
    std::cout << accept_int(mo) << std::endl;  // 临时对象需 const，此处是具名左值

    return 0;
}
