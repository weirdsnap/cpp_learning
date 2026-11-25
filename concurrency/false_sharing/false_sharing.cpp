// 伪共享（false sharing）性能对比 demo
//
// 两个线程分别自增两个计数器，逻辑上没有任何共享数据：
//   版本一（Packed）：两个计数器相邻，落在同一个缓存行；
//   版本二（Padded）：按硬件缓存行宽度对齐，隔到不同缓存行。
//
// 对应博客文章：cpp/practices/perf_01.md（用 perf c2c 定位伪共享）
// 原理讲解：    cpp/ch06/12.md（缓存行与伪共享）
#include <chrono>
#include <cstdio>
#include <new>
#include <thread>

namespace {

constexpr long long kIterations = 100'000'000;

// 版本一：两个计数器相邻，同处一个缓存行
struct PackedCounters {
    long long a = 0;
    long long b = 0;
};

// 版本二：用硬件干涉尺寸把 b 推到下一个缓存行。
// std::hardware_destructive_interference_size（C++17，<new>）是平台认为
// 可能发生伪共享的最小距离：x86 上通常是 64，Apple Silicon 上通常是 128，
// 比写死 64 更稳。
struct PaddedCounters {
    alignas(std::hardware_destructive_interference_size) long long a = 0;
    alignas(std::hardware_destructive_interference_size) long long b = 0;
};

template <typename Counters>
double run(long long iterations, Counters& out) {
    auto t0 = std::chrono::steady_clock::now();
    std::thread t1([&] {
        for (long long i = 0; i < iterations; ++i) ++out.a;
    });
    std::thread t2([&] {
        for (long long i = 0; i < iterations; ++i) ++out.b;
    });
    t1.join();
    t2.join();
    auto t1end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(t1end - t0).count();
}

}  // namespace

int main() {
    PackedCounters packed;
    PaddedCounters padded;
    std::printf("硬件干涉尺寸 = %zu, sizeof(Packed) = %zu, sizeof(Padded) = %zu\n",
                std::hardware_destructive_interference_size,
                sizeof(packed), sizeof(padded));

    double t_packed = run<PackedCounters>(kIterations, packed);
    double t_padded = run<PaddedCounters>(kIterations, padded);

    std::printf("packed（同一缓存行）: %.3f s\n", t_packed);
    std::printf("padded（各占一行）  : %.3f s\n", t_padded);
    std::printf("加速比: %.2fx\n", t_packed / t_padded);
    return 0;
}
