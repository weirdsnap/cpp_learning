#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <optional>
#include <stdexcept>
#include <variant>

TEST_CASE("std::variant 正确访问") {
    std::variant<int, double, std::string> v = 42;
    CHECK(v.index() == 0);
    CHECK(std::get<int>(v) == 42);

    v = 3.14;
    CHECK(v.index() == 1);
    CHECK(std::get<double>(v) == doctest::Approx(3.14));

    v = "hello";
    CHECK(v.index() == 2);
    CHECK(std::get<std::string>(v) == "hello");
}

TEST_CASE("std::variant 错误访问抛 bad_variant_access") {
    std::variant<int, double> v = 3.14;
    CHECK_THROWS_AS(std::get<int>(v), std::bad_variant_access);
}

TEST_CASE("std::variant get_if 安全访问") {
    std::variant<int, double> v = 3.14;
    CHECK(std::get_if<int>(&v) == nullptr);
    CHECK(std::get_if<double>(&v) != nullptr);
}

TEST_CASE("std::optional 持有值与空状态") {
    std::optional<int> o;
    CHECK_FALSE(o.has_value());
    CHECK_FALSE(o);
    CHECK(o.value_or(-1) == -1);

    o = 42;
    CHECK(o.has_value());
    CHECK(*o == 42);
    CHECK(o.value() == 42);
}

TEST_CASE("std::optional 空时 value() 抛 bad_optional_access") {
    std::optional<int> o;
    CHECK_THROWS_AS(o.value(), std::bad_optional_access);
}
