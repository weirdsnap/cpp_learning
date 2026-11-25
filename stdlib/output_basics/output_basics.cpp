// 验证 ch07/11 cout 默认输出类型 + ch07/16 输出运算符与逻辑运算符优先级陷阱
//
// 1. bool / char / char* / nullptr 的默认输出行为
// 2. 运算符优先级陷阱

#include <iostream>

// ============================================================
// 1. cout 默认输出类型
// ============================================================

void demo_default_output() {
    std::cout << "=== 1. cout 默认输出类型 ===\n";

    // bool：输出 0/1，不是 true/false
    std::cout << "  bool true:  " << true << "\n";
    std::cout << "  bool false: " << false << "\n";
    std::cout << "  boolalpha:  " << std::boolalpha << true << "\n";

    // char：输出字符，不是 ASCII 码
    char c = 'A';
    std::cout << "  char 'A':         " << c << "\n";
    std::cout << "  char as int:      " << static_cast<int>(c) << "\n";

    // char*：输出 C 字符串，不是地址
    const char* str = "hello";
    std::cout << "  const char*:      " << str << "\n";
    std::cout << "  as void*:         " << static_cast<const void*>(str) << "\n";

    // nullptr_t：没有标准 operator<<
    // std::cout << nullptr;  // ❌ 编译错误
    std::cout << "  nullptr as void*: " << static_cast<const void*>(nullptr) << "\n";
}

// ============================================================
// 2. 运算符优先级陷阱
// ============================================================

void demo_operator_precedence() {
    std::cout << "\n=== 2. 运算符优先级陷阱 ===\n";

    int a = 1, b = 2, c = 3, d = 4;

    // 陷阱 1：cout << a < b
    // 解析为 (std::cout << a) < b，编译错误
    // std::cout << a < b;   // ERROR

    // 正确：加括号
    std::cout << "  (a < b) = " << (a < b) << "\n";

    // 陷阱 2：移位与算术
    int x = 1 + 2 << 3;   // 解析为 (1 + 2) << 3 = 24
    std::cout << "  1 + 2 << 3 = " << x << " (不是 1+(2<<3)=17)\n";

    // 陷阱 3：赋值与输出链
    int y;
    // std::cout << y = 42;  // ERROR：= 优先级低于 <<
    std::cout << "  (y = 42) = " << (y = 42) << "\n";

    // 陷阱 4：逻辑与比较
    if (a < b && c < d) {
        std::cout << "  a < b && c < d: true (比较优先级高于逻辑)\n";
    }

    // 陷阱 5：位运算与比较
    int flags = 0x02;
    // if (flags & 0x02 == 0x02)  // 解析为 flags & (0x02 == 0x02) = flags & 1
    if ((flags & 0x02) == 0x02) {
        std::cout << "  (flags & 0x02) == 0x02: true\n";
    }
}

int main() {
    demo_default_output();
    demo_operator_precedence();

    return 0;
}
