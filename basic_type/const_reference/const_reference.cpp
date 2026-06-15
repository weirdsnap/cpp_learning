#include <algorithm>
#include <iostream>
#include <type_traits>
#include <typeinfo>

// 验证 ch01 中关于 const 引用与返回引用的知识点：
// 10 const 引用绑定与临时对象
// 11 std::max/min 返回引用

int main() {
    std::cout << "=== 10 const 引用绑定与临时对象 ===\n";
    double d = 3.14;
    const int& r = d;  // 产生 int 临时对象，r 绑定到临时对象而非 d

    std::cout << "d  = " << d << "\n";
    std::cout << "r  = " << r << " (绑定到从 d 转换来的临时 int)\n";

    d = 2.71;
    std::cout << "修改 d 后:\n";
    std::cout << "d  = " << d << "\n";
    std::cout << "r  = " << r << " (仍保持原临时对象的值 3)\n";

    // 非 const 引用无法绑定到需要转换的表达式
    // int& bad = d;  // 编译错误
    std::cout << "int& r2 = d;  // 编译错误：非 const 引用不能绑定到临时对象\n\n";

    std::cout << "=== 11 std::max/min 返回引用 ===\n";
    int a = 10, b = 20;

    // std::max 返回 const T&，是原对象的别名
    // std::max(a, b) = 99;  // 编译错误：返回的是 const 引用，不能赋值
    const int& m = std::max(a, b);
    std::cout << "max(a, b) = " << m << " (返回 b 的引用)\n";

    // 危险：传入临时对象，返回悬垂引用
    const int& bad_ref = std::max(10, 20);
    std::cout << "std::max(10, 20) 返回临时对象的引用，此处可能已悬垂\n";
    std::cout << "bad_ref = " << bad_ref << " (可能正常也可能异常，不要依赖)\n";

    // initializer_list 版本返回的是值，不是引用
    auto mm = std::max({1, 2, 3});
    std::cout << "std::max({1,2,3}) = " << mm << " (initializer_list 版本返回 T 值)\n";

    return 0;
}
