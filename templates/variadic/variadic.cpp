// 验证 ch04 可变参数模板主题：
// 1. 递归展开参数包
// 2. C++17 折叠表达式

#include <iostream>
#include <string>

// C++11 递归展开
template<typename T>
void print_cxx11(T&& value) {
    std::cout << value << "\n";
}

template<typename T, typename... Rest>
void print_cxx11(T&& value, Rest&&... rest) {
    std::cout << value << " ";
    print_cxx11(std::forward<Rest>(rest)...);
}

// C++17 折叠表达式
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // 一元右折叠
}

template<typename... Args>
void print_cxx17(Args&&... args) {
    ((std::cout << args << " "), ...);  // 逗号折叠
    std::cout << "\n";
}

int main() {
    print_cxx11(1, 2.5, "hello");
    print_cxx17(1, 2.5, "hello");
    std::cout << "sum = " << sum(1, 2, 3, 4, 5) << "\n";
    return 0;
}
