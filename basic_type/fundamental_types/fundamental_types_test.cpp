#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "fundamental_types_utils.h"

#include <climits>
#include <cstdint>
#include <string>
#include <type_traits>

TEST_CASE("无符号整型回绕") {
    unsigned int max = ~0u;
    unsigned int a = max - 1;
    unsigned int b = 3;
    CHECK((a + b) == 1);  // 模 2^32 回绕到 1

    unsigned int c = 2, d = 5;
    CHECK((c - d) == (max - 2));  // 回绕成极大正数

    unsigned int u = 10;
    int i = -42;
    CHECK((u + i) == (10u + static_cast<unsigned int>(-42)));
}

TEST_CASE("char / signed char / unsigned char 是三种独立类型") {
    CHECK_FALSE(std::is_same_v<char, signed char>);
    CHECK_FALSE(std::is_same_v<char, unsigned char>);
    CHECK_FALSE(std::is_same_v<signed char, unsigned char>);
}

TEST_CASE("整型字面量默认类型") {
    CHECK((std::is_same_v<decltype(42), int>));
    CHECK((std::is_same_v<decltype(052), int>));
    CHECK_EQ(052, 42);  // 八进制 52 = 十进制 42
    CHECK_EQ(0x2A, 42); // 十六进制 2A = 十进制 42
    CHECK_EQ(0b101010, 42); // 二进制
}

TEST_CASE("整型后缀改变类型") {
    CHECK((std::is_same_v<decltype(42U), unsigned int>));
    CHECK((std::is_same_v<decltype(42L), long>));
    CHECK((std::is_same_v<decltype(42LL), long long>));
    CHECK((std::is_same_v<decltype(42ULL), unsigned long long>));
}

TEST_CASE("浮点字面量默认类型") {
    CHECK((std::is_same_v<decltype(3.14), double>));
    CHECK((std::is_same_v<decltype(3.14f), float>));
    CHECK((std::is_same_v<decltype(3.14L), long double>));
}

TEST_CASE("字符串字面量的类型与行为") {
    CHECK(sizeof("abc") == 4);  // 含 \0
    CHECK((std::is_same_v<decltype("abc"), const char(&)[4]>));

    const char* joined = "hello " "world";
    CHECK(std::string(joined) == "hello world");

    const char* raw = R"(\d+\.\d+)";
    CHECK(std::string(raw) == R"(\d+\.\d+)");
}

TEST_CASE("用户自定义字面量 UDL") {
    auto d1 = 5.0_km;
    auto d2 = 300.0_m;
    CHECK(d1.meters == doctest::Approx(5000.0L));
    CHECK(d2.meters == doctest::Approx(300.0L));
}
