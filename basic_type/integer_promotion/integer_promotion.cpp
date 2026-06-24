// 验证 ch01/28 整数提升与通常算术转换
// 演示 char/short 等窄整数类型在表达式中先提升为 int/unsigned int

#include <iostream>
#include <type_traits>
#include <typeinfo>

void f(unsigned int) { std::cout << "u"; }
void f(int)          { std::cout << "i"; }
void f(char)         { std::cout << "c"; }

int main() {
    std::cout << "=== unsigned short 减法 ===\n";
    unsigned short zero = 0, one = 1;
    auto diff = zero - one;
    std::cout << "zero - one = " << diff
              << ", type = " << typeid(diff).name() << "\n";
    std::cout << "sizeof(unsigned short) = " << sizeof(unsigned short)
              << ", sizeof(int) = " << sizeof(int) << "\n";
    if (zero - one < zero)
        std::cout << "输出：less\n";
    else
        std::cout << "输出：more\n";

    std::cout << "\n=== char 加法与重载选择 ===\n";
    char x = 1, y = 2;
    auto s = x + y;
    std::cout << "x + y = " << s
              << ", type = " << typeid(s).name() << "\n";
    std::cout << "std::is_signed_v<char> = " << std::is_signed_v<char>
              << ", sizeof(char) = " << sizeof(char)
              << ", sizeof(int) = " << sizeof(int) << "\n";
    std::cout << "f(x + y) 调用：";
    f(x + y);
    std::cout << "\n";

    std::cout << "\n=== 通常算术转换：有符号 + 无符号 ===\n";
    unsigned int u = 10;
    int i = -42;
    std::cout << "u + i = " << (u + i)
              << "  <-- int 被提升为 unsigned int\n";

    return 0;
}
