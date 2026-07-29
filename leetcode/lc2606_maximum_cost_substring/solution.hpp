// LC 2606. 最大开销子字符串 — 博客 leetcode/dp/ch36.md
#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace lc2606 {

// Kadane 变体：字符 → 开销映射后就是 LC 53，区别是允许空子串（result 初始 0）
class Solution {
public:
    int maximumCostSubstring(std::string s, std::string chars, std::vector<int>& vals) {
        int values[26];
        for (int i = 0; i < 26; i++) {
            values[i] = i + 1;
        }
        for (int i = 0; i < (int)chars.size(); i++) {
            values[chars[i] - 'a'] = vals[i];
        }
        int result = 0;
        int last = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            last = std::max(last + values[s[i] - 'a'], values[s[i] - 'a']);
            result = std::max(result, last);
        }
        return result;
    }
};

} // namespace lc2606
