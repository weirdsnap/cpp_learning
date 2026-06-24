#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <vector>

// ============================================================
// 1. mutex — 裸锁
// ============================================================

TEST_CASE("mutex 裸锁保护临界区") {
    int counter = 0;
    std::mutex mtx;

    auto worker = [&](int n) {
        for (int i = 0; i < n; ++i) {
            mtx.lock();
            ++counter;
            mtx.unlock();
        }
    };

    std::thread t1(worker, 5000);
    std::thread t2(worker, 5000);
    t1.join();
    t2.join();

    CHECK(counter == 10000);
}

// ============================================================
// 2. lock_guard — RAII 锁包装器
// ============================================================

TEST_CASE("lock_guard RAII 自动加解锁") {
    int counter = 0;
    std::mutex mtx;

    auto worker = [&](int n) {
        for (int i = 0; i < n; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            ++counter;
        }
    };

    std::thread t1(worker, 5000);
    std::thread t2(worker, 5000);
    t1.join();
    t2.join();

    CHECK(counter == 10000);
}

// ============================================================
// 3. unique_lock — 可移动、可延迟加锁
// ============================================================

TEST_CASE("unique_lock 与 condition_variable 配合") {
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;
    bool proceeded = false;

    std::thread waiter([&] {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return ready; });
        proceeded = true;
    });

    std::thread preparer([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        {
            std::lock_guard<std::mutex> lock(mtx);
            ready = true;
        }
        cv.notify_one();
    });

    preparer.join();
    waiter.join();

    CHECK(proceeded == true);
}

TEST_CASE("unique_lock 可移动转移所有权") {
    std::mutex mtx;

    auto get_lock = [&]() -> std::unique_lock<std::mutex> {
        std::unique_lock<std::mutex> lock(mtx);
        return lock;  // 移动
    };

    auto lock = get_lock();
    CHECK(lock.owns_lock());
    lock.unlock();
    CHECK_FALSE(lock.owns_lock());
}

TEST_CASE("unique_lock 延迟加锁") {
    std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    CHECK_FALSE(lock.owns_lock());
    lock.lock();
    CHECK(lock.owns_lock());
}

// ============================================================
// 4. scoped_lock (C++17) — 同时锁定多个 mutex
// ============================================================

TEST_CASE("scoped_lock 同时锁定多个 mutex") {
    int a = 0, b = 0;
    std::mutex mtx_a, mtx_b;

    auto worker = [&](int da, int db) {
        std::scoped_lock lock(mtx_a, mtx_b);
        a += da;
        b += db;
    };

    std::thread t1(worker, 10, 20);
    std::thread t2(worker, 30, 40);
    t1.join();
    t2.join();

    CHECK(a == 40);
    CHECK(b == 60);
}

// ============================================================
// 5. std::atomic — 原子操作
// ============================================================

TEST_CASE("atomic fetch_add 无锁递增") {
    std::atomic<int> counter{0};

    auto worker = [&](int n) {
        for (int i = 0; i < n; ++i) {
            counter.fetch_add(1, std::memory_order_relaxed);
        }
    };

    std::thread t1(worker, 10000);
    std::thread t2(worker, 10000);
    t1.join();
    t2.join();

    CHECK(counter.load() == 20000);
}

TEST_CASE("atomic CAS compare_exchange_weak") {
    std::atomic<int> val{0};

    // CAS 成功：期望值匹配
    int expected = 0;
    bool success = val.compare_exchange_weak(expected, 42);
    CHECK(success);
    CHECK(val.load() == 42);

    // CAS 失败：期望值不匹配
    expected = 0;
    success = val.compare_exchange_weak(expected, 100);
    CHECK_FALSE(success);
    CHECK(expected == 42);  // expected 被更新为当前值
}

// ============================================================
// 6. std::atomic 内存序
// ============================================================

TEST_CASE("release-acquire 建立 happens-before") {
    std::atomic<bool> flag{false};
    int data = 0;

    std::thread producer([&] {
        data = 42;
        flag.store(true, std::memory_order_release);
    });

    std::thread consumer([&] {
        while (!flag.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        // release-acquire 保证 data == 42
        CHECK(data == 42);
    });

    producer.join();
    consumer.join();
}

TEST_CASE("relaxed 保证原子性但不保证顺序") {
    std::atomic<int> counter{0};

    auto worker = [&](int n) {
        for (int i = 0; i < n; ++i) {
            counter.fetch_add(1, std::memory_order_relaxed);
        }
    };

    std::thread t1(worker, 10000);
    std::thread t2(worker, 10000);
    t1.join();
    t2.join();

    // relaxed 保证最终结果正确（原子性）
    CHECK(counter.load() == 20000);
}

// ============================================================
// 7. 数据竞争
// ============================================================

TEST_CASE("mutex 保护消除数据竞争") {
    int counter = 0;
    std::mutex mtx;

    auto worker = [&](int n) {
        for (int i = 0; i < n; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            ++counter;
        }
    };

    std::thread t1(worker, 5000);
    std::thread t2(worker, 5000);
    t1.join();
    t2.join();

    CHECK(counter == 10000);
}

TEST_CASE("多线程只读不构成数据竞争") {
    const int data = 42;
    std::atomic<int> sum{0};

    auto reader = [&] {
        sum.fetch_add(data, std::memory_order_relaxed);
    };

    std::thread t1(reader);
    std::thread t2(reader);
    t1.join();
    t2.join();

    CHECK(sum.load() == 84);
}
