#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdlib>
#include <memory>
#include <stdexcept>

class RawResource {
public:
    static int constructed;
    static int destroyed;
    RawResource() { ++constructed; }
    ~RawResource() { ++destroyed; }
};
int RawResource::constructed = 0;
int RawResource::destroyed = 0;

class RaiiGuard {
public:
    explicit RaiiGuard(RawResource* p) : ptr_(p) {}
    ~RaiiGuard() { delete ptr_; }
    RawResource* get() const { return ptr_; }
    RaiiGuard(const RaiiGuard&) = delete;
    RaiiGuard& operator=(const RaiiGuard&) = delete;
private:
    RawResource* ptr_;
};

TEST_CASE("new/delete 自动调用构造和析构") {
    RawResource::constructed = 0;
    RawResource::destroyed = 0;
    {
        RawResource* p = new RawResource();
        CHECK(RawResource::constructed == 1);
        delete p;
        CHECK(RawResource::destroyed == 1);
    }
}

TEST_CASE("malloc/free 不调用构造和析构") {
    RawResource::constructed = 0;
    RawResource::destroyed = 0;
    void* mem = std::malloc(sizeof(RawResource));
    CHECK(RawResource::constructed == 0);

    RawResource* p = ::new (mem) RawResource();
    CHECK(RawResource::constructed == 1);

    p->~RawResource();
    CHECK(RawResource::destroyed == 1);
    std::free(mem);
}

TEST_CASE("RAII 代理在异常时也能释放资源") {
    RawResource::constructed = 0;
    RawResource::destroyed = 0;

    try {
        RaiiGuard guard(new RawResource());
        CHECK(RawResource::constructed == 1);
        throw std::runtime_error("boom");
    } catch (const std::runtime_error&) {
        // 异常导致栈展开，guard 析构释放资源
    }

    CHECK(RawResource::destroyed == 1);
}

TEST_CASE("智能指针是 RAII 代理的标准实现") {
    RawResource::constructed = 0;
    RawResource::destroyed = 0;
    {
        auto up = std::make_unique<RawResource>();
        CHECK(RawResource::constructed == 1);
    }
    CHECK(RawResource::destroyed == 1);
}
