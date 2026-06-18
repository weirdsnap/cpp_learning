#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <utility>

class Tracer {
public:
    static int copies;
    static int moves;

    Tracer() = default;
    explicit Tracer(std::string s) : data_(std::move(s)) {}

    Tracer(const Tracer& other) : data_(other.data_) { ++copies; }
    Tracer(Tracer&& other) noexcept : data_(std::move(other.data_)) { ++moves; }

    Tracer& operator=(const Tracer&) = default;
    Tracer& operator=(Tracer&&) = default;

private:
    std::string data_;
};

int Tracer::copies = 0;
int Tracer::moves = 0;

Tracer make_prvalue() {
    return Tracer("prvalue");
}

TEST_CASE("C++17 返回 prvalue 强制保证零拷贝/零移动") {
    Tracer::copies = 0;
    Tracer::moves = 0;
    Tracer t = make_prvalue();
    CHECK(Tracer::copies == 0);
    CHECK(Tracer::moves == 0);
}

TEST_CASE("std::move 显式触发移动构造") {
    Tracer::copies = 0;
    Tracer::moves = 0;
    Tracer source("x");
    Tracer target = std::move(source);
    CHECK(Tracer::copies == 0);
    CHECK(Tracer::moves == 1);
}

TEST_CASE("左值触发拷贝构造") {
    Tracer::copies = 0;
    Tracer::moves = 0;
    Tracer source("x");
    Tracer target = source;  // 左值，拷贝
    CHECK(Tracer::copies == 1);
    CHECK(Tracer::moves == 0);
}

TEST_CASE("移动语义避免容器重新分配时的深拷贝") {
    Tracer::copies = 0;
    Tracer::moves = 0;

    std::vector<Tracer> v;
    v.reserve(3);
    v.push_back(Tracer("a"));  // 临时对象被移动入容器
    v.push_back(Tracer("b"));

    // 临时对象通过移动进入 vector，不应产生拷贝
    CHECK(Tracer::copies == 0);
    CHECK(Tracer::moves >= 2);
}
