#include <iostream>
#include <limits>

int main() {
    // 1. 无符号整型加法回绕
    unsigned int max = std::numeric_limits<unsigned int>::max();
    unsigned int a = max - 1;
    unsigned int b = 3;
    std::cout << "=== 无符号整型加法回绕 ===\n";
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "a + b = " << (a + b) << "  <-- 回绕到 1\n\n";

    // 2. 无符号整型减法回绕
    unsigned int c = 2;
    unsigned int d = 5;
    std::cout << "=== 无符号整型减法回绕 ===\n";
    std::cout << "c = " << c << "\n";
    std::cout << "d = " << d << "\n";
    std::cout << "c - d = " << (c - d) << "  <-- 变成极大的正数\n\n";

    // 3. 无符号与有符号混合运算：int 提升为 unsigned
    unsigned int u = 10;
    int i = -42;
    std::cout << "=== unsigned + int 混合运算 ===\n";
    std::cout << "u = " << u << "\n";
    std::cout << "i = " << i << "\n";
    std::cout << "u + i = " << (u + i) << "  <-- 不是 -32，而是很大的正数\n\n";

    // 4. 向下递减循环的无限循环陷阱
    std::cout << "=== 向下递减循环陷阱 ===\n";
    std::cout << "for (unsigned i = 3; i >= 0; --i) 会无限循环，因为 i 减到 0 后继续回绕到 MAX\n";

    // 5. 有符号溢出（UB，仅供对比，不要依赖其行为）
    int max_int = std::numeric_limits<int>::max();
    std::cout << "=== 有符号整型溢出（UB，不要依赖） ===\n";
    std::cout << "max_int + 1 = " << (max_int + 1) << "  <-- 这是未定义行为\n";

    return 0;
}
