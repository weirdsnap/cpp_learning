#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <format>
#include <string>

TEST_CASE("std::format 基本用法") {
    std::string s = std::format("Error {}: {}", 404, "Not Found");
    CHECK(s == "Error 404: Not Found");
}

TEST_CASE("std::format 对齐") {
    CHECK(std::format("|{:>10}|", "right") == "|     right|");
    CHECK(std::format("|{:<10}|", "left") == "|left      |");
    CHECK(std::format("|{:^10}|", "center") == "|  center  |");
    CHECK(std::format("|{:*^10}|", "hi") == "|****hi****|");
}

TEST_CASE("std::format 精度") {
    CHECK(std::format("{:.2f}", 3.14159) == "3.14");
    CHECK(std::format("{:.5f}", 3.14159) == "3.14159");
}

TEST_CASE("std::format 进制") {
    CHECK(std::format("{:x}", 255) == "ff");
    CHECK(std::format("{:X}", 255) == "FF");
    CHECK(std::format("{:o}", 255) == "377");
    CHECK(std::format("{:b}", 255) == "11111111");
    CHECK(std::format("{:#x}", 255) == "0xff");
}

TEST_CASE("std::format 宽度+精度组合") {
    CHECK(std::format("{:>10.2f}", 3.14159) == "      3.14");
}

TEST_CASE("std::format_to 写入迭代器") {
    std::string buf;
    std::format_to(std::back_inserter(buf), "{} + {} = {}", 1, 2, 3);
    CHECK(buf == "1 + 2 = 3");
}

// 自定义类型格式化
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

TEST_CASE("std::format 自定义类型") {
    Point p{10, 20};
    CHECK(std::format("{}", p) == "(10, 20)");
}
