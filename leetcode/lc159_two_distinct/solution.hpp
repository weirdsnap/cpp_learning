#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>

namespace lc159 {

// 提交的原始版本
class Solution {
public:
    int lengthOfLongestSubstringTwoDistinct(std::string s) {
        if (s.size() <= 2) {
            return s.size();
        }

        int i = 0;
        int j = 1;
        int result = 2;

        std::unordered_map<char, int> cnt;
        cnt[s[i]]++;
        while (j < (int)s.size() && s[j] == s[i]) {
            cnt[s[j]]++;
            j++;
        }
        if (j < (int)s.size()) {
            cnt[s[j]]++;
            result = std::max(result, j - i + 1);
        } else {
            return s.size();
        }

        while (j < (int)s.size()) {
            ++j;
            cnt[s[j]]++;                 // j 可能越到 s.size()，读到 '\0'
            if (cnt[s[j]] == 1) {        // 新字符进入窗口
                while (true) {
                    cnt[s[i]]--;
                    if (cnt[s[i]] == 0) {
                        i++;
                        break;
                    }
                    i++;
                }
            }
            result = std::max(result, j - i + 1);
        }
        return result;
    }
};

// 参考实现：标准滑动窗口模板，j 由 for 循环管理，无越界读
class SolutionRef {
public:
    int lengthOfLongestSubstringTwoDistinct(std::string s) {
        std::unordered_map<char, int> cnt;
        int i = 0, result = 0;
        for (int j = 0; j < (int)s.size(); j++) {
            cnt[s[j]]++;
            while ((int)cnt.size() > 2) {
                if (--cnt[s[i]] == 0) cnt.erase(s[i]);
                i++;
            }
            result = std::max(result, j - i + 1);
        }
        return result;
    }
};

} // namespace lc159
