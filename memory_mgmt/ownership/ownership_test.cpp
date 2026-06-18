#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <memory>
#include <string>
#include <type_traits>
#include <utility>

class RuleOfFive {
public:
    static int ctor_count;
    static int copy_ctor_count;
    static int copy_assign_count;
    static int move_ctor_count;
    static int move_assign_count;
    static int dtor_count;

    explicit RuleOfFive(int n) : size_(n), data_(new int[n]) { ++ctor_count; }
    ~RuleOfFive() { ++dtor_count; delete[] data_; }

    RuleOfFive(const RuleOfFive& other)
        : size_(other.size_), data_(new int[other.size_]) {
        ++copy_ctor_count;
        std::copy(other.data_, other.data_ + size_, data_);
    }

    RuleOfFive& operator=(const RuleOfFive& other) {
        ++copy_assign_count;
        if (this != &other) {
            int* new_data = new int[other.size_];
            std::copy(other.data_, other.data_ + other.size_, new_data);
            delete[] data_;
            data_ = new_data;
            size_ = other.size_;
        }
        return *this;
    }

    RuleOfFive(RuleOfFive&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        ++move_ctor_count;
        other.size_ = 0;
        other.data_ = nullptr;
    }

    RuleOfFive& operator=(RuleOfFive&& other) noexcept {
        ++move_assign_count;
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    int size() const { return size_; }

private:
    int size_;
    int* data_;
};

int RuleOfFive::ctor_count = 0;
int RuleOfFive::copy_ctor_count = 0;
int RuleOfFive::copy_assign_count = 0;
int RuleOfFive::move_ctor_count = 0;
int RuleOfFive::move_assign_count = 0;
int RuleOfFive::dtor_count = 0;

class RuleOfZero {
public:
    explicit RuleOfZero(int n) : data_(std::make_unique<int[]>(n)), size_(n) {}
    int size() const { return size_; }
private:
    std::unique_ptr<int[]> data_;
    int size_;
};

class CopyOnly {
public:
    std::string tag;
    static int copy_count;
    static int move_count;

    explicit CopyOnly(std::string t) : tag(std::move(t)) {}
    CopyOnly(const CopyOnly& other) : tag(other.tag) { ++copy_count; }
    // 用户声明了拷贝构造，编译器不会自动生成移动构造
    // 因此 std::move 产生的 xvalue 会 fallback 到 const T& 拷贝构造
};
int CopyOnly::copy_count = 0;
int CopyOnly::move_count = 0;

TEST_CASE("拷贝构造用于初始化新对象，拷贝赋值用于已存在对象") {
    RuleOfFive::ctor_count = 0;
    RuleOfFive::copy_ctor_count = 0;
    RuleOfFive::copy_assign_count = 0;

    RuleOfFive a(10);
    RuleOfFive b = a;  // 拷贝构造
    CHECK(RuleOfFive::copy_ctor_count == 1);
    CHECK(RuleOfFive::copy_assign_count == 0);

    RuleOfFive c(5);
    c = a;  // 拷贝赋值
    CHECK(RuleOfFive::copy_ctor_count == 1);
    CHECK(RuleOfFive::copy_assign_count == 1);
}

TEST_CASE("移动构造转移资源所有权") {
    RuleOfFive::move_ctor_count = 0;
    RuleOfFive::dtor_count = 0;

    RuleOfFive a(10);
    RuleOfFive b = std::move(a);  // 移动构造
    CHECK(RuleOfFive::move_ctor_count == 1);
    CHECK(a.size() == 0);  // 被移后资源已转移
    CHECK(b.size() == 10);
}

TEST_CASE("零法则：使用智能指针则无需自定义特殊成员") {
    static_assert(std::is_move_constructible_v<RuleOfZero>);
    static_assert(std::is_move_assignable_v<RuleOfZero>);
    static_assert(!std::is_copy_constructible_v<RuleOfZero>);
    static_assert(!std::is_copy_assignable_v<RuleOfZero>);

    RuleOfZero rz1(20);
    RuleOfZero rz2 = std::move(rz1);
    CHECK(rz2.size() == 20);
}

TEST_CASE("用户声明拷贝构造并删除移动时，std::move 回退到拷贝") {
    CopyOnly::copy_count = 0;
    CopyOnly co("hello");
    CopyOnly co2 = std::move(co);  // 无可用移动构造，调用拷贝构造
    CHECK(CopyOnly::copy_count == 1);
    CHECK(co2.tag == "hello");
}
