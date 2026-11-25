// 对象池 vs new/delete 基准对比 — 博客 cpp/practices/object_pool_01.md
#include <chrono>
#include <cstdio>
#include <vector>

#include "object_pool.hpp"

struct Node {
    long a, b, c, d;  // 32 字节，典型小对象
    Node(long v) : a(v), b(v), c(v), d(v) {}
};

template <typename F>
double time_ms(F&& f, int rounds) {
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < rounds; ++i) f();
    auto t1 = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count() / rounds;
}

int main() {
    constexpr int N = 100000;
    constexpr int ROUNDS = 20;

    // 场景：反复创建 N 个对象再全部销毁（模拟每帧重建粒子系统）
    double t_new = time_ms([] {
        std::vector<Node*> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) v.push_back(new Node(i));
        for (auto* p : v) delete p;
    }, ROUNDS);

    double t_pool = time_ms([] {
        object_pool::ObjectPool<Node, 1024> pool;
        std::vector<Node*> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) v.push_back(pool.create(i));
        for (auto* p : v) pool.destroy(p);
    }, ROUNDS);

    std::printf("new/delete : %8.2f ms\n", t_new);
    std::printf("object pool: %8.2f ms\n", t_pool);
    std::printf("speedup    : %8.1fx\n", t_new / t_pool);
    return 0;
}
