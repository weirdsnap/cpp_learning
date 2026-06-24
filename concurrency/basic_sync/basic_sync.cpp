// 验证 ch06 并发与内存模型相关主题：
// 1. mutex — 裸锁
// 2. lock_guard — RAII 锁包装器
// 3. unique_lock — 可移动、可延迟加锁的 RAII 封装
// 4. 三者选择策略
// 5. std::atomic — 原子操作
// 6. std::atomic 内存序
// 7. 数据竞争

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <chrono>
#include <cassert>

// ============================================================
// 1. mutex — 裸锁
// ============================================================

int shared_counter = 0;
std::mutex g_mutex;

void unsafe_increment(int n) {
    for (int i = 0; i < n; ++i) {
        ++shared_counter;  // 数据竞争！
    }
}

void safe_increment_raw_mutex(int n) {
    for (int i = 0; i < n; ++i) {
        g_mutex.lock();
        ++shared_counter;
        g_mutex.unlock();
    }
}

// ============================================================
// 2. lock_guard — RAII 锁包装器
// ============================================================

int raii_counter = 0;
std::mutex raii_mutex;

void safe_increment_lock_guard(int n) {
    for (int i = 0; i < n; ++i) {
        std::lock_guard<std::mutex> lock(raii_mutex);
        ++raii_counter;
    }  // 析构时自动解锁
}

// ============================================================
// 3. unique_lock — 可移动、可延迟加锁
// ============================================================

int ul_counter = 0;
std::mutex ul_mutex;
std::condition_variable cv;
bool ready = false;

void worker_prepare() {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    {
        std::lock_guard<std::mutex> lock(ul_mutex);
        ready = true;
    }
    cv.notify_one();
}

void worker_wait() {
    std::unique_lock<std::mutex> lock(ul_mutex);
    // unique_lock 可与 condition_variable 配合
    cv.wait(lock, [] { return ready; });
    std::cout << "  worker_wait: condition met, proceeding\n";
}

// unique_lock 可移动，将锁所有权转移
std::unique_lock<std::mutex> get_lock() {
    std::unique_lock<std::mutex> lock(ul_mutex);
    // 做一些准备工作...
    return lock;  // 移动语义，转移锁所有权
}

// ============================================================
// 4. scoped_lock (C++17) — 同时锁定多个 mutex
// ============================================================

std::mutex mtx_a;
std::mutex mtx_b;
int resource_a = 0;
int resource_b = 0;

void safe_transfer_scoped(int from_a, int from_b) {
    // scoped_lock 同时锁定两个 mutex，避免死锁
    std::scoped_lock lock(mtx_a, mtx_b);
    resource_a += from_a;
    resource_b += from_b;
}

// ============================================================
// 5. std::atomic — 原子操作
// ============================================================

std::atomic<int> atomic_counter{0};

void atomic_increment(int n) {
    for (int i = 0; i < n; ++i) {
        atomic_counter.fetch_add(1, std::memory_order_relaxed);
    }
}

// CAS 操作：实现无锁的 push（仅演示）
struct Node {
    int value;
    Node* next;
};

std::atomic<Node*> atomic_head{nullptr};

void push_node(int val) {
    Node* new_node = new Node{val, nullptr};
    Node* old_head = atomic_head.load(std::memory_order_relaxed);
    do {
        new_node->next = old_head;
        // compare_exchange_weak 可能伪失败，适合循环
    } while (!atomic_head.compare_exchange_weak(old_head, new_node,
                                                std::memory_order_release,
                                                std::memory_order_relaxed));
}

// ============================================================
// 6. std::atomic 内存序
// ============================================================

std::atomic<bool> flag{false};
int data = 0;

void producer_release() {
    data = 42;                              // A: 普通写
    flag.store(true, std::memory_order_release);  // B: release 写
    // A 在 B 之前，release 保证 A 对 acquire 可见
}

void consumer_acquire() {
    while (!flag.load(std::memory_order_acquire)) {
        std::this_thread::yield();          // C: acquire 读
    }
    // C 看到 B 的写入，因此 A 也对本线程可见
    assert(data == 42);
}

// relaxed 仅保证原子性，不保证顺序
std::atomic<int> relaxed_counter{0};

void relaxed_demo() {
    std::thread t1([] {
        for (int i = 0; i < 10000; ++i)
            relaxed_counter.fetch_add(1, std::memory_order_relaxed);
    });
    std::thread t2([] {
        for (int i = 0; i < 10000; ++i)
            relaxed_counter.fetch_add(1, std::memory_order_relaxed);
    });
    t1.join();
    t2.join();
    std::cout << "  relaxed_counter (expect 20000): " << relaxed_counter.load() << "\n";
}

// ============================================================
// 7. 数据竞争演示
// ============================================================

int race_data = 0;
std::mutex race_mutex;

void race_write_unsafe() {
    for (int i = 0; i < 5000; ++i) {
        ++race_data;  // 数据竞争：无同步的一写一读
    }
}

void race_write_safe() {
    for (int i = 0; i < 5000; ++i) {
        std::lock_guard<std::mutex> lock(race_mutex);
        ++race_data;
    }
}

// ============================================================
// 演示函数
// ============================================================

void demo_mutex() {
    std::cout << "=== 1. mutex — 裸锁 ===\n";

    // 不安全版本：数据竞争
    shared_counter = 0;
    std::thread t1(unsafe_increment, 10000);
    std::thread t2(unsafe_increment, 10000);
    t1.join();
    t2.join();
    std::cout << "  unsafe (expect 20000, may be less): " << shared_counter << "\n";

    // 安全版本：使用 mutex
    shared_counter = 0;
    std::thread t3(safe_increment_raw_mutex, 10000);
    std::thread t4(safe_increment_raw_mutex, 10000);
    t3.join();
    t4.join();
    std::cout << "  safe with mutex (expect 20000): " << shared_counter << "\n";
}

void demo_lock_guard() {
    std::cout << "\n=== 2. lock_guard — RAII 锁包装器 ===\n";

    raii_counter = 0;
    std::thread t1(safe_increment_lock_guard, 10000);
    std::thread t2(safe_increment_lock_guard, 10000);
    t1.join();
    t2.join();
    std::cout << "  lock_guard (expect 20000): " << raii_counter << "\n";
}

void demo_unique_lock() {
    std::cout << "\n=== 3. unique_lock — 可移动、可延迟加锁 ===\n";

    ready = false;
    std::thread t1(worker_prepare);
    std::thread t2(worker_wait);
    t1.join();
    t2.join();

    // unique_lock 可移动
    auto lock = get_lock();
    std::cout << "  unique_lock moved successfully\n";
    // 手动解锁
    lock.unlock();
    std::cout << "  unique_lock manually unlocked\n";
}

void demo_scoped_lock() {
    std::cout << "\n=== 4. scoped_lock — 多锁同时锁定 ===\n";

    resource_a = 0;
    resource_b = 0;
    std::thread t1(safe_transfer_scoped, 10, 20);
    std::thread t2(safe_transfer_scoped, 30, 40);
    t1.join();
    t2.join();
    std::cout << "  resource_a (expect 40): " << resource_a << "\n";
    std::cout << "  resource_b (expect 60): " << resource_b << "\n";
}

void demo_atomic() {
    std::cout << "\n=== 5. std::atomic — 原子操作 ===\n";

    atomic_counter.store(0);
    std::thread t1(atomic_increment, 10000);
    std::thread t2(atomic_increment, 10000);
    t1.join();
    t2.join();
    std::cout << "  atomic counter (expect 20000): " << atomic_counter.load() << "\n";

    // CAS 无锁栈
    push_node(1);
    push_node(2);
    push_node(3);
    std::cout << "  lock-free stack: ";
    Node* cur = atomic_head.load();
    while (cur) {
        std::cout << cur->value << " ";
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
    std::cout << "\n";
}

void demo_memory_order() {
    std::cout << "\n=== 6. std::atomic 内存序 ===\n";

    // release-acquire 同步
    for (int i = 0; i < 10; ++i) {
        data = 0;
        flag.store(false);
        std::thread t1(producer_release);
        std::thread t2(consumer_acquire);
        t1.join();
        t2.join();
    }
    std::cout << "  release-acquire: all 10 iterations passed (data == 42)\n";

    // relaxed 计数器
    relaxed_counter.store(0);
    relaxed_demo();
}

void demo_data_race() {
    std::cout << "\n=== 7. 数据竞争 ===\n";

    // 不安全：数据竞争
    race_data = 0;
    std::thread t1(race_write_unsafe);
    std::thread t2(race_write_unsafe);
    t1.join();
    t2.join();
    std::cout << "  unsafe (expect 10000, may be less): " << race_data << "\n";

    // 安全：mutex 保护
    race_data = 0;
    std::thread t3(race_write_safe);
    std::thread t4(race_write_safe);
    t3.join();
    t4.join();
    std::cout << "  safe with mutex (expect 10000): " << race_data << "\n";
}

int main() {
    demo_mutex();
    demo_lock_guard();
    demo_unique_lock();
    demo_scoped_lock();
    demo_atomic();
    demo_memory_order();
    demo_data_race();

    return 0;
}
