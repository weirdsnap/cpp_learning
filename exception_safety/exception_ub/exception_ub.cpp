// 验证 ch08 异常安全与未定义行为 + ch09 编译期元编程与深层机制：
// ch08:
// 1. noexcept 契约
// 2. 析构函数抛出异常与栈展开
// 3. 未定义行为（UB）演示
// ch09:
// 4. 拷贝并交换惯用法（Copy-and-Swap）
// 5. placement new 与显式析构

#include <iostream>
#include <string>
#include <vector>
#include <new>      // placement new
#include <cstring>
#include <cassert>

// ============================================================
// ch08-1: noexcept 契约
// ============================================================

// noexcept 承诺不抛异常，违反则 terminate
class NoexceptDemo {
public:
    std::vector<int> data;

    // 移动构造标记 noexcept，vector 扩容时优先使用
    NoexceptDemo(NoexceptDemo&& other) noexcept
        : data(std::move(other.data)) {
        std::cout << "  NoexceptDemo move constructor (noexcept)\n";
    }

    NoexceptDemo() = default;
    NoexceptDemo(const NoexceptDemo&) = default;
};

class NonNoexceptDemo {
public:
    std::vector<int> data;

    // 移动构造未标记 noexcept，vector 扩容时回退到拷贝
    NonNoexceptDemo(NonNoexceptDemo&& other)
        : data(std::move(other.data)) {
        std::cout << "  NonNoexceptDemo move constructor (not noexcept)\n";
    }

    NonNoexceptDemo() = default;
    NonNoexceptDemo(const NonNoexceptDemo&) = default;
};

// noexcept 运算符：编译期检查表达式是否 noexcept
template <typename T>
void test_noexcept_move() {
    std::cout << "  std::is_nothrow_move_constructible_v<T>: "
              << std::is_nothrow_move_constructible_v<T> << "\n";
}

// ============================================================
// ch08-2: 析构函数抛出异常
// ============================================================

struct DtorThrower {
    ~DtorThrower() noexcept(false) {
        std::cout << "  DtorThrower destructor throwing...\n";
        throw std::runtime_error("dtor exception");
    }
};

struct SafeDtor {
    ~SafeDtor() {
        try {
            // 模拟可能抛出的操作
            std::cout << "  SafeDtor destructor, catching internally\n";
            // 如果内部操作可能抛，在析构内 try-catch 吞掉
        } catch (...) {
            // 绝不从析构函数传播异常
        }
    }
};

// ============================================================
// ch08-3: 未定义行为演示（仅演示，不放入测试）
// ============================================================

// 有符号整数溢出（UB）
int signed_overflow() {
    int x = 2147483647;  // INT_MAX
    return x + 1;        // UB: 有符号整数溢出
}

// 未初始化变量（UB）
int uninitialized_use() {
    int x;               // 未初始化
    return x;            // UB: 读取未初始化变量
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
        std::cout << "  StringBuffer ctor: \"" << str << "\"\n";
    }

    // 拷贝构造
    StringBuffer(const StringBuffer& other)
        : size_(other.size_) {
        data_ = new char[size_ + 1];
        std::strcpy(data_, other.data_);
        std::cout << "  StringBuffer copy ctor\n";
    }

    // 移动构造
    StringBuffer(StringBuffer&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "  StringBuffer move ctor\n";
    }

    // 析构
    ~StringBuffer() {
        delete[] data_;
    }

    // Copy-and-Swap 赋值运算符
    // 按值传递 = 拷贝构造，然后交换，临时对象析构释放旧资源
    StringBuffer& operator=(StringBuffer other) {
        swap(other);
        return *this;
    }

    void swap(StringBuffer& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    const char* c_str() const { return data_ ? data_ : ""; }

private:
    char* data_;
    size_t size_;
};

// ============================================================
// ch09-2: placement new 与显式析构
// ============================================================

struct alignas(64) PoolObject {
    int id;
    std::string name;

    PoolObject(int i, const std::string& n) : id(i), name(n) {
        std::cout << "  PoolObject(" << id << ") constructed\n";
    }

    ~PoolObject() {
        std::cout << "  PoolObject(" << id << ") destroyed\n";
    }
};

// 简单的内存池
class ObjectPool {
public:
    ObjectPool() : pool_(new char[sizeof(PoolObject) * 8]), count_(0) {}

    ~ObjectPool() {
        // 销毁所有已构造的对象
        for (size_t i = 0; i < count_; ++i) {
            reinterpret_cast<PoolObject*>(pool_ + i * sizeof(PoolObject))->~PoolObject();
        }
        delete[] pool_;
    }

    template <typename... Args>
    PoolObject* construct(Args&&... args) {
        assert(count_ < 8);
        void* ptr = pool_ + count_ * sizeof(PoolObject);
        PoolObject* obj = ::new (ptr) PoolObject(std::forward<Args>(args)...);
        ++count_;
        return obj;
    }

private:
    char* pool_;
    size_t count_;
};

// ============================================================
// 演示函数
// ============================================================

void demo_noexcept() {
    std::cout << "=== ch08-1: noexcept 契约 ===\n";

    // noexcept 对 vector 扩容的影响
    std::vector<NoexceptDemo> vec1;
    vec1.reserve(1);
    std::cout << "  Pushing NoexceptDemo (noexcept move):\n";
    vec1.push_back(NoexceptDemo{});

    std::vector<NonNoexceptDemo> vec2;
    vec2.reserve(1);
    std::cout << "  Pushing NonNoexceptDemo (not noexcept):\n";
    vec2.push_back(NonNoexceptDemo{});

    // noexcept 运算符
    std::cout << "  noexcept check:\n";
    test_noexcept_move<NoexceptDemo>();
    test_noexcept_move<NonNoexceptDemo>();
}

void demo_dtor_exception() {
    std::cout << "\n=== ch08-2: 析构函数抛出异常 ===\n";

    // 安全的析构：内部捕获
    {
        SafeDtor sd;
        std::cout << "  SafeDtor scope ending...\n";
    }

    // 注意：DtorThrower 的析构会抛异常，这里用 try-catch 演示
    // 但实际工程中绝不应该让析构抛异常
    std::cout << "  DtorThrower (noexcept(false)) will throw:\n";
    try {
        DtorThrower dt;
        // 作用域结束，dt 析构，抛出异常
    } catch (const std::exception& e) {
        std::cout << "  Caught: " << e.what() << "\n";
    }
}

void demo_copy_and_swap() {
    std::cout << "\n=== ch09-1: 拷贝并交换惯用法 ===\n";

    StringBuffer a("Hello");
    StringBuffer b("World");

    std::cout << "  Before swap: a=\"" << a.c_str() << "\", b=\"" << b.c_str() << "\"\n";
    a = b;  // Copy-and-Swap 赋值
    std::cout << "  After a = b: a=\"" << a.c_str() << "\", b=\"" << b.c_str() << "\"\n";

    // 自赋值安全
    a = a;
    std::cout << "  After a = a (self-assignment): a=\"" << a.c_str() << "\"\n";
}

void demo_placement_new() {
    std::cout << "\n=== ch09-2: placement new 与显式析构 ===\n";

    ObjectPool pool;
    PoolObject* p1 = pool.construct(1, "first");
    PoolObject* p2 = pool.construct(2, "second");
    PoolObject* p3 = pool.construct(3, "third");

    std::cout << "  Pool objects: " << p1->name << ", " << p2->name << ", " << p3->name << "\n";
    std::cout << "  Pool going out of scope, destructors called...\n";
}

int main() {
    demo_noexcept();
    demo_dtor_exception();
    demo_copy_and_swap();
    demo_placement_new();

    return 0;
}
