// 验证 ch04 万能引用与完美转发主题：
// 1. T&& 模板参数是万能引用，可绑定左值和右值
// 2. std::forward<T> 保持原始值类别

#include <iostream>
#include <string>
#include <utility>

struct Resource {
    Resource() { std::cout << "default\n"; }
    Resource(const Resource&) { std::cout << "copy\n"; }
    Resource(Resource&&) noexcept { std::cout << "move\n"; }
};

template<typename T>
void wrapper_bad(T&& x) {
    // 未使用 forward，x 始终是左值
    consume(x);
}

template<typename T>
void wrapper_good(T&& x) {
    // 完美转发：保持调用时的值类别
    consume(std::forward<T>(x));
}

void consume(Resource) {}

int main() {
    Resource r;
    std::cout << "--- bad wrapper ---\n";
    wrapper_bad(r);              // 实际仍是左值，但 consume 会拷贝
    wrapper_bad(Resource());     // 临时对象变左值，consume 会拷贝

    std::cout << "--- good wrapper ---\n";
    wrapper_good(r);             // 左值 -> 拷贝
    wrapper_good(Resource());    // 右值 -> 移动

    return 0;
}
