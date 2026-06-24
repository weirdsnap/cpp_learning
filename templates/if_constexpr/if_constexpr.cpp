// 验证 ch04/25：if constexpr 的编译期分支与 IFNDR 陷阱
// 注意：本文件只编译合法用法，IFNDR 反例以注释形式保留。

#include <iostream>
#include <string>
#include <type_traits>

// 合法用法 1：按类型返回不同值，返回类型 auto 依赖 T
template<typename T>
auto describe(T t) {
    if constexpr (std::is_integral_v<T>) {
        return std::to_string(t) + " is integral";
    } else {
        return std::string(t) + " is not integral";
    }
}

// 合法用法 2：用 if constexpr 替代递归终止
template<typename T, typename... Rest>
void print_all(T value, Rest... rest) {
    std::cout << value;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << " ";
        print_all(rest...);
    } else {
        std::cout << "\n";
    }
}

// 合法用法 3：返回类型固定为 int，else 分支也合法
template<typename T>
int safe_f()
{
    if constexpr (std::is_same_v<T, int>) { return 0; }
    else { return 42; }   // 与返回类型一致
}

// IFNDR 反例（不要编译）：
// else 分支里的 std::string{} 不依赖 T，在模板定义期就会被判定为非法。
//
// template<typename T>
// int bad_f()
// {
//     if constexpr (std::is_same_v<T, int>) { return 0; }
//     else { return std::string{}; }  // ill-formed, IFNDR
// }

int main() {
    std::cout << "=== if constexpr 按类型分派 ===\n";
    std::cout << describe(42) << "\n";
    std::cout << describe(std::string{"hello"}) << "\n";

    std::cout << "\n=== if constexpr 替代递归终止 ===\n";
    print_all(1, 2.5, "end");

    std::cout << "\n=== 合法的分支都返回 int ===\n";
    std::cout << "safe_f<int>() = " << safe_f<int>() << "\n";
    std::cout << "safe_f<double>() = " << safe_f<double>() << "\n";

    return 0;
}
