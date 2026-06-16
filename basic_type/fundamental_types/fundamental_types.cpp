#include <chrono>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>

// 验证 ch01 中关于基础类型体系的知识点：
// 01 无符号整型回绕
// 02 char 的符号性
// 03 C++ 字面量与类型
// 04 字符串字面量的特殊行为与陷阱

// 02 char 符号性：重载示例
void f(char)             { std::cout << "  f(char)\n"; }
void f(signed char)      { std::cout << "  f(signed char)\n"; }
void f(unsigned char)    { std::cout << "  f(unsigned char)\n"; }

// 03 UDL 示例
struct Distance {
    long double meters;
};

Distance operator"" _km(long double value) {
    return Distance{value * 1000.0L};
}
Distance operator"" _m(long double value) {
    return Distance{value};
}

std::ostream& operator<<(std::ostream& os, const Distance& d) {
    return os << d.meters << "m";
}

// 01 有符号溢出检查：编译器优化陷阱
bool check_overflow(int x) {
    if (x + 1 < x) {   // 程序员想检查加法是否溢出
        return true;   // 有符号溢出是 UB，编译器认为 x+1<x 恒假
    }
    return false;
}

int main() {
    std::cout << "=== 01 无符号整型回绕 ===\n";
    {
        unsigned int max = ~0u;
        unsigned int a = max - 1;
        unsigned int b = 3;
        std::cout << "a + b = " << (a + b) << "  <-- 模 2^32 回绕到 1\n";

        unsigned int c = 2, d = 5;
        std::cout << "c - d = " << (c - d) << "  <-- 回绕成极大正数\n";

        unsigned int u = 10;
        int i = -42;
        std::cout << "u + i = " << (u + i)
                  << "  <-- int 提升为 unsigned，不是 -32\n";

        std::cout << "for (unsigned i = 3; i >= 0; --i) 会无限循环\n";
    }

    std::cout << "\n有符号溢出检查被编译器优化删除的陷阱:\n";
    std::cout << "check_overflow(INT_MAX) = " << check_overflow(INT_MAX)
              << " (优化后恒为 false)\n";
    std::cout << "需要检查溢出时应使用无符号类型、更大类型或 __builtin_add_overflow\n\n";

    std::cout << "=== 02 char 的符号性 ===\n";
    std::cout << "std::is_signed_v<char> = " << std::is_signed_v<char> << "\n";
    std::cout << "char / signed char / unsigned char 是三种独立类型\n";
    std::cout << "std::is_same_v<char, signed char> = "
              << std::is_same_v<char, signed char> << "\n";
    char c = 'A';
    std::cout << "f('A') 调用: ";
    f(c);
    std::cout << "f((signed char)'A') 调用: ";
    f(static_cast<signed char>(c));
    std::cout << "f((unsigned char)'A') 调用: ";
    f(static_cast<unsigned char>(c));
    std::cout << "\n";

    std::cout << "=== 03 C++ 字面量与类型 ===\n";
    std::cout << "整型字面量:\n";
    std::cout << "42    (十进制) -> " << typeid(42).name() << "\n";
    std::cout << "052   (八进制) -> " << typeid(052).name() << " = " << 052 << "\n";
    std::cout << "0x2A  (十六进制) -> " << typeid(0x2A).name() << " = " << 0x2A << "\n";
    std::cout << "0b101010 (二进制) -> " << typeid(0b101010).name() << " = " << 0b101010 << "\n";

    std::cout << "\n后缀改变类型:\n";
    std::cout << "42    -> " << typeid(42).name() << "\n";
    std::cout << "42U   -> " << typeid(42U).name() << "\n";
    std::cout << "42L   -> " << typeid(42L).name() << "\n";
    std::cout << "42LL  -> " << typeid(42LL).name() << "\n";
    std::cout << "42ULL -> " << typeid(42ULL).name() << "\n";

    std::cout << "\n浮点字面量:\n";
    std::cout << "3.14   -> " << typeid(3.14).name() << " (double)\n";
    std::cout << "3.14f  -> " << typeid(3.14f).name() << " (float)\n";
    std::cout << "3.14L  -> " << typeid(3.14L).name() << " (long double)\n";

    std::cout << "\n标准库 UDL:\n";
    using namespace std::chrono_literals;
    using namespace std::string_literals;
    auto t1 = 60s;
    auto s1 = "hello"s;
    std::cout << "60s   -> " << typeid(t1).name() << "\n";
    std::cout << "\"hello\"s -> " << typeid(s1).name() << "\n";

    std::cout << "\n自定义 UDL:\n";
    auto dist1 = 5.0_km;
    auto dist2 = 300.0_m;
    std::cout << "5.0_km = " << dist1 << "\n";
    std::cout << "300.0_m = " << dist2 << "\n\n";

    std::cout << "=== 04 字符串字面量的特殊行为与陷阱 ===\n";
    std::cout << "sizeof(\"abc\") = " << sizeof("abc") << " (含 \\0)\n";
    std::cout << "auto s = \"abc\" -> " << typeid("abc").name() << " (const char*)\n";
    decltype("abc") arr = "abc";
    std::cout << "decltype(\"abc\") -> " << typeid(arr).name() << " (const char[4])\n";

    std::cout << "\n相邻字符串自动连接:\n";
    const char* joined = "hello " "world";
    std::cout << "\"hello \" \"world\" = " << joined << "\n";

    std::cout << "\n原始字符串字面量:\n";
    const char* regex1 = "\\d+\\.\\d+";       // 普通字符串需转义
    const char* regex2 = R"(\d+\.\d+)";      // 原始字符串更直观
    std::cout << "普通: " << regex1 << "\n";
    std::cout << "原始: " << regex2 << "\n";

    std::cout << "\n宽字符与 UTF 字符串:\n";
    const wchar_t*  ws = L"中文";
    const char16_t* u16 = u"中文";
    const char32_t* u32 = U"中文";
    (void)ws; (void)u16; (void)u32;  // 仅作类型展示
    std::cout << "L\"...\"  -> wchar_t*\n";
    std::cout << "u\"...\"  -> char16_t*\n";
    std::cout << "U\"...\"  -> char32_t*\n";

    return 0;
}
