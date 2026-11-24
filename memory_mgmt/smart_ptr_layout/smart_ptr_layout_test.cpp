// 智能指针体积与删除器布局的结构性断言
//
// 只断言跨实现（libstdc++ / libc++ / MSVC）都成立的关系，
// 不断言具体字节数（那是实现细节）。
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdio>
#include <memory>
#include <type_traits>

struct EmptyFunctorDeleter {
    void operator()(std::FILE* p) const { if (p) std::fclose(p); }
};

struct StatefulDeleter {
    int tag;
    void operator()(int* p) const { delete p; }
};

using FnPtrDeleter = void (*)(int*);

TEST_CASE("unique_ptr 默认删除器：体积等于裸指针") {
    CHECK(sizeof(std::unique_ptr<int>) == sizeof(int*));
}

TEST_CASE("无状态删除器靠 EBO 不占空间") {
    CHECK(std::is_empty<EmptyFunctorDeleter>::value);
    CHECK(sizeof(std::unique_ptr<std::FILE, EmptyFunctorDeleter>) == sizeof(std::FILE*));
    auto captureless = [](int* p) { delete p; };
    CHECK(std::is_empty<decltype(captureless)>::value);
    CHECK(sizeof(std::unique_ptr<int, decltype(captureless)>) == sizeof(int*));
}

TEST_CASE("有状态删除器与函数指针使 unique_ptr 变胖") {
    CHECK(sizeof(std::unique_ptr<int, StatefulDeleter>) > sizeof(int*));
    CHECK(sizeof(std::unique_ptr<int, FnPtrDeleter>) > sizeof(int*));
}

TEST_CASE("shared_ptr 固定为两个指针大小") {
    CHECK(sizeof(std::shared_ptr<int>) == 2 * sizeof(int*));
}

TEST_CASE("unique_ptr 删除器是类型的一部分") {
    using Default = std::unique_ptr<int>;
    using Custom = std::unique_ptr<int, StatefulDeleter>;
    CHECK_FALSE(std::is_same<Default, Custom>::value);
}

TEST_CASE("shared_ptr 删除器被类型擦除：不影响自身类型") {
    std::shared_ptr<int> a(new int(1));
    std::shared_ptr<int> b(new int(2), [](int* p) { delete p; });
    CHECK(std::is_same<decltype(a), decltype(b)>::value);
}

TEST_CASE("自定义删除器确实被调用") {
    int deleted = 0;
    {
        auto counting = [&deleted](int* p) { ++deleted; delete p; };
        std::shared_ptr<int> sp(new int(42), counting);
        CHECK(deleted == 0);
    }
    CHECK(deleted == 1);
}
