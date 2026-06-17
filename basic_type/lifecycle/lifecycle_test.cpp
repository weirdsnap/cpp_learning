#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <sstream>
#include <string>
#include <type_traits>

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
