#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <climits>
#include <limits>
#include <type_traits>
#include <typeinfo>

// 重载选择探针：根据 x + y 的实际类型返回不同字符
char g(unsigned int) { return 'u'; }
char g(int)          { return 'i'; }
char g(char)         { return 'c'; }

TEST_CASE("unsigned short 减法的整数提升") {
    unsigned short zero = 0, one = 1;
    auto diff = zero - one;

    if constexpr (sizeof(unsigned short) < sizeof(int)) {
        // int 能表示所有 unsigned short 的值，提升为 int
        CHECK(std::is_same_v<decltype(diff), int>);
        CHECK(diff == -1);
        CHECK((zero - one < zero) == true);
    } else {
        // unsigned short 与 int 同宽，提升为 unsigned int
        CHECK(std::is_same_v<decltype(diff), unsigned int>);
        CHECK(diff == std::numeric_limits<unsigned int>::max());
        CHECK((zero - one < zero) == false);
    }
}

TEST_CASE("char 加法的整数提升与重载选择") {
    char x = 1, y = 2;
    auto s = x + y;

    if constexpr (sizeof(char) == sizeof(int) && std::is_unsigned_v<char>) {
        // 无符号 char 与 int 同宽，int 放不下所有 unsigned char 值
        CHECK(std::is_same_v<decltype(s), unsigned int>);
        CHECK(g(x + y) == 'u');
    } else {
        // 常见情况：char 提升为 int
        CHECK(std::is_same_v<decltype(s), int>);
        CHECK(g(x + y) == 'i');
    }
}

TEST_CASE("通常算术转换：unsigned int + int") {
    unsigned int u = 10;
    int i = -42;
    // int 先转成 unsigned int，-42 对应 max()-41，再加 10 得 max()-31
    CHECK((u + i) == (std::numeric_limits<unsigned int>::max() - 31));
}
