#pragma once

#include <string>
#include <unordered_map>

namespace lc1100 {

// 提交的原始版本：定长窗口 + 每步全表扫描判合法
class Solution {
public:
    int numKLenSubstrNoRepeats(std::string s, int k) {
        if (k > (int)s.size()) return 0;

        int i = 0, j = 0, result = 0;
        std::unordered_map<char, int> cnt;
        while (j < k - 1) {
            cnt[s[j] - 'a']++;
            j++;
        }
        while (j < (int)s.size()) {
            cnt[s[j] - 'a']++;
            bool valid = true;
            for (const auto& [c, time] : cnt) {
                if (time > 1) { valid = false; break; }
            }
            if (valid) result++;
            cnt[s[i] - 'a']--;
            i++;
            j++;
        }
        return result;
    }
};

// 提交的版本二：保留两段式结构，合法性改 dup 增量维护
class SolutionV2 {
public:
    int numKLenSubstrNoRepeats(std::string s, int k) {
        if (k > (int)s.size()) return 0;

        int i = 0, j = 0, result = 0;
        int duplicates = 0;
        std::unordered_map<char, int> cnt;

        while (j < k - 1) {
            if (++cnt[s[j] - 'a'] == 2) duplicates++;
            j++;
        }
        while (j < (int)s.size()) {
            if (++cnt[s[j] - 'a'] == 2) duplicates++;
            if (duplicates == 0) result++;
            if (--cnt[s[i] - 'a'] == 1) duplicates--;
            i++;
            j++;
        }
        return result;
    }
};

// 提交的版本三：两段式 + dup 计数 + 定长数组
class SolutionV3 {
public:
    int numKLenSubstrNoRepeats(std::string s, int k) {
        if (k > (int)s.size()) return 0;

        int i = 0, j = 0, result = 0;
        int duplicates = 0;
        int cnt[26] = {0};

        while (j < k - 1) {
            if (++cnt[s[j] - 'a'] == 2) duplicates++;
            j++;
        }
        while (j < (int)s.size()) {
            if (++cnt[s[j] - 'a'] == 2) duplicates++;
            if (duplicates == 0) result++;
            if (--cnt[s[i] - 'a'] == 1) duplicates--;
            i++;
            j++;
        }
        return result;
    }
};

// 参考实现：dup 计数器 + 定长数组 + 单循环，O(n)
class SolutionRef {
public:
    int numKLenSubstrNoRepeats(std::string s, int k) {
        if (k > (int)s.size()) return 0;
        int cnt[26] = {}, dup = 0, result = 0;
        for (int j = 0; j < (int)s.size(); j++) {
            if (++cnt[s[j] - 'a'] == 2) dup++;          // 新进窗字符造成重复
            if (j >= k && --cnt[s[j - k] - 'a'] == 1) dup--;  // 出窗字符消除重复
            if (j >= k - 1 && dup == 0) result++;
        }
        return result;
    }
};

} // namespace lc1100
