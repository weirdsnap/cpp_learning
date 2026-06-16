#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <typeinfo>

// 验证 ch01/25：NULL、nullptr 与 void*

void foo(int) {
    std::cout << "  foo(int)\n";
}

void foo(char*) {
    std::cout << "  foo(char*)\n";
}

void only_nullptr(std::nullptr_t) {
    std::cout << "  only_nullptr(std::nullptr_t)\n";
}

int main() {
    std::cout << "=== 25 NULL / nullptr / void* ===\n";

    std::cout << "重载解析差异:\n";
    std::cout << "foo(0):    ";
    foo(0);             // 调用 foo(int)
    // foo(NULL);       // 可能调用 foo(int)，取决于 NULL 定义
    std::cout << "foo(nullptr): ";
    foo(nullptr);       // 明确调用 foo(char*)

    std::cout << "\nnullptr 的类型:\n";
    std::cout << "typeid(nullptr).name() = " << typeid(nullptr).name() << "\n";
    std::cout << "std::is_same_v<decltype(nullptr), std::nullptr_t> = "
              << std::is_same_v<decltype(nullptr), std::nullptr_t> << "\n";

    std::cout << "\nnullptr 不能转 int:\n";
    // int x = nullptr;  // 编译错误
    std::cout << "int x = nullptr;  // 编译错误\n";

    std::cout << "\n可以专门重载只接受 nullptr 的函数:\n";
    only_nullptr(nullptr);  // OK

    std::cout << "\nvoid* 是通用对象指针:\n";
    void* vp = std::malloc(100);
    if (vp) {
        // char* cp = vp;     // C++ 中编译错误：不能隐式转换
        char* cp = static_cast<char*>(vp);  // 必须显式转换
        *cp = 'A';
        std::cout << "  static_cast<char*>(vp)[0] = " << *cp << "\n";
        // void* next = vp + 1;  // 编译错误：void* 不能做指针算术
        std::cout << "  void* 不能做 ++ 或 *p 操作\n";
        std::free(vp);
    }

    return 0;
}
