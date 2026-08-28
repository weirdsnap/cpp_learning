#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace lc3720 {

// 提交的版本一：增量维护 target 前缀计数
class Solution {
public:
    std::string lexGreaterPermutation(std::string s, std::string target) {
        std::vector<int> cnt(26, 0);
        for (const auto& c : target) cnt[c - 'a']++;
        std::vector<int> s_cnt(26, 0);
        for (const auto& c : s) s_cnt[c - 'a']++;

        for (int i = (int)s.size() - 1; i >= 0; i--) {
            cnt[target[i] - 'a']--;   // cnt 变为 target[0..i-1] 的计数
            bool valid = true;
            std::vector<int> right(26, 0);
            for (int j = 0; j < 26; j++) {
                right[j] = s_cnt[j] - cnt[j];
                if (right[j] < 0) { valid = false; break; }
            }
            if (!valid) continue;

            valid = false;
            char mid;
            for (int j = target[i] - 'a' + 1; j < 26; j++) {
                if (right[j] > 0) {
                    valid = true;
                    right[j]--;
                    mid = 'a' + j;
                    break;
                }
            }
            if (!valid) continue;

            std::string ans = target.substr(0, i);
            ans += mid;
            for (int j = 0; j < 26; j++) ans.append(right[j], 'a' + j);
            return ans;
        }
        return "";
    }
};

// 提交的版本二：每个 i 复制库存重新扣
class SolutionV2 {
public:
    std::string lexGreaterPermutation(std::string s, std::string target) {
        int n = s.size();
        std::vector<int> s_cnt(26, 0);
        for (const auto& c : s) s_cnt[c - 'a']++;

        for (int i = n - 1; i >= 0; i--) {
            std::vector<int> tmp = s_cnt;
            bool ok = true;
            for (int k = 0; k < i; k++) {
                if (tmp[target[k] - 'a'] > 0) tmp[target[k] - 'a']--;
                else { ok = false; break; }
            }
            if (!ok) continue;

            int mid_idx = -1;
            for (int j = target[i] - 'a' + 1; j < 26; j++) {
                if (tmp[j] > 0) { mid_idx = j; tmp[j]--; break; }
            }
            if (mid_idx == -1) continue;

            std::string ans = target.substr(0, i);
            ans += char('a' + mid_idx);
            for (int j = 0; j < 26; j++) ans.append(tmp[j], 'a' + j);
            return ans;
        }
        return "";
    }
};

// 提交的版本三：单数组差值，left[c] = s_cnt[c] - target_cnt[c]
class SolutionV3 {
public:
    std::string lexGreaterPermutation(std::string s, std::string target) {
        int n = s.size();
        int left[26] = {};
        for (int i = 0; i < n; i++) {
            left[s[i] - 'a']++;
            left[target[i] - 'a']--;
        }

        for (int i = n - 1; i >= 0; i--) {
            left[target[i] - 'a']++;   // 前缀缩短一位，还回 target[i]

            bool ok = true;
            for (int c : left) {
                if (c < 0) { ok = false; break; }
            }
            if (!ok) continue;

            for (int j = target[i] - 'a' + 1; j < 26; j++) {
                if (left[j] == 0) continue;
                left[j]--;
                target[i] = 'a' + j;
                target.resize(i + 1);
                for (int k = 0; k < 26; k++) {
                    target += std::string(left[k], 'a' + k);
                }
                return target;
            }
        }
        return "";
    }
};

// 参考实现：排序后 next_permutation 枚举，第一个 > target 的即答案
class SolutionBrute {
public:
    std::string lexGreaterPermutation(std::string s, std::string target) {
        std::sort(s.begin(), s.end());
        do {
            if (s > target) return s;
        } while (std::next_permutation(s.begin(), s.end()));
        return "";
    }
};

} // namespace lc3720
