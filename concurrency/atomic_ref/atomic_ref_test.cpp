#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <atomic>
#include <thread>
#include <vector>

TEST_CASE("atomic_ref 基本读写") {
    int value = 42;
    {
        std::atomic_ref<int> ref(value);
        CHECK(ref.load() == 42);
        ref.store(100);
        CHECK(ref.load() == 100);
    }
    CHECK(value == 100);
}

TEST_CASE("atomic_ref fetch_add") {
    int value = 0;
    {
        std::atomic_ref<int> ref(value);
        int prev = ref.fetch_add(10);
        CHECK(prev == 0);
        CHECK(ref.load() == 10);
    }
}

TEST_CASE("atomic_ref 多线程累加") {
    int counter = 0;
    constexpr int N = 10000;
    constexpr int THREADS = 4;

    {
        std::vector<std::thread> threads;
        for (int t = 0; t < THREADS; ++t) {
            threads.emplace_back([&counter] {
                std::atomic_ref<int> ref(counter);
                for (int i = 0; i < N; ++i) {
                    ref.fetch_add(1);
                }
            });
        }
        for (auto& t : threads) t.join();
    }

    CHECK(counter == N * THREADS);
}

TEST_CASE("atomic_wait / notify_one") {
    std::atomic<int> flag{0};
    int data = 0;

    std::thread t([&] {
        flag.wait(0);
        CHECK(data == 42);
    });

    data = 42;
    flag.store(1);
    flag.notify_one();

    t.join();
}
