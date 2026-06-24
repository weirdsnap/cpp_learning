#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <map>
#include <string>
#include <type_traits>
#include <vector>

using Func = int();

struct S {
    Func f;
};

int S::f() { return 1; }

using FuncPtr = int(*)();

int free_func() { return 1; }
using IntArray = int[10];

struct Obj {
    int x;
    int g() { return 2; }
};
using MemberVar  = int Obj::*;
using MemberFunc = int (Obj::*)();

template<typename T>
using Vec = std::vector<T>;

TEST_CASE("函数类型别名声明成员函数") {
    S s;
    CHECK(s.f() == 1);
}

TEST_CASE("函数类型与函数指针类型的关系") {
    static_assert(std::is_same_v<Func*, FuncPtr>);
    FuncPtr p = &free_func;
    CHECK((*p)() == 1);
}

TEST_CASE("数组类型别名") {
    static_assert(std::is_same_v<IntArray, int[10]>);
    IntArray arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    CHECK(sizeof(arr) == sizeof(int) * 10);
    CHECK(arr[3] == 3);
}

TEST_CASE("成员指针别名") {
    MemberVar  mv = &Obj::x;
    MemberFunc mf = &Obj::g;
    Obj o{42};
    CHECK(o.*mv == 42);
    CHECK((o.*mf)() == 2);
}

TEST_CASE("别名模板") {
    static_assert(std::is_same_v<Vec<int>, std::vector<int>>);
    Vec<int> v = {1, 2, 3};
    CHECK(v.size() == 3);

    using IntMap = std::map<std::string, int>;
    static_assert(std::is_same_v<std::map<std::string, int>, IntMap>);
}

TEST_CASE("类型别名不创建新类型") {
    using MyInt = int;
    static_assert(std::is_same_v<MyInt, int>);
    CHECK(typeid(MyInt) == typeid(int));
}
