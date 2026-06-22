// 验证 CRC32：只能检错，不能纠错
// 通过查表法快速计算 32 位循环冗余校验码

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>

uint32_t crc32(const void* data, size_t len) {
    static uint32_t table[256];
    static bool init = false;
    if (!init) {
        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t c = i;
            for (int j = 0; j < 8; ++j)
                c = (c >> 1) ^ (0xEDB88320u & -(c & 1u));
            table[i] = c;
        }
        init = true;
    }
    const uint8_t* p = static_cast<const uint8_t*>(data);
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; ++i)
        crc = table[(crc ^ p[i]) & 0xFFu] ^ (crc >> 8);
    return ~crc;
}

int main() {
    std::string msg = "hello world";
    uint32_t c1 = crc32(msg.data(), msg.size());
    std::cout << "CRC32(\"" << msg << "\") = 0x" << std::hex << c1 << std::dec << "\n";

    // 翻转一个 bit，CRC 会变
    msg[0] ^= 0x01;
    uint32_t c2 = crc32(msg.data(), msg.size());
    std::cout << "after flip one bit: 0x" << std::hex << c2 << std::dec << "\n";
    std::cout << "same? " << (c1 == c2 ? "yes" : "no") << "\n";

    return 0;
}
