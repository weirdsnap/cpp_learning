#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <vector>
#include <new>
#include <cstring>
#include <type_traits>

// ============================================================
// ch08-1: noexcept 契约
// ============================================================

class NoexceptMovable {
public:
    std::vector<int> data;
    NoexceptMovable(NoexceptMovable&& other) noexcept
        : data(std::move(other.data)) {}
    NoexceptMovable() = default;
    NoexceptMovable(const NoexceptMovable&) = default;
};

class NonNoexceptMovable {
public:
    std::vector<int> data;
    NonNoexceptMovable(NonNoexceptMovable&& other)
        : data(std::move(other.data)) {}
    NonNoexceptMovable() = default;
    NonNoexceptMovable(const NonNoexceptMovable&) = default;
};

TEST_CASE("noexcept 移动构造标记") {
    CHECK(std::is_nothrow_move_constructible_v<NoexceptMovable>);
    CHECK_FALSE(std::is_nothrow_move_constructible_v<NonNoexceptMovable>);
}

TEST_CASE("noexcept 影响 vector 扩容策略") {
    // noexcept 移动构造：vector 扩容时使用移动
    std::vector<NoexceptMovable> vec1;
    vec1.reserve(1);
    vec1.push_back(NoexceptMovable{});
    CHECK(vec1.size() == 1);

    // 非 noexcept 移动构造：vector 扩容时回退到拷贝
    std::vector<NonNoexceptMovable> vec2;
    vec2.reserve(1);
    vec2.push_back(NonNoexceptMovable{});
    CHECK(vec2.size() == 1);
}

// ============================================================
// ch08-2: 析构函数抛出异常
// ============================================================

struct SafeDtor {
    bool destroyed = false;
    ~SafeDtor() noexcept {
        destroyed = true;
        // 析构内捕获所有异常
        try {
            // 模拟可能抛的操作
        } catch (...) {
            // 绝不传播
        }
    }
};

TEST_CASE("安全析构函数不传播异常") {
    SafeDtor sd;
    sd.~SafeDtor();
    CHECK(sd.destroyed);
}

// ============================================================
// ch09-1: 拷贝并交换惯用法（Copy-and-Swap）
// ============================================================

class StringBuffer {
public:
    StringBuffer() : data_(nullptr), size_(0) {}

    StringBuffer(const char* str) {
        size_ = std::strlen(str);
        data_ = new char[size_ + 1];
        std::strcpy(data_, str);
    }

    StringBuffer(const StringBuffer& other)
        : size_(other.size_) {
        data_ = new char[size_ + 1];
        std::strcpy(data_, other.data_);
    }

    StringBuffer(StringBuffer&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    ~StringBuffer() {
        delete[] data_;
    }

    // Copy-and-Swap 赋值
    StringBuffer& operator=(StringBuffer other) {
        swap(other);
        return *this;
    }

    void swap(StringBuffer& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    const char* c_str() const { return data_ ? data_ : ""; }
    size_t size() const { return size_; }

private:
    char* data_;
    size_t size_;
};

TEST_CASE("Copy-and-Swap 赋值正确") {
    StringBuffer a("Hello");
    StringBuffer b("World");

    a = b;
    CHECK(std::string(a.c_str()) == "World");
    CHECK(std::string(b.c_str()) == "World");  // b 不受影响
}

TEST_CASE("Copy-and-Swap 自赋值安全") {
    StringBuffer a("Hello");
    a = a;  // 自赋值不应崩溃
    CHECK(std::string(a.c_str()) == "Hello");
}

TEST_CASE("Copy-and-Swap 移动赋值") {
    StringBuffer a("Hello");
    StringBuffer b("World");

    a = std::move(b);
    CHECK(std::string(a.c_str()) == "World");
    // b 被移动，处于有效但未指定状态
}

// ============================================================
// ch09-2: placement new 与显式析构
// ============================================================

struct alignas(64) PoolObject {
    int id;
    std::string name;
    bool alive = true;

    PoolObject(int i, const std::string& n) : id(i), name(n) {}
    ~PoolObject() { alive = false; }
};

class ObjectPool {
public:
    ObjectPool() : pool_(new char[sizeof(PoolObject) * 8]), count_(0) {}

    ~ObjectPool() {
        for (size_t i = 0; i < count_; ++i) {
            reinterpret_cast<PoolObject*>(pool_ + i * sizeof(PoolObject))->~PoolObject();
        }
        delete[] pool_;
    }

    template <typename... Args>
    PoolObject* construct(Args&&... args) {
        REQUIRE(count_ < 8);
        void* ptr = pool_ + count_ * sizeof(PoolObject);
        PoolObject* obj = ::new (ptr) PoolObject(std::forward<Args>(args)...);
        ++count_;
        return obj;
    }

private:
    char* pool_;
    size_t count_;
};

TEST_CASE("placement new 在预分配内存上构造对象") {
    alignas(PoolObject) char buffer[sizeof(PoolObject)];

    PoolObject* obj = ::new (buffer) PoolObject(42, "test");
    CHECK(obj->id == 42);
    CHECK(obj->name == "test");
    CHECK(obj->alive);

    // 显式析构，不释放内存
    obj->~PoolObject();
    CHECK_FALSE(obj->alive);
}

TEST_CASE("对象池使用 placement new 管理对象生命周期") {
    ObjectPool pool;

    PoolObject* p1 = pool.construct(1, "first");
    PoolObject* p2 = pool.construct(2, "second");

    CHECK(p1->id == 1);
    CHECK(p1->name == "first");
    CHECK(p2->id == 2);
    CHECK(p2->name == "second");

    // 析构时自动调用 ~PoolObject()
}
