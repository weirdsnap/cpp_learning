#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdint>
#include <cstddef>
#include <string>

uint8_t xor_checksum(const void* data, size_t len) {
    const uint8_t* p = static_cast<const uint8_t*>(data);
    uint8_t sum = 0;
    for (size_t i = 0; i < len; ++i) sum ^= p[i];
    return sum;
}

int parity_bit(const void* data, size_t len) {
    const uint8_t* p = static_cast<const uint8_t*>(data);
    int count = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t v = p[i];
        while (v) {
            count += v & 1;
            v >>= 1;
        }
    }
    return count & 1;
}

TEST_CASE("相同数据 XOR 校验和相同") {
    std::string a = "hello";
    std::string b = "hello";
    CHECK(xor_checksum(a.data(), a.size()) == xor_checksum(b.data(), b.size()));
}

TEST_CASE("XOR 校验和能检测单字节翻转") {
    std::string a = "hello";
    std::string b = a;
    b[0] ^= 0x01;
    CHECK(xor_checksum(a.data(), a.size()) != xor_checksum(b.data(), b.size()));
}

TEST_CASE("奇偶校验位能检测奇数个 bit 翻转") {
    std::string a = "hello";
    std::string b = a;
    b[0] ^= 0x01;  // 翻转 1 个 bit
    CHECK(parity_bit(a.data(), a.size()) != parity_bit(b.data(), b.size()));
}

TEST_CASE("奇偶校验位无法检测偶数个 bit 翻转") {
    std::string a = "hello";
    std::string b = a;
    b[0] ^= 0x03;  // 翻转 2 个 bit
    CHECK(parity_bit(a.data(), a.size()) == parity_bit(b.data(), b.size()));
}
