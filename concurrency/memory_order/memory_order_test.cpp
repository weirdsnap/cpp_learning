// 内存序行为的 doctest 断言
//
// 只断言正确内存序下必须成立的行为；
// "缺了内存序会出错"这类反向场景无法稳定复现，不写进测试。
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <array>
#include <atomic>
#include <thread>
#include <vector>

TEST_CASE("release/acquire 配对：读到标志就一定能读到数据") {
    for (int round = 0; round < 1000; round++) {
        std::atomic<bool> flag{false};
        int data = 0;
        std::thread writer([&] {
            data = 42;
            flag.store(true, std::memory_order_release);
        });
        std::thread reader([&] {
            while (!flag.load(std::memory_order_acquire)) {}
            CHECK(data == 42);
        });
        writer.join();
        reader.join();
    }
}

TEST_CASE("relaxed 计数器：原子性不受内存序影响") {
    std::atomic<long> hits{0};
    constexpr int kThreads = 4, kPer = 10000;
    {
        std::vector<std::thread> pool;
        for (int i = 0; i < kThreads; i++) {
            pool.emplace_back([&] {
                for (int j = 0; j < kPer; j++)
                    hits.fetch_add(1, std::memory_order_relaxed);
            });
        }
        for (auto& t : pool) t.join();
    }
    CHECK(hits == kThreads * kPer);
}

TEST_CASE("SPSC 环形缓冲区：release/acquire 下标保证数据有序") {
    constexpr int kCap = 64, kTotal = 10000;
    std::array<int, kCap> buf{};
    std::atomic<int> head{0}, tail{0};

    std::thread producer([&] {
        for (int i = 0; i < kTotal; i++) {
            int t = tail.load(std::memory_order_relaxed);
            while (t - head.load(std::memory_order_acquire) >= kCap) {}
            buf[t % kCap] = i;
            tail.store(t + 1, std::memory_order_release);
        }
    });
    std::thread consumer([&] {
        for (int expect = 0; expect < kTotal; expect++) {
            int h = head.load(std::memory_order_relaxed);
            while (tail.load(std::memory_order_acquire) <= h) {}
            CHECK(buf[h % kCap] == expect);
            head.store(h + 1, std::memory_order_release);
        }
    });
    producer.join();
    consumer.join();
}
