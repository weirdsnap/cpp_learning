// 验证简单校验方法：
// 1. 奇偶校验位：只能检测奇数个 bit 翻转
// 2. 字节异或校验和：简单累加检错能力弱

#include <cstdint>
#include <cstddef>
#include <iostream>
#include <string>

// 计算字节异或校验和
uint8_t xor_checksum(const void* data, size_t len) {
    const uint8_t* p = static_cast<const uint8_t*>(data);
    uint8_t sum = 0;
    for (size_t i = 0; i < len; ++i) sum ^= p[i];
    return sum;
}

// 计算奇偶校验位：1 的个数为奇数返回 1，偶数返回 0
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

int main() {
    std::string msg = "hello";
    std::cout << "xor_checksum = 0x" << std::hex
              << static_cast<int>(xor_checksum(msg.data(), msg.size())) << std::dec << "\n";
    std::cout << "parity_bit = " << parity_bit(msg.data(), msg.size()) << "\n";

    // 翻转两个 bit：奇偶校验无法检测
    std::string corrupted = msg;
    corrupted[0] ^= 0x03;  // 翻转两个 bit
    std::cout << "after two-bit flip parity = "
              << parity_bit(corrupted.data(), corrupted.size()) << "\n";

    return 0;
}
