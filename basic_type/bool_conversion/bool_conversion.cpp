// 验证 ch01/05 隐式转换 bool / operator bool
//
// 1. 条件语境中的隐式 bool 转换（if/while/for/&&/||/!）
// 2. explicit operator bool 阻止非条件语境中的隐式转换
// 3. 非 explicit operator bool 导致 int 提升的陷阱
// 4. Safe Bool Idiom（C++11 前）与 explicit operator bool（C++11 起）

#include <iostream>
#include <type_traits>

// ============================================================
// 1. 条件语境中的隐式 bool 转换
// ============================================================

void demo_implicit_bool_in_conditions() {
    std::cout << "=== 1. 条件语境中的隐式 bool 转换 ===\n";

    int x = 42;
    int y = 0;

    // 指针、整数、浮点数在条件中隐式转 bool
    if (x)   std::cout << "  if (x): true (非零)\n";
    if (!y)  std::cout << "  if (!y): true (零为 false)\n";

    int* p = &x;
    int* q = nullptr;
    if (p)   std::cout << "  if (p): true (非空指针)\n";
    if (!q)  std::cout << "  if (!q): true (空指针为 false)\n";

    double d = 0.0;
    if (!d)  std::cout << "  if (!d): true (0.0 为 false)\n";

    // while/for 同理
    int count = 3;
    while (count--) {}  // count 隐式转 bool
    std::cout << "  while (count--): count 最终为 " << count << "\n";
}

// ============================================================
// 2. explicit operator bool 的正确用法
// ============================================================

class FileHandle {
    void* handle_;
public:
    explicit FileHandle(void* h) : handle_(h) {}
    explicit operator bool() const { return handle_ != nullptr; }
};

class StreamState {
    bool good_;
public:
    explicit StreamState(bool g) : good_(g) {}
    explicit operator bool() const { return good_; }
};

void demo_explicit_bool() {
    std::cout << "\n=== 2. explicit operator bool 的正确用法 ===\n";

    FileHandle fh(new int(42));
    FileHandle fh_null(nullptr);

    // 条件语境中允许 explicit operator bool
    if (fh)       std::cout << "  if (fh): true (非空句柄)\n";
    if (!fh_null) std::cout << "  if (!fh_null): true (空句柄)\n";

    // 非条件语境中阻止隐式转换
    // int n = fh;           // ❌ 编译错误
    // bool b = fh;          // ❌ 编译错误（C++11 起）
    // if (fh == true)       // ❌ 编译错误
    // int sum = fh + 1;     // ❌ 编译错误

    // 但可以显式转换
    bool b = static_cast<bool>(fh);
    std::cout << "  static_cast<bool>(fh) = " << b << "\n";

    // 模拟流状态检测
    StreamState ss_good(true);
    StreamState ss_bad(false);

    if (ss_good) std::cout << "  if (ss_good): 流状态良好\n";
    if (!ss_bad) std::cout << "  if (!ss_bad): 流状态错误\n";
}

// ============================================================
// 3. 非 explicit operator bool 的陷阱
// ============================================================

class DangerousBool {
    int val_;
public:
    DangerousBool(int v) : val_(v) {}
    operator bool() const { return val_ != 0; }  // 非 explicit！
};

void demo_non_explicit_trap() {
    std::cout << "\n=== 3. 非 explicit operator bool 的陷阱 ===\n";

    DangerousBool db1(42);
    DangerousBool db2(0);

    // 条件语境正常
    if (db1) std::cout << "  if (db1): true\n";

    // 陷阱：bool 提升为 int
    int n = db1;           // ✅ 合法！db1 隐式转 bool 再转 int
    std::cout << "  int n = db1; n = " << n << "\n";

    // 更糟：算术运算
    int sum = db1 + 2;     // ✅ 合法！db1 → true → 1, 1 + 2 = 3
    std::cout << "  db1 + 2 = " << sum << " (荒谬！)\n";

    // 比较
    if (db1 == true)       // ✅ 合法！但语义奇怪
        std::cout << "  db1 == true: 成立\n";

    // 更荒谬的
    DangerousBool a(1), b(2);
    if (a == b)            // ✅ 合法！a → true(1), b → true(1), 1==1
        std::cout << "  a == b: true (两个不同的非零值相等！荒谬)\n";
}

// ============================================================
// 4. Safe Bool Idiom（C++11 前）vs explicit operator bool
// ============================================================

// C++11 前：Safe Bool Idiom 使用 operator void* 避免转 int
class PreCpp11Safe {
    int val_;
public:
    PreCpp11Safe(int v) : val_(v) {}
    operator void*() const { return val_ ? const_cast<PreCpp11Safe*>(this) : nullptr; }
};

// C++11 起：explicit operator bool 彻底解决
class Cpp11Modern {
    int val_;
public:
    Cpp11Modern(int v) : val_(v) {}
    explicit operator bool() const { return val_ != 0; }
};

void demo_safe_bool_idiom() {
    std::cout << "\n=== 4. Safe Bool Idiom vs explicit operator bool ===\n";

    PreCpp11Safe old1(42);
    PreCpp11Safe old2(0);

    // Safe Bool Idiom 在条件中可用
    if (old1) std::cout << "  Safe Bool: if (old1): true\n";
    if (!old2) std::cout << "  Safe Bool: if (!old2): true\n";

    // 但 operator void* 也有自己的问题
    // delete old1;  // ❌ 编译错误（void* 不能直接 delete）
    // 但可以比较指针
    if (old1 == old2)  // 比较的是 void* 地址，不是语义值
        std::cout << "  Safe Bool: old1 == old2 (比较指针地址)\n";

    // C++11 方式
    Cpp11Modern modern1(42);
    Cpp11Modern modern2(0);

    if (modern1) std::cout << "  explicit bool: if (modern1): true\n";
    if (!modern2) std::cout << "  explicit bool: if (!modern2): true\n";

    // 以下全部编译错误
    // int n = modern1;       // ❌
    // modern1 + 2;           // ❌
    // modern1 == modern2;    // ❌
    // modern1 == true;       // ❌
}

int main() {
    demo_implicit_bool_in_conditions();
    demo_explicit_bool();
    demo_non_explicit_trap();
    demo_safe_bool_idiom();

    return 0;
}
