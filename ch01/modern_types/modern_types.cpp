#include <iostream>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

// 验证 ch01 中关于现代类型特性的知识点：
// 20 std::variant
// 21 右值引用与转发引用
// 22 std::optional

// 20 std::variant
void test_variant() {
    std::cout << "=== 20 std::variant ===\n";
    std::variant<int, double, std::string> v = 42;
    std::cout << "  index = " << v.index() << ", value = " << std::get<int>(v) << "\n";

    v = 3.14;
    std::cout << "  index = " << v.index() << ", value = " << std::get<double>(v) << "\n";

    v = "hello";
    std::cout << "  index = " << v.index() << ", value = " << std::get<std::string>(v) << "\n";

    // std::get_if 安全访问
    if (auto p = std::get_if<int>(&v)) {
        std::cout << "  当前是 int: " << *p << "\n";
    } else {
        std::cout << "  当前不是 int\n";
    }

    // std::visit 处理所有可能类型
    std::visit([](auto&& arg) {
        std::cout << "  visit 看到: " << arg << "\n";
    }, v);

    // 错误访问会抛异常
    try {
        std::get<int>(v);  // v 当前是 std::string
    } catch (const std::bad_variant_access& e) {
        std::cout << "  错误访问抛出: " << e.what() << "\n";
    }
    std::cout << "\n";
}

// 21 右值引用与转发引用
void foo(int&& x) {
    std::cout << "  右值引用版本，x = " << x << "\n";
}

// 转发引用：模板参数推导中的 T&&
template <typename T>
void forwarder(T&& x) {
    (void)x;  // 仅用于类型推导展示
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "  转发引用绑定到左值，T = " << typeid(T).name() << "\n";
    } else {
        std::cout << "  转发引用绑定到右值，T = " << typeid(T).name() << "\n";
    }
}

void test_forwarding_reference() {
    std::cout << "=== 21 右值引用与转发引用 ===\n";
    int i = 42;

    foo(42);            // OK：绑定右值
    // foo(i);          // 编译错误：int&& 不能绑定左值

    forwarder(i);                // T = int&, x = int&
    forwarder(42);               // T = int,  x = int&&
    forwarder(std::move(i));     // T = int,  x = int&&
    std::cout << "\n";
}

// 22 std::optional
std::optional<int> safe_divide(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

void test_optional() {
    std::cout << "=== 22 std::optional ===\n";

    auto r1 = safe_divide(10, 2);
    if (r1) {
        std::cout << "  10 / 2 = " << *r1 << "\n";
    }

    auto r2 = safe_divide(10, 0);
    if (!r2) {
        std::cout << "  10 / 0 -> std::nullopt\n";
    }

    std::optional<int> empty;
    std::cout << "  empty.has_value() = " << empty.has_value() << "\n";
    std::cout << "  empty.value_or(-1) = " << empty.value_or(-1) << "\n";

    // optional<T&> 不合法
    // std::optional<int&> ref_opt;  // 编译错误
    std::cout << "  optional<T&> 不合法，应使用 optional<T*> 或 reference_wrapper\n";
}

int main() {
    test_variant();
    test_forwarding_reference();
    test_optional();
    return 0;
}
