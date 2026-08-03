// LC 3014 / 3016. 按键次数最少的打字方案 I & II — 博客 leetcode/greedy/ch40.md
#pragma once

#include <algorithm>
#include <functional>
#include <string>

namespace lc3014 {

// I（3014）特判版：字母各不相同，频率全 1，逢 8 进 1 直接累加
class SolutionDistinct {
public:
    int minimumPushes(std::string word) {
        int result = 0;
        for (int i = 0; i < (int)word.size(); i++) {
            result += i / 8 + 1;
        }
        return result;
    }
};

// II（3016）通用版：频率降序排序，前 8 个 ×1、接着 8 个 ×2……（交换论证保证最优）
// 对 I 也正确，但频率统计 + 排序都是多余动作
class Solution {
public:
    int minimumPushes(std::string word) {
        int cnt[26] = {0};
        for (const auto& c : word) cnt[c - 'a']++;
        std::sort(std::begin(cnt), std::end(cnt), std::greater<int>());
        int result = 0;
        for (int i = 0; i < 26; ++i) {
            if (cnt[i] == 0) break;
            result += cnt[i] * (i / 8 + 1);
        }
        return result;
    }
};

} // namespace lc3014
