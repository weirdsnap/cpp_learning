// 验证 ch04 模板特化主题：
// 1. 类模板全特化 vs 偏特化
// 2. 函数模板全特化
// 3. 全特化以主模板身份参与重载决议

#include <iostream>
#include <string>

// 主模板
template<typename T>
struct TypeName {
    static std::string name() { return "generic"; }
};

// 偏特化：指针类型
template<typename T>
struct TypeName<T*> {
    static std::string name() { return "pointer"; }
};

// 全特化：int
template<>
struct TypeName<int> {
    static std::string name() { return "int"; }
};

// 函数模板主模板
template<typename T>
std::string func(T) { return "generic"; }

// 函数模板全特化
template<>
std::string func<int>(int) { return "int specialization"; }

int main() {
    std::cout << TypeName<double>::name() << "\n";   // generic
    std::cout << TypeName<int*>::name() << "\n";     // pointer (偏特化)
    std::cout << TypeName<int>::name() << "\n";      // int (全特化)

    std::cout << func(3.14) << "\n";                 // generic
    std::cout << func(42) << "\n";                   // int specialization

    return 0;
}
