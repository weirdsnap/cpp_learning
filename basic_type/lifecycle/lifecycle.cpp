#include <iostream>
#include <type_traits>

// 验证 ch01 中关于生命周期与声明/定义的知识点：
// 13 构造与析构顺序
// 14 声明、定义与初始化（const vs constexpr）
// 15 返回局部/临时变量的指针/引用

struct Tracer {
    const char* name;
    Tracer(const char* n) : name(n) {
        std::cout << "  构造: " << name << "\n";
    }
    ~Tracer() {
        std::cout << "  析构: " << name << "\n";
    }
};

class Demo {
public:
    // 注意：成员按类内声明顺序构造，与初始化列表顺序无关
    Tracer m1;
    Tracer m2;
    Tracer m3;

    Demo() : m3("m3"), m1("m1"), m2("m2") {
        std::cout << "  Demo 构造函数体\n";
    }
    ~Demo() {
        std::cout << "  Demo 析构函数体（成员按声明逆序析构）\n";
    }
};

// 13 构造/析构顺序验证
void test_lifecycle_order() {
    std::cout << "=== 13 构造与析构顺序 ===\n";
    std::cout << "成员声明顺序：m1 -> m2 -> m3\n";
    std::cout << "初始化列表顺序：m3 -> m1 -> m2\n";
    std::cout << "实际构造顺序应为声明顺序 m1 -> m2 -> m3:\n";
    Demo d;
    std::cout << "Demo 对象生命周期结束，开始析构:\n";
}

// constexpr 函数既可在编译期调用，也可在运行期调用
constexpr int square(int x) { return x * x; }

// 14 const vs constexpr
void test_const_constexpr() {
    std::cout << "\n=== 14 const vs constexpr ===\n";

    const int runtime_const = 42;  // 可能是编译期，也可能是运行期
    constexpr int compile_const = 42;  // 必须是编译期

    // constexpr 可用于需要编译期常量的场景
    int arr[compile_const];  // OK：数组大小需要编译期常量
    (void)arr;  // 仅作编译期验证，运行时未使用
    // int arr2[runtime_const];  // 可能报错，取决于编译器扩展

    std::cout << "runtime_const is_const = " << std::is_const_v<decltype(runtime_const)>
              << ", value = " << runtime_const << "\n";
    std::cout << "compile_const is_const = " << std::is_const_v<decltype(compile_const)>
              << ", value = " << compile_const << "\n";
    std::cout << "数组大小使用 constexpr: OK\n";

    constexpr int r = square(5);
    std::cout << "constexpr square(5) = " << r << "\n";

    int n = 5;
    int r2 = square(n);  // constexpr 函数也可以在运行期调用
    std::cout << "square(n) 运行期结果 = " << r2 << "\n";
}

// 15 返回局部变量的指针/引用（危险，不要这样做）
int* dangling_ptr() {
    int local = 42;
    return &local;  // 返回局部变量地址，悬垂指针
}

int& dangling_ref() {
    int local = 42;
    return local;  // 返回局部变量引用，悬垂引用
}

void test_dangling() {
    std::cout << "\n=== 15 返回局部变量的指针/引用 ===\n";
    std::cout << "dangling_ptr() 和 dangling_ref() 都返回局部对象的地址/引用\n";
    std::cout << "访问结果是未定义行为，可能暂时\"正常\"，也可能崩溃\n";
    // int* p = dangling_ptr();  // 危险！
    // std::cout << *p << "\n";  // UB
}

int main() {
    test_lifecycle_order();
    test_const_constexpr();
    test_dangling();
    return 0;
}
