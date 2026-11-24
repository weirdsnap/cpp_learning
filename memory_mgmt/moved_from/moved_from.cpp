// 移后对象（moved-from state）行为观察（博客 ch03/08）
//
// 标准保证：移后对象处于"有效但未指定"状态，只能析构或重新赋值。
// 各类型具体表现不同，本 demo 逐一观察。
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main() {
    // trivial 类型没有移动构造，move 就是拷贝，源值不变
    int a = 42;
    int b = std::move(a);
    (void)b;
    std::cout << "int 移后值: " << a << "（不变，移动即拷贝）\n";

    // unique_ptr：标准明确保证变空
    auto p = std::make_unique<int>(7);
    auto q = std::move(p);
    std::cout << "unique_ptr 移后为空: " << (p == nullptr) << "（标准保证）\n";

    // vector：实现上通常变空，但标准只说"有效但未指定"
    std::vector<int> v{1, 2, 3};
    auto w = std::move(v);
    std::cout << "vector 移后 size: " << v.size() << "（未指定，别依赖）\n";

    // 移后重新赋值：合法且必须能工作
    p = std::make_unique<int>(9);
    v = {4, 5};
    std::cout << "重新赋值后: *p=" << *p << ", v.size()=" << v.size() << '\n';
}
