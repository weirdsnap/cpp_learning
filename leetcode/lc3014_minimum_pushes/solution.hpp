// LC 3014. 按键次数最少的打字方案 I — 博客 leetcode/greedy/ch40.md
#pragma once

#include <algorithm>
#include <functional>
#include <string>

namespace lc3014 {

// 频率降序排序，前 8 个 ×1、接着 8 个 ×2……（交换论证保证最优）
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
