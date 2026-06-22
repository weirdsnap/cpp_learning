// 验证 Hamming(7,4) 码：
// 4 位数据 + 3 位校验，可纠正 1 位错误，检测 2 位错误。
// 与 CRC32 形成对比：CRC 只能检错，Hamming 可以纠错。

#include <cstdint>
#include <iostream>

// 编码：4 位数据 -> 7 位码字
// 位置：1 2 3 4 5 6 7
// 校验位：p1@1, p2@2, p3@4
// 数据位：d1@3, d2@5, d3@6, d4@7
uint8_t hamming74_encode(uint8_t nibble) {
    uint8_t d1 = (nibble >> 0) & 1;
    uint8_t d2 = (nibble >> 1) & 1;
    uint8_t d3 = (nibble >> 2) & 1;
    uint8_t d4 = (nibble >> 3) & 1;

    uint8_t p1 = d1 ^ d2 ^ d4;
    uint8_t p2 = d1 ^ d3 ^ d4;
    uint8_t p3 = d2 ^ d3 ^ d4;

    // 位位置 1..7
    return (p1 << 0) | (p2 << 1) | (d1 << 2) |
           (p3 << 3) | (d2 << 4) | (d3 << 5) | (d4 << 6);
}

// 解码并纠错：返回低 4 位为数据，第 7 位表示是否发生了纠正
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
        //  syndrome 即出错位的位置
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

int main() {
    uint8_t data = 0b1010;
    uint8_t code = hamming74_encode(data);
    std::cout << "data=" << static_cast<int>(data)
              << " code=" << static_cast<int>(code) << "\n";

    // 人为翻转 1 位
    uint8_t corrupted = code ^ (1u << 3);
    bool corrected = false;
    uint8_t recovered = hamming74_decode(corrupted, &corrected);
    std::cout << "corrupted=" << static_cast<int>(corrupted)
              << " corrected=" << corrected
              << " recovered=" << static_cast<int>(recovered) << "\n";

    return 0;
}
