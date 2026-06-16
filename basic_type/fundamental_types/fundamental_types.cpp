#include <cstdint>
#include <iostream>
#include <limits>
#include <type_traits>

// 验证 ch01 中关于基础类型体系的知识点：
// 01 无符号整型回绕
// 02 char 的符号性
// 03 字面量的默认类型
// 04 字符串字面量的默认类型
// 25 获取类型的最大/最小值

int main() {
    {
        std::cout << "=== 01 无符号整型回绕 ===\n";
        unsigned int max = std::numeric_limits<unsigned int>::max();
        unsigned int a = max - 1;
        unsigned int b = 3;
        std::cout << "a = " << a << "\n";
        std::cout << "b = " << b << "\n";
        std::cout << "a + b = " << (a + b) << "  <-- 模 2^N 回绕到 1\n";

        unsigned int c = 2;
        unsigned int d = 5;
        std::cout << "c - d = " << (c - d) << "  <-- 回绕成极大正数\n";

        unsigned int u = 10;
        int i = -42;
        std::cout << "u + i = " << (u + i)
                  << "  <-- int 提升为 unsigned，不是 -32\n";

        std::cout << "for (unsigned i = 3; i >= 0; --i) 会无限循环（0 再减回绕到 MAX）\n";

        int max_int = std::numeric_limits<int>::max();
        std::cout << "max_int + 1 = " << (max_int + 1)
                  << "  <-- 有符号溢出是 UB，不要依赖\n\n";
    }

    std::cout << "=== 02 char 的符号性 ===\n";
    std::cout << "std::is_signed_v<char> = " << std::is_signed_v<char> << "\n";
    std::cout << "std::is_unsigned_v<char> = " << std::is_unsigned_v<char> << "\n";
    std::cout << "注意：char、signed char、unsigned char 是三种不同类型\n";
    std::cout << "std::is_same_v<char, signed char> = "
              << std::is_same_v<char, signed char> << "\n\n";

    std::cout << "=== 03 字面量的默认类型 ===\n";
    auto i = 1;        // int
    auto d = 1.1;      // double，不是 float
    auto f = 1.1f;     // float
    auto ld = 1.1L;    // long double
    auto u = 42U;      // unsigned int
    auto ll = 42LL;    // long long

    std::cout << "1    -> " << typeid(i).name() << " (int)\n";
    std::cout << "1.1  -> " << typeid(d).name() << " (double)\n";
    std::cout << "1.1f -> " << typeid(f).name() << " (float)\n";
    std::cout << "1.1L -> " << typeid(ld).name() << " (long double)\n";
    std::cout << "42U  -> " << typeid(u).name() << " (unsigned int)\n";
    std::cout << "42LL -> " << typeid(ll).name() << " (long long)\n\n";

    std::cout << "=== 04 字符串字面量的默认类型 ===\n";
    auto s = "abc";                 // 表达式中退化为 const char*
    decltype("abc") arr = "abc";    // 保留 const char[4]

    std::cout << "auto s = \"abc\"      -> " << typeid(s).name() << " (const char*)\n";
    std::cout << "decltype(\"abc\")    -> " << typeid(arr).name() << " (const char[4])\n";
    std::cout << "sizeof(\"abc\")      = " << sizeof("abc") << " (含末尾 \\0)\n";
    std::cout << "sizeof(s)            = " << sizeof(s) << " (指针大小)\n\n";

    std::cout << "=== 25 获取类型的最大/最小值 ===\n";
    std::cout << "int max:    " << std::numeric_limits<int>::max() << "\n";
    std::cout << "int min:    " << std::numeric_limits<int>::min() << "\n";
    std::cout << "uint max:   " << std::numeric_limits<unsigned>::max() << "\n";
    std::cout << "size_t max: " << std::numeric_limits<size_t>::max() << "\n";
    std::cout << "double lowest (最小负值): " << std::numeric_limits<double>::lowest() << "\n";
    std::cout << "double min    (最小正值): " << std::numeric_limits<double>::min() << "\n";
    std::cout << "float epsilon: " << std::numeric_limits<float>::epsilon() << "\n";

    return 0;
}
