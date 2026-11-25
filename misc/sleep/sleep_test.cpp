#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <thread>
#include <chrono>

TEST_CASE("sleep_for 基本功能") {
    auto start = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    auto elapsed = std::chrono::steady_clock::now() - start;

    // 至少睡了 10ms（允许调度误差）
    CHECK(elapsed >= std::chrono::milliseconds(10));
}

TEST_CASE("sleep_until 基本功能") {
    auto target = std::chrono::steady_clock::now() + std::chrono::milliseconds(10);
    std::this_thread::sleep_until(target);

    auto now = std::chrono::steady_clock::now();
    CHECK(now >= target);
}

TEST_CASE("sleep_for 微秒级") {
    auto start = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    auto elapsed = std::chrono::steady_clock::now() - start;

    // 至少睡了 100us
    CHECK(elapsed >= std::chrono::microseconds(100));
}
