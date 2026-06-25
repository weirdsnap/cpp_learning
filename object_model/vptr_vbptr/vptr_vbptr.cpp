// 验证 ch02/07 vptr 与 vbptr 的区别 + ch02/08 同时含 vptr 与 vbptr
//
// 1. 无虚函数无虚继承：最小对象
// 2. 有虚函数：含 vptr
// 3. 有虚继承：含 vbptr
// 4. 同时有虚函数和虚继承：含 vptr + vbptr

#include <iostream>

// ============================================================
// 1. 无虚函数无虚继承
// ============================================================

struct Plain {
    int a;
    int b;
};

// ============================================================
// 2. 有虚函数：含 vptr
// ============================================================

struct WithVirtual {
    int a;
    int b;
    virtual void foo() {}
    virtual ~WithVirtual() = default;
};

// ============================================================
// 3. 有虚继承：含 vbptr
// ============================================================

struct Base {
    int x;
};

struct Derived : virtual Base {
    int y;
};

// ============================================================
// 4. 同时有虚函数和虚继承
// ============================================================

struct VirtualBase {
    int a;
    virtual void foo() {}
    virtual ~VirtualBase() = default;
};

struct VirtualDerived : virtual VirtualBase {
    int b;
    void foo() override {}
};

// ============================================================
// 5. 多层虚继承
// ============================================================

struct GrandBase {
    int g;
};

struct Mid1 : virtual GrandBase {
    int m1;
};

struct Mid2 : virtual GrandBase {
    int m2;
};

struct Top : Mid1, Mid2 {
    int t;
};

void demo_vptr_vbptr() {
    std::cout << "=== vptr 与 vbptr 内存布局 ===\n\n";

    std::cout << "1. 无虚函数无虚继承:\n";
    std::cout << "   sizeof(Plain) = " << sizeof(Plain) << " (2 ints)\n";

    std::cout << "\n2. 有虚函数（含 vptr）:\n";
    std::cout << "   sizeof(WithVirtual) = " << sizeof(WithVirtual)
              << " (2 ints + vptr)\n";

    std::cout << "\n3. 有虚继承（含 vbptr）:\n";
    std::cout << "   sizeof(Base)    = " << sizeof(Base) << " (1 int)\n";
    std::cout << "   sizeof(Derived) = " << sizeof(Derived)
              << " (vbptr + y + Base::x)\n";

    std::cout << "\n4. 同时有虚函数和虚继承:\n";
    std::cout << "   sizeof(VirtualBase)    = " << sizeof(VirtualBase)
              << " (1 int + vptr)\n";
    std::cout << "   sizeof(VirtualDerived) = " << sizeof(VirtualDerived)
              << " (vbptr + b + VirtualBase)\n";

    std::cout << "\n5. 菱形虚继承:\n";
    std::cout << "   sizeof(GrandBase) = " << sizeof(GrandBase) << "\n";
    std::cout << "   sizeof(Mid1)      = " << sizeof(Mid1) << "\n";
    std::cout << "   sizeof(Mid2)      = " << sizeof(Mid2) << "\n";
    std::cout << "   sizeof(Top)       = " << sizeof(Top) << "\n";
}

int main() {
    demo_vptr_vbptr();

    return 0;
}
