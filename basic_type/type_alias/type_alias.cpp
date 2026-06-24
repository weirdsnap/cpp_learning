// 验证 ch01/29：类型别名 using / typedef
// 演示函数类型别名、函数指针、数组别名、成员指针别名、别名模板

#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

// 函数类型别名可直接用于成员函数声明
using Func = int();

struct S {
    Func f;  // 等价于 int f();
};

int S::f() { return 1; }

// 函数指针类型别名
using FuncPtr = int(*)();

int free_func() { return 1; }

// 数组类型别名
using IntArray = int[10];

// 成员指针别名
struct Obj {
    int x;
    int g() { return 2; }
};
using MemberVar  = int Obj::*;
using MemberFunc = int (Obj::*)();

// 别名模板
template<typename T>
using Vec = std::vector<T>;

template<typename K, typename V>
using Map = std::map<K, V>;

int main() {
    std::cout << "=== 函数类型别名声明成员函数 ===\n";
    S s;
    std::cout << "s.f() = " << s.f() << "\n";

    std::cout << "\n=== 函数类型 vs 函数指针类型 ===\n";
    FuncPtr p = &free_func;
    std::cout << "(*p)() = " << (*p)() << "\n";
    static_assert(std::is_same_v<Func*, FuncPtr>);

    std::cout << "\n=== 数组类型别名 ===\n";
    IntArray arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << "sizeof(arr) = " << sizeof(arr) << "\n";
    static_assert(std::is_same_v<IntArray, int[10]>);

    std::cout << "\n=== 成员指针别名 ===\n";
    MemberVar  mv = &Obj::x;
    MemberFunc mf = &Obj::g;
    Obj o{42};
    std::cout << "o.*mv = " << o.*mv << "\n";
    std::cout << "(o.*mf)() = " << (o.*mf)() << "\n";

    std::cout << "\n=== 别名模板 ===\n";
    Vec<int> v = {1, 2, 3};
    Map<std::string, int> m = {{"a", 1}};
    std::cout << "Vec size = " << v.size() << ", Map size = " << m.size() << "\n";

    std::cout << "\n=== 别名不创建新类型 ===\n";
    using MyInt = int;
    std::cout << "is_same<MyInt, int> = " << std::is_same_v<MyInt, int> << "\n";
    std::cout << "typeid equal = " << (typeid(MyInt) == typeid(int)) << "\n";

    return 0;
}
