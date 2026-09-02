#pragma once
#include <algorithm>
#include <string>
#include <vector>

namespace lc266 {

using std::string;
using std::vector;

// 用户提交版：计数后奇数频次字符至多一个
class Solution {
public:
    bool canPermutePalindrome(string s) {
        vector<int> cnt(26, 0);
        for (const auto& c : s) {
            cnt[c - 'a']++;
        }
        bool odd = false;
        for (const auto& c : cnt) {
            if (c & 1) {
                if (!odd) {
                    odd = true;
                } else {
                    return false;
                }
            }
        }
        return true;
    }
};

// 独立对拍：暴力枚举所有排列，存在回文排列则 true。仅用于短串。
inline bool bruteForce(string s) {
    if (s.empty()) return true;  // 空串视为回文（题目约束 len >= 1，仅防御）
    std::sort(s.begin(), s.end());
    do {
        bool pal = true;
        for (size_t i = 0, j = s.size() - 1; i < j; i++, j--) {
            if (s[i] != s[j]) { pal = false; break; }
        }
        if (pal) return true;
    } while (std::next_permutation(s.begin(), s.end()));
    return false;
}

} // namespace lc266
