#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <type_traits>

// ============================================================
// 1. 条件语境中的隐式 bool 转换
// ============================================================

TEST_CASE("整数在条件中隐式转 bool") {
    int x = 42;
    int y = 0;

    CHECK(static_cast<bool>(x) == true);
    CHECK(static_cast<bool>(y) == false);
}

TEST_CASE("指针在条件中隐式转 bool") {
    int val = 42;
    int* p = &val;
    int* q = nullptr;

    CHECK(static_cast<bool>(p) == true);
    CHECK(static_cast<bool>(q) == false);
}

TEST_CASE("浮点数在条件中隐式转 bool") {
    double d1 = 3.14;
    double d2 = 0.0;

    CHECK(static_cast<bool>(d1) == true);
    CHECK(static_cast<bool>(d2) == false);
}

// ============================================================
// 2. explicit operator bool
// ============================================================

class FileHandle {
    void* handle_;
public:
    explicit FileHandle(void* h) : handle_(h) {}
    explicit operator bool() const { return handle_ != nullptr; }
};

TEST_CASE("explicit operator bool 在条件语境中可用") {
    FileHandle fh(new int(42));
    FileHandle fh_null(nullptr);

    CHECK(static_cast<bool>(fh) == true);
    CHECK(static_cast<bool>(fh_null) == false);
}

TEST_CASE("explicit operator bool 阻止非条件语境隐式转换") {
    FileHandle fh(new int(42));

    // 编译期验证：explicit operator bool 不会隐式转 int
    CHECK_FALSE((std::is_convertible_v<FileHandle, int>));
    CHECK_FALSE((std::is_convertible_v<FileHandle, bool>));
    // 但可以显式转换
    CHECK((std::is_constructible_v<bool, FileHandle>));
}

// ============================================================
// 3. 非 explicit operator bool 的陷阱
// ============================================================

class DangerousBool {
    int val_;
public:
    DangerousBool(int v) : val_(v) {}
    operator bool() const { return val_ != 0; }
};

TEST_CASE("非 explicit operator bool 可隐式转 int") {
    DangerousBool db1(42);
    DangerousBool db2(0);

    CHECK((std::is_convertible_v<DangerousBool, int>));
    CHECK((std::is_convertible_v<DangerousBool, bool>));

    // 隐式转换导致荒谬的算术运算
    int n = db1;
    CHECK(n == 1);  // true → 1

    int sum = db1 + 2;
    CHECK(sum == 3);  // true(1) + 2 = 3
}

TEST_CASE("非 explicit operator bool 导致不同对象相等") {
    DangerousBool a(1), b(2);
    // 都转成 true(1)，所以相等
    CHECK(a == b);  // 荒谬但合法
}

// ============================================================
// 4. Safe Bool Idiom vs explicit operator bool
// ============================================================

class PreCpp11Safe {
    int val_;
public:
    PreCpp11Safe(int v) : val_(v) {}
    operator void*() const { return val_ ? const_cast<PreCpp11Safe*>(this) : nullptr; }
};

class Cpp11Modern {
    int val_;
public:
    Cpp11Modern(int v) : val_(v) {}
    explicit operator bool() const { return val_ != 0; }
};

TEST_CASE("Safe Bool Idiom 在条件中可用") {
    PreCpp11Safe old1(42);
    PreCpp11Safe old2(0);

    CHECK(static_cast<bool>(old1) == true);
    CHECK(static_cast<bool>(old2) == false);
}

TEST_CASE("explicit operator bool 阻止隐式算术转换") {
    Cpp11Modern modern1(42);
    Cpp11Modern modern2(0);

    CHECK(static_cast<bool>(modern1) == true);
    CHECK(static_cast<bool>(modern2) == false);

    // 编译期验证
    CHECK_FALSE((std::is_convertible_v<Cpp11Modern, int>));
    CHECK_FALSE((std::is_convertible_v<Cpp11Modern, bool>));
}
