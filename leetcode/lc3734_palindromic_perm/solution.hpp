#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace lc3734 {

// 提交的原始版本：3720 单数组差值思路搬到"前半段"上
class Solution {
public:
    std::string lexPalindromicPermutation(std::string s, std::string target) {
        const int n = s.size();
        std::vector<int> s_cnt(26, 0);
        char mid = 0;
        for (char c : s) s_cnt[c - 'a']++;
        for (int i = 0; i < 26; i++) {
            if (s_cnt[i] % 2) {
                if (mid != 0) return "";   // 超过 1 个奇数字母
                mid = 'a' + i;
            }
            s_cnt[i] /= 2;
        }

        int m = n / 2;
        std::string t = target.substr(0, m);

        bool tFeasible = true;
        std::vector<int> tmp = s_cnt;
        for (char c : t) {
            if (tmp[c - 'a'] == 0) { tFeasible = false; break; }
            tmp[c - 'a']--;
        }

        std::string base = t;
        if (mid) base += mid;
        std::string rev = t;
        std::reverse(rev.begin(), rev.end());
        base += rev;
        bool canEqual = base > target;

        auto makePal = [&](std::string p) {
            std::string ans = p;
            if (mid) ans += mid;
            std::string r = p;
            std::reverse(r.begin(), r.end());
            ans += r;
            return ans;
        };

        if (canEqual && tFeasible) return makePal(t);

        for (int i = 0; i < m; i++) s_cnt[t[i] - 'a']--;

        for (int i = m - 1; i >= 0; i--) {
            s_cnt[t[i] - 'a']++;
            bool valid = true;
            for (int c : s_cnt) {
                if (c < 0) { valid = false; break; }
            }
            if (!valid) continue;
            for (int j = t[i] - 'a' + 1; j < 26; j++) {
                if (s_cnt[j] == 0) continue;
                s_cnt[j]--;
                t[i] = 'a' + j;
                t.resize(i + 1);
                for (int k = 0; k < 26; k++) t += std::string(s_cnt[k], 'a' + k);
                return makePal(t);
            }
        }
        return "";
    }
};

// 参考实现：穷举前半段的全部独特排列，拼回文取 > target 的最小者
class SolutionBrute {
public:
    std::string lexPalindromicPermutation(std::string s, std::string target) {
        int cnt[26] = {};
        for (char c : s) cnt[c - 'a']++;
        std::string half, mid;
        for (int i = 0; i < 26; i++) {
            if (cnt[i] % 2) {
                if (!mid.empty()) return "";
                mid = char('a' + i);
            }
            half.append(cnt[i] / 2, 'a' + i);
        }
        std::sort(half.begin(), half.end());
        std::string best;
        do {
            std::string pal =
                half + mid + std::string(half.rbegin(), half.rend());
            if (pal > target && (best.empty() || pal < best)) best = pal;
        } while (std::next_permutation(half.begin(), half.end()));
        return best;
    }
};

} // namespace lc3734
