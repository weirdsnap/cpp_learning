#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdint>

uint8_t hamming74_encode(uint8_t nibble) {
    uint8_t d1 = (nibble >> 0) & 1;
    uint8_t d2 = (nibble >> 1) & 1;
    uint8_t d3 = (nibble >> 2) & 1;
    uint8_t d4 = (nibble >> 3) & 1;

    uint8_t p1 = d1 ^ d2 ^ d4;
    uint8_t p2 = d1 ^ d3 ^ d4;
    uint8_t p3 = d2 ^ d3 ^ d4;

    return (p1 << 0) | (p2 << 1) | (d1 << 2) |
           (p3 << 3) | (d2 << 4) | (d3 << 5) | (d4 << 6);
}

uint8_t hamming74_decode(uint8_t code, bool* corrected = nullptr) {
    uint8_t p1 = (code >> 0) & 1;
    uint8_t p2 = (code >> 1) & 1;
    uint8_t d1 = (code >> 2) & 1;
    uint8_t p3 = (code >> 3) & 1;
    uint8_t d2 = (code >> 4) & 1;
    uint8_t d3 = (code >> 5) & 1;
    uint8_t d4 = (code >> 6) & 1;

    uint8_t s1 = p1 ^ d1 ^ d2 ^ d4;
    uint8_t s2 = p2 ^ d1 ^ d3 ^ d4;
    uint8_t s3 = p3 ^ d2 ^ d3 ^ d4;

    uint8_t syndrome = (s3 << 2) | (s2 << 1) | s1;
    if (syndrome != 0) {
        code ^= (1u << (syndrome - 1));
        if (corrected) *corrected = true;
    } else {
        if (corrected) *corrected = false;
    }

    uint8_t d1c = (code >> 2) & 1;
    uint8_t d2c = (code >> 4) & 1;
    uint8_t d3c = (code >> 5) & 1;
    uint8_t d4c = (code >> 6) & 1;
    return (d4c << 3) | (d3c << 2) | (d2c << 1) | d1c;
}

TEST_CASE("Hamming(7,4) 无错解码") {
    for (uint8_t data = 0; data < 16; ++data) {
        uint8_t code = hamming74_encode(data);
        bool corrected = false;
        uint8_t decoded = hamming74_decode(code, &corrected);
        CHECK(decoded == data);
        CHECK(!corrected);
    }
}

TEST_CASE("Hamming(7,4) 纠正单比特错误") {
    for (uint8_t data = 0; data < 16; ++data) {
        uint8_t code = hamming74_encode(data);
        for (int bit = 0; bit < 7; ++bit) {
            uint8_t corrupted = code ^ (1u << bit);
            bool corrected = false;
            uint8_t decoded = hamming74_decode(corrupted, &corrected);
            CHECK(decoded == data);
            CHECK(corrected);
        }
    }
}

TEST_CASE("Hamming(7,4) 双比特错误无法正确纠正") {
    uint8_t data = 0b1010;
    uint8_t code = hamming74_encode(data);
    // 翻转第 0 和第 1 位
    uint8_t corrupted = code ^ 0x03;
    bool corrected = false;
    uint8_t decoded = hamming74_decode(corrupted, &corrected);
    // 它仍会“纠正”其中一位，但结果不是原数据
    CHECK(corrected);
    CHECK(decoded != data);
}
