#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <sstream>

TEST_CASE("bool 默认输出 0/1") {
    std::ostringstream oss;
    oss << true << false;
    CHECK(oss.str() == "10");
}

TEST_CASE("boolalpha 输出 true/false") {
    std::ostringstream oss;
    oss << std::boolalpha << true << false;
    CHECK(oss.str() == "truefalse");
}

TEST_CASE("char 输出字符而非 ASCII 码") {
    std::ostringstream oss;
    oss << 'A';
    CHECK(oss.str() == "A");
}

TEST_CASE("char 显式转 int 输出 ASCII 码") {
    std::ostringstream oss;
    oss << static_cast<int>('A');
    CHECK(oss.str() == "65");
}

TEST_CASE("const char* 输出字符串") {
    std::ostringstream oss;
    oss << "hello";
    CHECK(oss.str() == "hello");
}

TEST_CASE("const char* 转 void* 输出地址") {
    std::ostringstream oss;
    const char* s = "hello";
    oss << static_cast<const void*>(s);
    // 地址格式不固定，只验证非空
    CHECK_FALSE(oss.str().empty());
}

TEST_CASE("运算符优先级：1+2<<3 = 24") {
    int x = 1 + 2 << 3;
    CHECK(x == 24);  // (1+2) << 3
}

TEST_CASE("运算符优先级：位运算需括号") {
    int flags = 0x02;
    // 错误：flags & 0x02 == 0x02 解析为 flags & (0x02 == 0x02)
    int wrong = flags & 0x02 == 0x02;
    CHECK(wrong == 0);  // flags & 1 = 0

    // 正确
    CHECK(((flags & 0x02) == 0x02));
}
