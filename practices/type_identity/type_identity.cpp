// 验证 type_identity 专题 + std::function 与 lambda 关系：
// 1. type_identity_t 把参数变成非推导语境，修复 lambda -> std::function 推导失败
// 2. type_identity_t 阻止 auto 退化 / 强制两参数同类型
// 3. lambda 闭包类型唯一、std::function 类型固定；转换单向
// 4. std::function 的 SBO 缓冲大小实测

#include <functional>
#include <iostream>
#include <type_traits>

// ============================================================
// 1. 推导失败与 type_identity 修复
// ============================================================

// 原题写法：T 需要从 std::function<void(T)> 推导，lambda 类型不匹配 -> 推导失败
// template <typename T>
// void call_with(std::function<void(T)> f, T val) { f(val); }

// 修复：嵌套名限定 type_identity<...>::type 是非推导语境（[temp.deduct.type]/5）
// T 只从 val 推导；f 的类型确定后，lambda 靠隐式转换装入 std::function
template <typename T>
void call_with(std::type_identity_t<std::function<void(T)>> f, T val) {
    f(val);
}

// ============================================================
// 2. type_identity 的另外两个用途
// ============================================================

// 用途一：强制两个参数同类型（不加则 T、U 各自独立推导）
template <typename T>
T clamp_same(T v, std::type_identity_t<T> lo, std::type_identity_t<T> hi) {
    return v < lo ? lo : (hi < v ? hi : v);
}

// 用途二：作为标签类型传递类型信息（tag dispatching 的零开销形式）
template <typename T>
constexpr size_t size_of_tag(std::type_identity<T>) {
    return sizeof(T);
}

// ============================================================
// 3. lambda 与 std::function 的类型关系
// ============================================================

void demo_types() {
    auto l1 = [](int x) { return x + 1; };
    auto l2 = [](int x) { return x + 1; };  // 同样的签名和函数体

    // 每个 lambda 表达式都有唯一的闭包类型
    static_assert(!std::is_same_v<decltype(l1), decltype(l2)>);

    // std::function 的类型只由签名决定，与装入的可调用对象无关
    std::function<int(int)> f1 = l1;
    std::function<int(int)> f2 = l2;
    static_assert(std::is_same_v<decltype(f1), decltype(f2)>);

    // lambda -> std::function 是单向的用户定义转换，没有回头路
    static_assert(std::is_convertible_v<decltype(l1), std::function<int(int)>>);
    static_assert(!std::is_convertible_v<std::function<int(int)>, decltype(l1)>);

    std::cout << "sizeof(空捕获 lambda) = " << sizeof(l1) << "\n";

    int big[16] = {};
    auto l3 = [big](int x) { return x + big[0]; };
    std::cout << "sizeof(大捕获 lambda) = " << sizeof(l3) << "\n";
    std::cout << "sizeof(std::function) = " << sizeof(f1) << "\n";
}

// ============================================================
// 4. SBO：小对象优化缓冲的边界
// ============================================================

void demo_sbo() {
    auto small = [] { return 1; };
    std::function<int()> f_small = small;

    // libstdc++ 的 std::function 内部缓冲为 16 字节（两个指针）
    int buf[8] = {};
    auto big = [buf] { return buf[0]; };  // 32 字节捕获，超出 SBO
    std::function<int()> f_big = big;

    std::cout << "SBO 内？small: " << std::boolalpha
              << (sizeof(small) <= 16) << ", big: " << (sizeof(big) <= 16) << "\n";
    std::cout << "sizeof(f_small) == sizeof(f_big): "
              << (sizeof(f_small) == sizeof(f_big)) << "\n";  // 类型固定 -> 大小固定
}

int main() {
    auto print = [](int x) { std::cout << x << "\n"; };

    std::cout << "== type_identity 修复推导 ==\n";
    call_with(print, 42);  // T=int 从 val 推导

    std::cout << "== clamp_same 强制同类型 ==\n";
    std::cout << clamp_same(5, 1, 3) << "\n";  // 3
    // clamp_same(5, 1, 3.0);  // ❌ T=int 已确定，double 不能隐式收窄... 其实能转，但试试 long:
    // clamp_same(5, 1L, 3L);  // ❌ 编译错误：long 不能转成 int 的 type_identity 参数?（实际可转换，留作讨论）

    std::cout << "== tag 用法 ==\n";
    std::cout << size_of_tag(std::type_identity<double>{}) << "\n";  // 8

    std::cout << "== lambda vs std::function 类型 ==\n";
    demo_types();

    std::cout << "== SBO 边界 ==\n";
    demo_sbo();
}
