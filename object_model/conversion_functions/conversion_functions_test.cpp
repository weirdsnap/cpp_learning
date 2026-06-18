#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <type_traits>

class SafeBool {
public:
    explicit operator bool() const { return true; }
};

class Ambiguous {
public:
    operator int() const { return 1; }
    operator long() const { return 2L; }
};

class MutableOnly {
public:
    operator int() { return 42; }
};

class ConstCorrect {
public:
    operator int() const { return 24; }
};

std::string accept_int(int) { return "int"; }
std::string accept_long(long) { return "long"; }

TEST_CASE("explicit 转换函数阻止隐式转换") {
    static_assert(!std::is_convertible_v<SafeBool, bool>);
}

TEST_CASE("explicit 转换函数在布尔上下文和 static_cast 中可用") {
    SafeBool sb;
    CHECK(static_cast<bool>(sb) == true);

    bool result = false;
    if (sb) { result = true; }
    CHECK(result == true);
}

TEST_CASE("多个转换函数在需要标准转换时产生二义性") {
    Ambiguous a;
    // 必须显式指定目标类型以避免二义性
    CHECK(accept_int(static_cast<int>(a)) == "int");
    CHECK(accept_long(static_cast<long>(a)) == "long");
}

TEST_CASE("const 转换函数可在临时对象/const 对象上调用") {
    ConstCorrect cc;
    CHECK(accept_int(cc) == "int");
    CHECK(accept_int(ConstCorrect()) == "int");
}

TEST_CASE("非 const 转换函数只能在可变对象上调用") {
    MutableOnly mo;
    CHECK(accept_int(mo) == "int");
}
