// 验证 ch07/01 printf 类型不安全 + ch07/06 std::format 类型安全 + ch07/07 std::print/println
//
// 1. printf 类型不匹配导致 UB
// 2. std::format 编译期类型检查
// 3. 对齐、精度、进制格式化
// 4. std::print / std::println (C++23)

#include <cstdio>
#include <iostream>
#include <format>
#include <string>
#include <string_view>

// ============================================================
// 1. printf 类型不安全
// ============================================================

void demo_printf_unsafe() {
    std::cout << "=== 1. printf 类型不安全 ===\n";

    // 正确用法
    std::printf("  %%d for int: %d\n", 42);
    std::printf("  %%f for double: %f\n", 3.14);
    std::printf("  %%s for string: %s\n", "hello");

    // 类型不匹配：UB（注释掉避免实际运行 UB）
    // std::printf("%d\n", 3.14);   // UB：%d 期望 int，给了 double
    // std::printf("%f\n", 42);     // UB：%f 期望 double，给了 int
    // std::printf("%s\n", 42);     // UB：%s 期望 char*，给了 int（大概率崩溃）

    std::cout << "  printf 类型不匹配是 UB，编译器无法阻止\n";
    std::cout << "  GCC/Clang -Wformat 只能检查部分情况\n";
}

// ============================================================
// 2. std::format 类型安全
// ============================================================

struct Point {
    int x, y;
};

template <>
struct std::formatter<Point> {
    constexpr auto parse(auto& ctx) { return ctx.begin(); }
    auto format(const Point& p, auto& ctx) const {
        return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

void demo_format_safe() {
    std::cout << "\n=== 2. std::format 类型安全 ===\n";

    // 基本用法
    std::string s = std::format("  Error {}: {}\n", 404, "Not Found");
    std::cout << s;

    // 编译期类型检查：以下代码编译失败
    // std::format("{:d}", 3.14);  // ❌ 格式说明符要求整数，给了 double

    // 自定义类型格式化
    Point p{10, 20};
    std::cout << std::format("  Point: {}\n", p);

    // format_to：写入迭代器避免额外分配
    std::string buf;
    std::format_to(std::back_inserter(buf), "  {} + {} = {}", 1, 2, 3);
    std::cout << buf << "\n";
}

// ============================================================
// 3. 对齐、精度、进制格式化
// ============================================================

void demo_format_spec() {
    std::cout << "\n=== 3. 格式化说明符 ===\n";

    // 对齐
    std::cout << std::format("  |{:>10}|\n", "right");    // 右对齐
    std::cout << std::format("  |{:<10}|\n", "left");     // 左对齐
    std::cout << std::format("  |{:^10}|\n", "center");   // 居中
    std::cout << std::format("  |{:*^10}|\n", "hi");      // 填充字符

    // 精度
    std::cout << std::format("  {:.2f}\n", 3.14159);      // 两位小数
    std::cout << std::format("  {:.5f}\n", 3.14159);      // 五位小数

    // 进制
    std::cout << std::format("  dec: {}  hex: {:x}  HEX: {:X}  oct: {:o}  bin: {:b}\n",
                             255, 255, 255, 255, 255);
    std::cout << std::format("  hex with prefix: {:#x}\n", 255);
    std::cout << std::format("  zero-padded: {:08b}\n", 42);

    // 宽度 + 精度组合
    std::cout << std::format("  {:>10.2f}\n", 3.14159);
}

// ============================================================
// 4. std::print / std::println (C++23)
// ============================================================

void demo_print() {
    std::cout << "\n=== 4. std::print / std::println (C++23) ===\n";

#if __cplusplus >= 202302L
    std::print("  std::print: {} + {} = {}\n", 1, 2, 3);
    std::println("  std::println: Hello, {}!", "world");
    std::println("  Pi is approximately {:.5f}", 3.1415926535);
#else
    std::cout << "  std::print/println 需要 C++23 (当前: C++" << __cplusplus << ")\n";
    std::cout << "  使用 std::format 模拟:\n";
    std::cout << std::format("  std::print: {} + {} = {}\n", 1, 2, 3);
    std::cout << std::format("  std::println: Hello, {}!\n", "world");
#endif
}

int main() {
    demo_printf_unsafe();
    demo_format_safe();
    demo_format_spec();
    demo_print();

    return 0;
}
