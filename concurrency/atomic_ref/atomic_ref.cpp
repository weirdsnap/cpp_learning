// 验证 ch10/02 std::atomic_ref / atomic_wait
//
// 1. atomic_ref 对非 atomic 变量执行原子操作
// 2. atomic_ref 生命周期必须短于被引用对象
// 3. atomic_wait / atomic_notify 无锁等待

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

// ============================================================
// 1. atomic_ref 基本用法
// ============================================================

void demo_atomic_ref_basic() {
    std::cout << "=== 1. atomic_ref 基本用法 ===\n";

    int shared = 0;
    constexpr int N = 100000;
    constexpr int THREADS = 4;

    // 不使用 atomic_ref：数据竞争
    {
        std::vector<std::thread> threads;
        int counter = 0;
        for (int t = 0; t < THREADS; ++t) {
            threads.emplace_back([&counter] {
                for (int i = 0; i < N; ++i) {
                    ++counter;  // 数据竞争！
                }
            });
        }
        for (auto& t : threads) t.join();
        std::cout << "  无保护: counter = " << counter
                  << " (期望 " << (N * THREADS) << ")\n";
    }

    // 使用 atomic_ref：原子操作
    {
        std::vector<std::thread> threads;
        int counter = 0;
        for (int t = 0; t < THREADS; ++t) {
            threads.emplace_back([&counter] {
                std::atomic_ref<int> atomic_counter(counter);
                for (int i = 0; i < N; ++i) {
                    atomic_counter.fetch_add(1);
                }
            });
        }
        for (auto& t : threads) t.join();
        std::cout << "  atomic_ref: counter = " << counter
                  << " (期望 " << (N * THREADS) << ")\n";
    }
}

// ============================================================
// 2. atomic_ref 生命周期管理
// ============================================================

void demo_atomic_ref_lifetime() {
    std::cout << "\n=== 2. atomic_ref 生命周期 ===\n";

    int value = 42;
    {
        std::atomic_ref<int> ref(value);
        ref.store(100);
        std::cout << "  atomic_ref 存活时: value = " << value << "\n";
    }  // ref 销毁
    std::cout << "  atomic_ref 销毁后: value = " << value << "\n";
    // 注意：atomic_ref 销毁后，value 恢复普通变量
}

// ============================================================
// 3. atomic_wait / atomic_notify 无锁等待
// ============================================================

void demo_atomic_wait() {
    std::cout << "\n=== 3. atomic_wait / atomic_notify ===\n";

    std::atomic<int> flag{0};
    int data = 0;

    std::thread producer([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        data = 42;           // 写数据
        flag.store(1);       // 发布标志
        flag.notify_one();   // 唤醒等待者
        std::cout << "  生产者: data=42, flag=1, 已通知\n";
    });

    std::thread consumer([&] {
        // 等待 flag 变为非 0
        flag.wait(0);
        // 此时 data 已就绪
        std::cout << "  消费者: flag=" << flag.load()
                  << ", data=" << data << "\n";
    });

    producer.join();
    consumer.join();
}

int main() {
    demo_atomic_ref_basic();
    demo_atomic_ref_lifetime();
    demo_atomic_wait();

    return 0;
}
