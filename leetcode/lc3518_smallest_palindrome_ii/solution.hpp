// LC 3518. 最小回文重排 II — 博客 leetcode/string/ch39.md
#pragma once

#include <string>
#include <vector>

namespace lc3518 {

// 逐位确定多重集第 k 小排列，组合数累乘 + 截断防溢出
class Solution {
public:
    std::string smallestPalindrome(std::string s, int k) {
        std::string mid = s.size() % 2 == 1 ? s.substr(s.size() / 2, 1) : "";
        std::string half = s.substr(0, s.size() / 2);

        std::vector<int> cnt(26, 0);
        for (const auto& c : half) cnt[c - 'a']++;

        int remaining = half.size();
        for (int i = 0; i < (int)half.size(); i++) {
            bool flag = false;
            for (int j = 0; j < 26; j++) {
                if (cnt[j] == 0) continue;
                cnt[j]--;
                remaining--;
                long long total = perms(cnt, remaining, k);
                if (total < k) {
                    k -= total;
                    cnt[j]++;
                    remaining++;
                } else {
                    half[i] = 'a' + j;
                    flag = true;
                    break;
                }
            }
            if (!flag) return "";
        }
        return half + mid + std::string(half.rbegin(), half.rend());
    }

private:
    // remaining! / ∏cnt[t]!，结果 > cap 时返回 cap+1
    long long perms(const std::vector<int>& cnt, int remaining, long long cap) {
        long long total = 1;
        int rem = remaining;
        for (int t = 0; t < 26; t++) {
            int r = cnt[t];
            for (int d = 1; d <= r; d++) {
                total = total * (rem - r + d) / d;
                if (total > cap) return cap + 1;
            }
            rem -= r;
        }
        return total;
    }
};

} // namespace lc3518
