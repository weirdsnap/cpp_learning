#include <functional>
#include <iostream>
#include <vector>

// 验证 ch01 中关于函数对象与求值顺序的知识点：
// 05 隐式转换 bool / operator bool
// 16 函数指针与仿函数
// 17 Lambda 捕获：值、引用、this 和陷阱
// 18 函数参数求值顺序未指定
// 19 花括号列表 vs 圆括号参数求值顺序

// 05 explicit operator bool
class SafeHandle {
    bool valid_;
public:
    explicit SafeHandle(bool v) : valid_(v) {}
    explicit operator bool() const { return valid_; }
};

void test_operator_bool() {
    std::cout << "=== 05 explicit operator bool ===\n";
    SafeHandle h(true);
    if (h) {
        std::cout << "  if (h) OK：条件语境允许 explicit 转换\n";
    }

    // bool b = h;        // 编译错误：explicit 阻止隐式转换
    bool b = static_cast<bool>(h);  // OK：显式转换
    std::cout << "  static_cast<bool>(h) = " << b << "\n";

    // int x = h;         // 编译错误：不能隐式转 int
    // if (h == 1)        // 编译错误：不能用于算术比较
    std::cout << "  h == 1 编译错误：explicit operator bool 不参与算术比较\n\n";
}

// 16 函数指针与仿函数
int add_one(int x) { return x + 1; }

struct Adder {
    int offset = 0;
    int operator()(int x) const { return x + offset; }
};

template <typename F>
void apply_to_all(const std::vector<int>& v, F f) {
    for (int x : v) {
        std::cout << f(x) << " ";
    }
    std::cout << "\n";
}

void test_function_vs_functor() {
    std::cout << "=== 16 函数指针与仿函数 ===\n";
    std::vector<int> v = {1, 2, 3};

    // 函数指针
    int (*fp)(int) = add_one;
    std::cout << "  函数指针: ";
    apply_to_all(v, fp);

    // 仿函数（带状态）
    Adder adder{10};
    std::cout << "  仿函数 (offset=10): ";
    apply_to_all(v, adder);

    // Lambda（匿名仿函数）
    int factor = 2;
    std::cout << "  Lambda (factor=2): ";
    apply_to_all(v, [factor](int x) { return x * factor; });
    std::cout << "\n";
}

// 17 Lambda 捕获：值、引用、this 和陷阱
int g_lambda_global = 1;

void test_lambda_capture() {
    std::cout << "=== 17 Lambda 捕获 ===\n";

    // 按值捕获：复制一份，外部变化不影响 lambda 内部
    int x = 10;
    auto f1 = [x]() {
        // x = 20;  // 编译错误：默认 lambda operator() 是 const
        return x;
    };
    x = 100;
    std::cout << "  按值捕获，外部 x 改为 100 后 lambda 仍返回 " << f1() << "\n";

    // mutable：修改闭包内部副本，外部不变
    auto f2 = [x]() mutable {
        x = 20;
        return x;
    };
    std::cout << "  mutable lambda 内 x 改为 " << f2() << "，外部 x 为 " << x << "\n";

    // 按引用捕获：lambda 内修改会影响外部
    int y = 10;
    auto f3 = [&y]() {
        y = 30;
    };
    f3();
    std::cout << "  按引用捕获后外部 y 变为 " << y << "\n";

    // 全局变量：不需要也不能捕获
    auto f4 = [](int b) { return g_lambda_global + b; };
    std::cout << "  lambda 中使用全局变量 g_lambda_global: " << f4(4) << "\n";

    // 隐式捕获
    int a = 1, b = 2;
    auto f5 = [=]() { return a + b; };
    auto f6 = [&]() { a++; b++; };
    f6();
    std::cout << "  隐式值捕获 [=] 结果 " << f5() << "，隐式引用捕获 [&] 后 a+b=" << (a + b) << "\n\n";
}

int global_counter = 0;
int next_id() { return ++global_counter; }

void print_two(int a, int b) {
    std::cout << "  a=" << a << ", b=" << b << "\n";
}

// 18 函数参数求值顺序未指定
void test_eval_order() {
    std::cout << "=== 18 函数参数求值顺序未指定 ===\n";
    global_counter = 0;
    std::cout << "  print_two(next_id(), next_id()) 结果（顺序由编译器决定）:\n";
    print_two(next_id(), next_id());
    std::cout << "  再次调用可能顺序不同:\n";
    global_counter = 0;
    print_two(next_id(), next_id());
    std::cout << "\n";
}

// 19 花括号列表 vs 圆括号参数求值顺序
void test_brace_eval_order() {
    std::cout << "=== 19 花括号列表 vs 圆括号参数求值顺序 ===\n";
    global_counter = 0;
    std::cout << "  圆括号 (next_id(), next_id(), next_id()) 顺序未指定:\n";
    print_two(next_id(), next_id());  // 只有两参，加一行单独演示

    global_counter = 0;
    std::cout << "  花括号 {next_id(), next_id(), next_id()} 保证从左到右:\n";
    std::vector<int> v = {next_id(), next_id(), next_id()};
    for (int x : v) std::cout << "  " << x;
    std::cout << "\n";
}

int main() {
    test_operator_bool();
    test_function_vs_functor();
    test_lambda_capture();
    test_eval_order();
    test_brace_eval_order();
    return 0;
}
