// 内存序（memory order）行为演示（博客 ch06/06）
//
// 三个典型场景：
//   1. message passing：release/acquire 配对，标志位保护普通数据
//   2. relaxed 计数器：只要原子性，不要顺序
//   3. 无锁 ring buffer 下标：release/acquire 的实战形态
#include <array>
#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

// ---- 场景 1：message passing ----
std::atomic<bool> flag{false};
int data = 0;

void messagePassingDemo() {
    std::thread writer([] {
        data = 42;                                       // 先写数据
        flag.store(true, std::memory_order_release);     // 再发布标志
    });
    std::thread reader([] {
        while (!flag.load(std::memory_order_acquire)) {} // 先等标志
        assert(data == 42);                              // 保证看到新数据
    });
    writer.join();
    reader.join();
    std::cout << "message passing: data = " << data << '\n';
}

// ---- 场景 2：relaxed 计数器 ----
std::atomic<long> hits{0};

void relaxedCounterDemo() {
    constexpr int kThreads = 4, kPer = 100000;
    std::vector<std::thread> pool;
    for (int i = 0; i < kThreads; i++) {
        pool.emplace_back([] {
            for (int j = 0; j < kPer; j++)
                hits.fetch_add(1, std::memory_order_relaxed);
        });
    }
    for (auto& t : pool) t.join();
    std::cout << "relaxed counter: hits = " << hits << '\n';
}

// ---- 场景 3：SPSC ring buffer ----
constexpr int kCap = 1024;
std::array<int, kCap> ringBuf{};
std::atomic<int> head{0}, tail{0};

void ringBufferDemo() {
    constexpr int kTotal = 100000;
    std::thread producer([] {
        for (int i = 0; i < kTotal; i++) {
            int t = tail.load(std::memory_order_relaxed);
            while (t - head.load(std::memory_order_acquire) >= kCap) {}
            ringBuf[t % kCap] = i;                           // 先写数据
            tail.store(t + 1, std::memory_order_release);    // 再发布下标
        }
    });
    std::thread consumer([] {
        for (int expect = 0; expect < kTotal; expect++) {
            int h = head.load(std::memory_order_relaxed);
            while (tail.load(std::memory_order_acquire) <= h) {}
            assert(ringBuf[h % kCap] == expect);             // 读到的一定是新数据
            head.store(h + 1, std::memory_order_release);
        }
    });
    producer.join();
    consumer.join();
    std::cout << "ring buffer: " << kTotal << " items in order\n";
}

int main() {
    messagePassingDemo();
    relaxedCounterDemo();
    ringBufferDemo();
}
