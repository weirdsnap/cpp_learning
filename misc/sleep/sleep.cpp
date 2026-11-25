// 验证 ch10/05 C++ 中的睡眠
//
// 1. C 风格 sleep/usleep/nanosleep
// 2. std::this_thread::sleep_for / sleep_until
// 3. 自旋等待
// 4. 混合策略：sleep + 自旋

#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <unistd.h>

// ============================================================
// 1. C 风格睡眠
// ============================================================

void demo_c_style_sleep() {
    std::cout << "=== 1. C 风格睡眠 ===\n";

    // sleep：秒级
    std::cout << "  sleep(1): 睡 1 秒...\n";
    sleep(1);
    std::cout << "  醒来\n";

    // usleep：微秒级（已废弃）
    std::cout << "  usleep(100000): 睡 100 毫秒...\n";
    usleep(100000);
    std::cout << "  醒来\n";

    // nanosleep：纳秒级
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 50000000L;  // 50 毫秒
    std::cout << "  nanosleep: 睡 50 毫秒...\n";
    nanosleep(&ts, nullptr);
    std::cout << "  醒来\n";
}

// ============================================================
// 2. std::this_thread::sleep_for / sleep_until
// ============================================================

void demo_modern_sleep() {
    std::cout << "\n=== 2. 现代 C++ sleep_for / sleep_until ===\n";

    // sleep_for：睡多久
    std::cout << "  sleep_for(100ms)...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "  醒来\n";

    std::cout << "  sleep_for(500us)...\n";
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    std::cout << "  醒来\n";

    // sleep_until：睡到什么时候
    auto start = std::chrono::steady_clock::now();
    auto next = start + std::chrono::milliseconds(100);

    std::cout << "  sleep_until(now + 100ms)...\n";
    std::this_thread::sleep_until(next);

    auto elapsed = std::chrono::steady_clock::now() - start;
    std::cout << "  实际睡了 "
              << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count()
              << " ms\n";
}

// ============================================================
// 3. 自旋等待
// ============================================================

void demo_spin_wait() {
    std::cout << "\n=== 3. 自旋等待 ===\n";

    auto start = std::chrono::high_resolution_clock::now();
    auto target = start + std::chrono::microseconds(100);

    volatile int dummy = 0;  // 防止被优化掉
    while (std::chrono::high_resolution_clock::now() < target) {
        dummy++;  // 空转
    }

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    std::cout << "  自旋 100us, 实际耗时 "
              << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
              << " us (dummy=" << dummy << ")\n";
}

// ============================================================
// 4. 混合策略
// ============================================================

void demo_hybrid_wait() {
    std::cout << "\n=== 4. 混合策略：sleep + 自旋 ===\n";

    auto target = std::chrono::steady_clock::now() + std::chrono::microseconds(500);

    // 先 sleep 到目标前 1 毫秒
    std::this_thread::sleep_until(target - std::chrono::microseconds(1000));

    // 最后 1 毫秒自旋，保证精度
    volatile int dummy = 0;
    while (std::chrono::steady_clock::now() < target) {
        dummy++;
    }

    std::cout << "  混合等待完成 (dummy=" << dummy << ")\n";
}

int main() {
    std::cout << "睡眠演示（注意：实际精度取决于操作系统调度器）\n\n";

    demo_c_style_sleep();
    demo_modern_sleep();
    demo_spin_wait();
    demo_hybrid_wait();

    return 0;
}
