// LC 1386. 安排电影院座位 — 博客 leetcode/bit/ch65.md
#pragma once

#include <bit>
#include <unordered_map>
#include <vector>

namespace lc1386 {

// 提交版：位掩码 + popcount 检测块占用，n*2 减去损失
class Solution {
public:
    int maxNumberOfFamilies(int n, std::vector<std::vector<int>>& reservedSeats) {
        std::unordered_map<int, unsigned int> cnt;
        for (const auto& seat : reservedSeats) {
            if (seat[1] == 1 || seat[1] == 10) continue;   // 不挡任何四人块
            cnt[seat[0]] += 1 << (9 - seat[1]);            // 座位 2→bit7 ... 座位 9→bit0
        }
        int lost = 0;
        for (const auto& [k, v] : cnt) {
            bool left   = std::popcount(0b11110000 & v) > 0;  // 2-5
            bool right  = std::popcount(0b00001111 & v) > 0;  // 6-9
            bool middle = std::popcount(0b00111100 & v) > 0;  // 4-7
            if (middle) {
                lost += (left && right) ? 2 : 1;
            } else {
                if (left || right) lost += 1;
            }
        }
        return n * 2 - lost;
    }
};

// 参照版：每行直接枚举三个块的最大可放数（不依赖 popcount 技巧）
class SolutionRef {
public:
    int maxNumberOfFamilies(int n, std::vector<std::vector<int>>& reservedSeats) {
        std::unordered_map<int, unsigned int> cnt;
        for (const auto& seat : reservedSeats) {
            if (seat[1] == 1 || seat[1] == 10) continue;
            cnt[seat[0]] += 1 << (9 - seat[1]);
        }
        int total = n * 2;
        for (const auto& [k, v] : cnt) {
            bool leftFree   = (v & 0b11110000) == 0;
            bool rightFree  = (v & 0b00001111) == 0;
            bool middleFree = (v & 0b00111100) == 0;
            int best;
            if (leftFree && rightFree)      best = 2;  // 左右两块不重叠
            else if (leftFree || middleFree || rightFree) best = 1;
            else                            best = 0;
            total -= (2 - best);
        }
        return total;
    }
};

} // namespace lc1386
