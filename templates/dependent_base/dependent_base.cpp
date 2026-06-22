// 验证 ch04 模板基类中的依赖名查找：
// 在模板派生类中，非限定名称不会自动到依赖基类 Base<T> 中查找。

#include <iostream>

void func() { std::cout << "global func\n"; }

template<typename T>
struct Base {
    void func() { std::cout << "Base<T>::func\n"; }
    static constexpr int value = 42;
};

template<typename T>
struct Derived : Base<T> {
    void call_global() {
        func();  // 只会在全局作用域找，不会找 Base<T>::func
    }

    void call_base() {
        this->func();  // 通过 this 强制依赖查找，找到 Base<T>::func
    }

    void call_base_qualified() {
        Base<T>::func();  // 显式限定
    }

    int get_value() {
        // return value;  // 错误：找不到 value
        return this->value;  // OK
    }
};

int main() {
    Derived<int> d;
    d.call_global();        // global func
    d.call_base();          // Base<T>::func
    d.call_base_qualified(); // Base<T>::func
    std::cout << d.get_value() << "\n";  // 42
    return 0;
}
