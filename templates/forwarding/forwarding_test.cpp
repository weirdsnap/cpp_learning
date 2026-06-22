#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <utility>

struct Tracer {
    static int copies;
    static int moves;
    Tracer() = default;
    Tracer(const Tracer&) { ++copies; }
    Tracer(Tracer&&) noexcept { ++moves; }
    Tracer& operator=(const Tracer&) = default;
    Tracer& operator=(Tracer&&) = default;
};
int Tracer::copies = 0;
int Tracer::moves = 0;

void consume(Tracer) {}

template<typename T>
void wrapper_bad(T&& x) {
    consume(x);  // 未转发，x 始终是左值
}

template<typename T>
void wrapper_good(T&& x) {
    consume(std::forward<T>(x));
}

TEST_CASE("未转发时万能引用参数退化为左值") {
    Tracer::copies = 0;
    Tracer::moves = 0;
    wrapper_bad(Tracer());  // 临时对象传入，但内部按左值传给 consume
    CHECK(Tracer::copies == 1);
    CHECK(Tracer::moves == 0);
}

TEST_CASE("完美转发保持右值属性") {
    Tracer::copies = 0;
    Tracer::moves = 0;
    wrapper_good(Tracer());  // 临时对象传入，转发后仍是右值
    CHECK(Tracer::copies == 0);
    CHECK(Tracer::moves == 1);
}

TEST_CASE("完美转发保持左值属性") {
    Tracer::copies = 0;
    Tracer::moves = 0;
    Tracer t;
    wrapper_good(t);  // 左值传入，转发后仍是左值
    CHECK(Tracer::copies == 1);
    CHECK(Tracer::moves == 0);
}
