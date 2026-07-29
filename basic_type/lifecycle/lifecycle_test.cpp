#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

static std::ostringstream g_log;

struct Tracer {
    const char* name;
    Tracer(const char* n) : name(n) {
        g_log << "C" << name;
    }
    ~Tracer() {
        g_log << "D" << name;
    }
};

class Demo {
public:
    Tracer m1;
    Tracer m2;
    Tracer m3;

    Demo() : m3("3"), m1("1"), m2("2") {}
};

TEST_CASE("成员按声明顺序构造，按声明逆序析构") {
    g_log.str("");
    {
        Demo d;
    }
    // 构造顺序：m1 -> m2 -> m3；析构顺序：m3 -> m2 -> m1
    CHECK(g_log.str() == "C1C2C3D3D2D1");
}

TEST_CASE("constexpr 可用于编译期常量") {
    constexpr int compile_const = 42;
    int arr[compile_const];  // 数组大小需要编译期常量
    (void)arr;
    CHECK(compile_const == 42);
}

TEST_CASE("constexpr 函数编译期和运行期都能调用") {
    constexpr auto square = [](int x) constexpr { return x * x; };
    constexpr int r1 = square(5);
    int n = 6;
    int r2 = square(n);
    CHECK(r1 == 25);
    CHECK(r2 == 36);
}

TEST_CASE("const 变量不一定是编译期常量") {
    const int runtime_const = 42;  // 运行期初始化
    CHECK((std::is_const_v<decltype(runtime_const)>));
    CHECK(runtime_const == 42);
}

// 静态变量默认零初始化测试
static int g_static_int;
static double g_static_double;
static int* g_static_ptr;
static bool g_static_bool;
static int g_static_arr[3];

void local_static_init() {
    static int s;
    CHECK(s == 0);
}

TEST_CASE("静态存储期变量默认零初始化") {
    CHECK(g_static_int == 0);
    CHECK(g_static_double == 0.0);
    CHECK(g_static_ptr == nullptr);
    CHECK(g_static_bool == false);
    CHECK(g_static_arr[0] == 0);
    CHECK(g_static_arr[1] == 0);
    CHECK(g_static_arr[2] == 0);

    local_static_init();
}

// 博客 cpp/ch07/33：vector(n) 的默认插入语义（C++11）
struct CtorProbe {
    static inline int default_calls = 0;
    static inline int copy_calls = 0;
    static void reset() { default_calls = 0; copy_calls = 0; }
    CtorProbe() { default_calls++; }
    CtorProbe(const CtorProbe&) { copy_calls++; }
};

TEST_CASE("vector(n) 默认插入：n 次默认构造，不经过拷贝构造") {
    CtorProbe::reset();
    {
        std::vector<CtorProbe> v(5);
        CHECK(v.size() == 5);
    }
    CHECK(CtorProbe::default_calls == 5);
    CHECK(CtorProbe::copy_calls == 0);
}

TEST_CASE("vector(n, value) 是拷贝语义：1 次构造 + n 次拷贝") {
    CtorProbe::reset();
    {
        std::vector<CtorProbe> v(5, CtorProbe{});
        CHECK(v.size() == 5);
    }
    CHECK(CtorProbe::default_calls == 1);
    CHECK(CtorProbe::copy_calls == 5);
}

TEST_CASE("reserve 只分配内存，不构造元素；resize 扩容部分默认插入") {
    CtorProbe::reset();
    std::vector<CtorProbe> v;
    v.reserve(5);
    CHECK(CtorProbe::default_calls == 0);
    v.resize(3);
    CHECK(CtorProbe::default_calls == 3);
    CHECK(CtorProbe::copy_calls == 0);
}
