// 验证 ch03 原始内存管理与 RAII 主题：
// 1. new/delete 调用构造/析构，malloc/free 不调用
// 2. RAII 代理对象在异常时也能正确释放资源

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

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
    RawResource* release() {
        RawResource* p = ptr_;
        ptr_ = nullptr;
        return p;
    }

    RaiiGuard(const RaiiGuard&) = delete;
    RaiiGuard& operator=(const RaiiGuard&) = delete;

private:
    RawResource* ptr_;
};

void may_throw(bool throw_now) {
    RaiiGuard guard(new RawResource());
    if (throw_now) {
        throw std::runtime_error("boom");
    }
}

int main() {
    RawResource::constructed = 0;
    RawResource::destroyed = 0;

    {
        RawResource* p = new RawResource();
        delete p;
    }

    RawResource::constructed = 0;
    RawResource::destroyed = 0;

    {
        void* mem = std::malloc(sizeof(RawResource));
        RawResource* p = ::new (mem) RawResource();  // placement new
        p->~RawResource();
        std::free(mem);
    }

    RawResource::constructed = 0;
    RawResource::destroyed = 0;
    try {
        may_throw(true);
    } catch (const std::runtime_error& e) {
        std::cout << "caught: " << e.what() << "\n";
    }
    std::cout << "constructed: " << RawResource::constructed
              << ", destroyed: " << RawResource::destroyed << "\n";

    return 0;
}
