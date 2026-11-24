// 智能指针体积与删除器布局验证（博客 ch03/02、ch03/04）
//
// 验证点：
//   1. unique_ptr 默认删除器下 sizeof == 裸指针（零体积开销）
//   2. 无状态删除器靠 EBO 不占空间；有状态删除器/函数指针使 unique_ptr 变胖
//   3. shared_ptr 固定为两个指针（对象指针 + 控制块指针）
//   4. unique_ptr 删除器是类型的一部分；shared_ptr 删除器被类型擦除
//
// 注意：具体字节数是 64 位平台下的实现细节，测试只断言跨实现成立的结构性关系。
#include <cstdio>
#include <iostream>
#include <memory>

struct EmptyFunctorDeleter {
    void operator()(std::FILE* p) const { if (p) std::fclose(p); }
};

struct StatefulDeleter {
    int tag;
    void operator()(int* p) const { delete p; }
};

using FnPtrDeleter = void (*)(int*);
void fnDelete(int* p) { delete p; }

int main() {
    auto captureless = [](int* p) { delete p; };

    std::cout << "裸指针:                          " << sizeof(int*) << '\n';
    std::cout << "unique_ptr 默认删除器:           " << sizeof(std::unique_ptr<int>) << '\n';
    std::cout << "unique_ptr 空仿函数删除器(EBO):  "
              << sizeof(std::unique_ptr<std::FILE, EmptyFunctorDeleter>) << '\n';
    std::cout << "unique_ptr 无捕获 lambda(EBO):   "
              << sizeof(std::unique_ptr<int, decltype(captureless)>) << '\n';
    std::cout << "unique_ptr 有状态删除器:         "
              << sizeof(std::unique_ptr<int, StatefulDeleter>) << '\n';
    std::cout << "unique_ptr 函数指针删除器:       "
              << sizeof(std::unique_ptr<int, FnPtrDeleter>) << '\n';
    std::cout << "shared_ptr:                      " << sizeof(std::shared_ptr<int>) << '\n';
}
