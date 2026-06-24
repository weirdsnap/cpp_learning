// 验证 ch04 可变参数模板主题：
// 1. C++11 递归展开参数包
// 2. 递归中显式指定模板参数 T 导致的类型陷阱
// 3. C++17 折叠表达式

#include <iostream>
#include <string>

// C++11 递归打印
template<typename T>
void print_cxx11(T&& value) {
    std::cout << value << "\n";
}

template<typename T, typename... Rest>
void print_cxx11(T&& value, Rest&&... rest) {
    std::cout << value << " ";
    print_cxx11(std::forward<Rest>(rest)...);
}

// C++11 递归求和：显式指定 T
// 注意：第一次调用后 T 就被固定了，后续参数都会转成 T
template<typename T>
T sum_cxx11(T arg) {
    return arg;
}

template<typename T, typename... Args>
T sum_cxx11(T arg, Args... args) {
    return arg + sum_cxx11<T>(args...);
}

// C++17 折叠表达式：类型由操作数自然推导
template<typename... Args>
auto sum_cxx17(Args... args) {
    return (args + ...);  // 一元右折叠
}

template<typename... Args>
void print_cxx17(Args&&... args) {
    ((std::cout << args << " "), ...);  // 逗号折叠
    std::cout << "\n";
}

int main() {
    std::cout << "=== C++11 递归打印 ===\n";
    print_cxx11(1, 2.5, "hello");

    std::cout << "=== C++17 折叠打印 ===\n";
    print_cxx17(1, 2.5, "hello");

    std::cout << "=== C++11 递归求和（显式 T） ===\n";
    std::cout << "sum(0.5, 1, 0.5, 1) = " << sum_cxx11(0.5, 1, 0.5, 1) << "\n";
    std::cout << "sum(1, 0.5, 1, 0.5) = " << sum_cxx11(1, 0.5, 1, 0.5) << " (0.5 被截断为 0)\n";

    std::cout << "=== C++17 折叠求和（自然推导） ===\n";
    std::cout << "sum(1, 0.5, 1, 0.5) = " << sum_cxx17(1, 0.5, 1, 0.5) << "\n";

    return 0;
}
