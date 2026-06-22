// 验证常用位运算惯用法：
// 2 的幂判断、lowbit、置位/清位/翻位、向上对齐、字节掩码、交换、popcount

#include <bit>
#include <cstdint>
#include <iostream>

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

int main() {
    std::cout << std::boolalpha;
    std::cout << "is_power_of_2(16) = " << is_power_of_2(16) << "\n";
    std::cout << "is_power_of_2(18) = " << is_power_of_2(18) << "\n";

    unsigned x = 0b00010100;
    std::cout << "lowbit = 0b" << lowbit(x) << "\n";
    std::cout << "clear_lowest = 0b" << clear_lowest_bit(x) << "\n";

    std::cout << "align_up(5000, 4096) = " << align_up(5000, 4096) << "\n";

    unsigned v = 0x12345678;
    std::cout << "low byte = 0x" << (v & 0xFFu) << "\n";
    std::cout << "high byte = 0x" << ((v >> 24) & 0xFFu) << "\n";

    std::cout << "popcount(0b1011) = " << std::popcount(0b1011u) << "\n";

    return 0;
}
