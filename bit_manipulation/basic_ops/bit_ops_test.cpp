#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <bit>
#include <cstdint>

bool is_power_of_2(unsigned x) {
    return x && (x & (x - 1)) == 0;
}

unsigned lowbit(unsigned x) {
    return x & -x;
}

unsigned clear_lowest_bit(unsigned x) {
    return x & (x - 1);
}

size_t align_up(size_t sz, size_t align) {
    return (sz + align - 1) & ~(align - 1);
}

TEST_CASE("判断 2 的幂") {
    CHECK(is_power_of_2(1));
    CHECK(is_power_of_2(2));
    CHECK(is_power_of_2(16));
    CHECK(is_power_of_2(1024));
    CHECK(!is_power_of_2(0));
    CHECK(!is_power_of_2(3));
    CHECK(!is_power_of_2(18));
    CHECK(!is_power_of_2(1023));
}

TEST_CASE("lowbit 只保留最低位的 1") {
    CHECK(lowbit(0b00010100) == 0b00000100);
    CHECK(lowbit(0b10000000) == 0b10000000);
    CHECK(lowbit(0b00000001) == 0b00000001);
}

TEST_CASE("x & (x - 1) 清零最低位的 1") {
    CHECK(clear_lowest_bit(0b00010100) == 0b00010000);
    CHECK(clear_lowest_bit(0b00000001) == 0b00000000);
}

TEST_CASE("设置/清除/翻转/读取某一位") {
    unsigned x = 0b00000000;
    x |= (1u << 2);       // 设置第 2 位
    CHECK(x == 0b00000100);

    x &= ~(1u << 2);      // 清除第 2 位
    CHECK(x == 0b00000000);

    x ^= (1u << 3);       // 翻转第 3 位
    CHECK(x == 0b00001000);

    bool bit = (x >> 3) & 1u;
    CHECK(bit);
}

TEST_CASE("向上对齐到 2 的幂") {
    CHECK(align_up(100, 4096) == 4096);
    CHECK(align_up(4096, 4096) == 4096);
    CHECK(align_up(4097, 4096) == 8192);
    CHECK(align_up(5000, 4096) == 8192);
    CHECK(align_up(7, 8) == 8);
    CHECK(align_up(8, 8) == 8);
    CHECK(align_up(9, 8) == 16);
}

TEST_CASE("掩码取低字节和高字节") {
    uint32_t x = 0x12345678;
    CHECK((x & 0xFFu) == 0x78);
    CHECK(((x >> 24) & 0xFFu) == 0x12);
}

TEST_CASE("XOR 交换") {
    int a = 42, b = 17;
    a ^= b;
    b ^= a;
    a ^= b;
    CHECK(a == 17);
    CHECK(b == 42);
}

TEST_CASE("popcount 统计 1 的个数") {
    CHECK(std::popcount(0b1011u) == 3);
    CHECK(std::popcount(0b0000u) == 0);
    CHECK(std::popcount(0xFFFFFFFFu) == 32);
}
