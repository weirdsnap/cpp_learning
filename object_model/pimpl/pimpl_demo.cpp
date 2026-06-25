// 验证 ch02/16 Pimpl 惯用法
//
// 演示 Pimpl 的编译隔离和 ABI 稳定性

#include "widget.h"

#include <iostream>

int main() {
    std::cout << "=== Pimpl 惯用法演示 ===\n\n";

    Widget w;
    std::cout << "  Widget created\n";
    w.draw();
    std::cout << "  value() = " << w.value() << "\n\n";

    // 移动语义
    Widget w2 = std::move(w);
    std::cout << "  Widget moved\n";
    w2.draw();

    return 0;
}
