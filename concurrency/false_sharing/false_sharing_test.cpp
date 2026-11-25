// 伪共享 demo 的 doctest 断言
//
// 只断言可移植的行为：
//   - 按硬件干涉尺寸对齐后，两个计数器的地址距离 >= 该尺寸（不落同一缓存行）
//   - PaddedCounters 的大小是对齐值的倍数（对齐要求的直接推论）
//   - 两个版本并发计数的结果都正确
// 不断言时间差：加速比取决于核数、负载、微架构，不适合做断言。
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdint>
#include <new>
#include <thread>

namespace {

constexpr std::size_t kLine = std::hardware_destructive_interference_size;

struct PackedCounters {
    long long a = 0;
    long long b = 0;
};

struct PaddedCounters {
    alignas(kLine) long long a = 0;
    alignas(kLine) long long b = 0;
};

template <typename Counters>
void count(long long iterations, Counters& out) {
    std::thread t1([&] {
        for (long long i = 0; i < iterations; ++i) ++out.a;
    });
    std::thread t2([&] {
        for (long long i = 0; i < iterations; ++i) ++out.b;
    });
    t1.join();
    t2.join();
}

}  // namespace

TEST_CASE("padded 计数器落在不同缓存行") {
    PaddedCounters c{};
    auto dist = reinterpret_cast<std::uintptr_t>(&c.b) -
                reinterpret_cast<std::uintptr_t>(&c.a);
    CHECK(dist >= kLine);
    CHECK(sizeof(PaddedCounters) % kLine == 0);
    CHECK(alignof(PaddedCounters) == kLine);
}

TEST_CASE("两个版本计数结果都正确") {
    constexpr long long kN = 100'000;
    PackedCounters packed;
    PaddedCounters padded;
    count(kN, packed);
    count(kN, padded);
    CHECK(packed.a == kN);
    CHECK(packed.b == kN);
    CHECK(padded.a == kN);
    CHECK(padded.b == kN);
}
