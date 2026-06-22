#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdint>
#include <cstddef>
#include <cstring>
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

TEST_CASE("相同数据 CRC32 相同") {
    std::string a = "hello world";
    std::string b = "hello world";
    CHECK(crc32(a.data(), a.size()) == crc32(b.data(), b.size()));
}

TEST_CASE("CRC32 能检测单比特翻转") {
    std::string a = "hello world";
    std::string b = a;
    b[0] ^= 0x01;  // 翻转一个 bit
    CHECK(crc32(a.data(), a.size()) != crc32(b.data(), b.size()));
}

TEST_CASE("CRC32 能检测两位交换") {
    std::string a = "abcd";
    std::string b = "abdc";  // 交换 c 和 d
    CHECK(crc32(a.data(), a.size()) != crc32(b.data(), b.size()));
}

TEST_CASE("CRC32 不能纠错") {
    std::string original = "hello world";
    std::string corrupted = original;
    corrupted[0] ^= 0x01;

    uint32_t original_crc = crc32(original.data(), original.size());
    uint32_t corrupted_crc = crc32(corrupted.data(), corrupted.size());

    // 只能发现不同，无法从 corrupted_crc 反推出原数据
    CHECK(original_crc != corrupted_crc);
    CHECK(corrupted != original);
}
