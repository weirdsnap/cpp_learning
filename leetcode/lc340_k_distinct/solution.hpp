#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>

namespace lc340 {

// 提交的原始版本（标准滑窗模板）
class Solution {
public:
    int lengthOfLongestSubstringKDistinct(std::string s, int k) {
        const int n = s.size();
        int i = 0;
        int j = 0;
        int result = 0;
        std::unordered_map<char, int> cnt;
        while (j < n) {
            cnt[s[j]]++;
            while ((int)cnt.size() > k) {
                cnt[s[i]]--;
                if (cnt[s[i]] == 0) {
                    cnt.erase(s[i]);
                }
                i++;
            }
            result = std::max(result, j - i + 1);
            j++;
        }
        return result;
    }
};

// 参考实现：暴力枚举起点，逐位扩张数不同字符，O(n^2)
class SolutionBrute {
public:
    int lengthOfLongestSubstringKDistinct(std::string s, int k) {
        const int n = s.size();
        int result = 0;
        for (int a = 0; a < n; a++) {
            bool seen[256] = {false};
            int distinct = 0;
            for (int b = a; b < n; b++) {
                if (!seen[(unsigned char)s[b]]) {
                    seen[(unsigned char)s[b]] = true;
                    distinct++;
                }
                if (distinct > k) break;
                result = std::max(result, b - a + 1);
            }
        }
        return result;
    }
};

} // namespace lc340
