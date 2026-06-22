#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>

template<typename T>
struct TypeName {
    static std::string name() { return "generic"; }
};

template<typename T>
struct TypeName<T*> {
    static std::string name() { return "pointer"; }
};

template<>
struct TypeName<int> {
    static std::string name() { return "int"; }
};

template<typename T>
std::string func(T) { return "generic"; }

template<>
std::string func<int>(int) { return "int specialization"; }

TEST_CASE("类模板主模板") {
    CHECK(TypeName<double>::name() == "generic");
}

TEST_CASE("类模板偏特化匹配指针") {
    CHECK(TypeName<int*>::name() == "pointer");
    CHECK(TypeName<const char*>::name() == "pointer");
}

TEST_CASE("类模板全特化优先于偏特化") {
    CHECK(TypeName<int>::name() == "int");
}

TEST_CASE("函数模板全特化以主模板身份参与重载") {
    CHECK(func(3.14) == "generic");
    CHECK(func(42) == "int specialization");
}
