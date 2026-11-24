// 移后对象的 doctest 断言
//
// 只断言标准保证的行为：
//   - unique_ptr 移后必为 nullptr（标准明确规定）
//   - trivial 类型移动即拷贝，源不变
//   - 移后对象可析构、可重新赋值
// 不断言 vector/string 移后为空（那是实现细节，标准不保证）。
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

TEST_CASE("trivial 类型移动即拷贝，源值不变") {
    int a = 42;
    int b = std::move(a);
    CHECK(a == 42);
    CHECK(b == 42);
}

TEST_CASE("unique_ptr 移后为空是标准保证") {
    auto p = std::make_unique<int>(7);
    auto q = std::move(p);
    CHECK(p == nullptr);
    CHECK(*q == 7);
}

TEST_CASE("移后对象可以重新赋值并继续使用") {
    auto p = std::make_unique<int>(1);
    auto q = std::move(p);
    p = std::make_unique<int>(9);  // 移后赋值：合法
    CHECK(*p == 9);

    std::string s = "hello";
    std::string t = std::move(s);
    s = "world";  // 移后赋值：合法
    CHECK(s == "world");
}

TEST_CASE("vector 移后仍有效：可调用无前置条件的操作") {
    std::vector<int> v{1, 2, 3};
    auto w = std::move(v);
    v.clear();          // 无前置条件的成员函数：合法
    v.push_back(10);    // 重新填充：合法
    CHECK(v.size() == 1);
    CHECK(v[0] == 10);
    CHECK(w.size() == 3);
}
