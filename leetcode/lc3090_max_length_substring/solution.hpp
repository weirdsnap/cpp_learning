// LC 3090. 每个字符最多出现两次的最长子字符串 — 博客 leetcode/string/ch51.md
#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace lc3090 {

// 滑动窗口版：右指针扩张，左指针收缩到合法（每字符 ≤ 2 次）
class Solution {
public:
    int maximumLengthSubstring(std::string s) {
        int left = 0, right = 0, len = 0;
        std::vector<int> cnt(26, 0);

        while (right < static_cast<int>(s.size())) {
            cnt[s[right] - 'a']++;
            while (cnt[s[right] - 'a'] > 2) {
                cnt[s[left] - 'a']--;
                left++;
            }
            len = std::max(len, right - left + 1);
            right++;
        }
        return len;
    }
};

// 暴力参照版：枚举所有子串，检查每字符出现 ≤ 2 次
class SolutionBrute {
public:
    int maximumLengthSubstring(std::string s) {
        int n = s.size(), best = 0;
        for (int i = 0; i < n; i++) {
            int cnt[26] = {0};
            for (int j = i; j < n; j++) {
                if (++cnt[s[j] - 'a'] > 2) break;
                best = std::max(best, j - i + 1);
            }
        }
        return best;
    }
};

} // namespace lc3090
