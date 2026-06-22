// 验证 ch04 模板参数推导主题：
// 1. 按值传递时顶层 const 被剥离
// 2. 按引用/指针传递时顶层 const 保留
// 3. auto 单独使用会剥离引用和顶层 const

#include <iostream>
#include <type_traits>

template<typename T>
void by_value(T /*x*/) {
    std::cout << "by_value T=" << typeid(T).name()
              << " const? " << std::is_const_v<T> << "\n";
}

template<typename T>
void by_ref(const T& /*x*/) {
    std::cout << "by_ref T=" << typeid(T).name()
              << " const? " << std::is_const_v<T> << "\n";
}

template<typename T>
void by_ptr(T* /*x*/) {
    std::cout << "by_ptr T=" << typeid(T).name()
              << " const? " << std::is_const_v<T> << "\n";
}

int main() {
    const int ci = 42;
    by_value(ci);   // T = int
    by_ref(ci);     // T = int（const 在引用上）

    int x = 0;
    by_ptr(&x);     // T = int
    const int* cp = &ci;
    by_ptr(cp);     // T = const int

    int& ref = x;
    auto a = ref;   // auto 剥离引用：a 是 int
    static_assert(std::is_same_v<decltype(a), int>);

    return 0;
}
