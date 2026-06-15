#include <iostream>
#include <type_traits>
#include <typeinfo>

// 验证 ch01 中关于类型推导的知识点：
// 06 auto 的类型推导
// 07 decltype 的推导规则
// 08 decltype(auto)
// 09 表达式中的类型退化

void takes_array(int arr[5]) {
    // 函数参数中的数组会退化为指针
    std::cout << "函数参数 int[5] 退化为指针: " << typeid(arr).name() << "\n";
}

int add(int a, int b) { return a + b; }

int main() {
    std::cout << "=== 06 auto 的类型推导 ===\n";
    int x = 42;
    int& rx = x;
    const int cx = 100;

    auto a1 = x;       // int
    auto a2 = rx;      // int（引用剥离）
    auto a3 = cx;      // int（顶层 const 剥离）
    auto& a4 = cx;     // const int&（auto 推导 int，加 & 后 const 保留）
    auto arr = "hi";   // const char*（数组退化）

    std::cout << "auto a1 = x       -> " << typeid(a1).name() << "\n";
    std::cout << "auto a2 = rx      -> " << typeid(a2).name() << " (引用被剥离)\n";
    std::cout << "auto a3 = cx      -> " << typeid(a3).name() << " (顶层 const 被剥离)\n";
    std::cout << "auto& a4 = cx     -> " << typeid(a4).name() << "\n";
    std::cout << "auto arr = \"hi\"   -> " << typeid(arr).name() << " (数组退化为指针)\n\n";

    std::cout << "=== 07 decltype 的推导规则 ===\n";
    int i = 42;
    decltype(i) b1 = i;     // int（标识符表达式）
    decltype((i)) b2 = i;   // int&（表达式语境，i 是左值）

    std::cout << "decltype(i)        -> " << typeid(b1).name() << " (int)\n";
    std::cout << "decltype((i))      -> " << typeid(b2).name() << " (int&)\n";
    std::cout << "is_reference_v<decltype((i))> = "
              << std::is_reference_v<decltype((i))> << "\n\n";

    std::cout << "=== 08 decltype(auto) ===\n";
    int& ri = i;
    auto c1 = ri;                    // int
    decltype(auto) c2 = ri;          // int&
    decltype(auto) c3 = "abc";       // const char[4]，保留数组类型

    std::cout << "auto c1 = ri           -> " << typeid(c1).name() << " (int)\n";
    std::cout << "decltype(auto) c2 = ri -> " << typeid(c2).name() << " (int&)\n";
    std::cout << "decltype(auto) c3 = \"abc\" -> " << typeid(c3).name() << "\n";
    std::cout << "sizeof(c3) = " << sizeof(c3) << " (数组大小，非指针)\n\n";

    std::cout << "=== 09 表达式中的类型退化 ===\n";
    int local_arr[5] = {1, 2, 3, 4, 5};
    std::cout << "sizeof(local_arr)      = " << sizeof(local_arr) << " (数组总字节)\n";

    int* p = local_arr;  // 数组名在大多数表达式中退化为指针
    std::cout << "sizeof(p)              = " << sizeof(p) << " (指针大小)\n";
    takes_array(local_arr);

    std::cout << "std::decay_t<int[5]>   = " << typeid(std::decay_t<int[5]>).name() << "\n";
    std::cout << "std::decay_t<int()>    = " << typeid(std::decay_t<int()>).name() << "\n";

    return 0;
}
