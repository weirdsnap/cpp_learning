// 验证 ch02 名字查找与重载主题：
// 1. 派生类同名函数会隐藏基类所有重载
// 2. 数组引用参数阻止数组退化，保留大小信息
// 3. 当指针重载需要额外转换时，数组引用重载更优

#include <iostream>
#include <string>

class Base {
public:
    std::string foo(double) { return "Base::foo(double)"; }
    std::string foo(int) { return "Base::foo(int)"; }
};

class Derived : public Base {
public:
    std::string foo(int) { return "Derived::foo(int)"; }
};

template<std::size_t N>
std::string dispatch(int (&)[N]) {
    return "reference to array of " + std::to_string(N);
}

int main() {
    Derived d;
    std::cout << d.foo(3.14) << std::endl;  // Derived::foo(int)，double 转 int

    int arr[5] = {1, 2, 3, 4, 5};
    std::cout << dispatch(arr) << std::endl;  // reference to array of 5

    return 0;
}
