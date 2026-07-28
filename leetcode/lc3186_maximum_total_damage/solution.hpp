// LC 3186. 施咒的最大总伤害 — 博客 leetcode/dp/ch32.md
#pragma once

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

namespace lc3186 {

// 解法一：双指针，j 指向第一个与 i 冲突的位置
class SolutionTwoPointers {
public:
    long long maximumTotalDamage(std::vector<int>& power) {
        std::map<int, long long> cnt;
        for (int p : power) cnt[p] += p;
        int m = cnt.size();
        std::vector<std::pair<int, long long>> items;
        items.reserve(m);
        for (auto& [k, v] : cnt) items.emplace_back(k, v);
        std::vector<long long> dp(m, 0);
        int j = 0;
        for (int i = 0; i < m; i++) {
            while (j < i && items[j].first + 2 < items[i].first) j++;
            long long take = items[i].second;
            if (j > 0) take += dp[j - 1];
            dp[i] = std::max(i > 0 ? dp[i - 1] : 0LL, take);
        }
        return dp[m - 1];
    }
};

// 解法二：二分查找最后一个不冲突位置
class SolutionBinarySearch {
public:
    long long maximumTotalDamage(std::vector<int>& power) {
        std::map<int, long long> cnt;
        for (int p : power) cnt[p] += p;
        int m = cnt.size();
        std::vector<int> keys;
        std::vector<long long> vals;
        keys.reserve(m);
        vals.reserve(m);
        for (auto& [k, v] : cnt) {
            keys.push_back(k);
            vals.push_back(v);
        }
        std::vector<long long> dp(m, 0);
        dp[0] = vals[0];
        for (int i = 1; i < m; i++) {
            int lo = 0, hi = i - 1, j = -1;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                if (keys[mid] + 2 < keys[i]) {
                    j = mid;
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
            long long take = vals[i] + (j >= 0 ? dp[j] : 0);
            dp[i] = std::max(dp[i - 1], take);
        }
        return dp[m - 1];
    }
};

// 解法三：O(1) 滚动（键严格递增，最多回看 3 个状态）
class SolutionRollingO1 {
public:
    long long maximumTotalDamage(std::vector<int>& power) {
        std::map<int, long long> cnt;
        for (int p : power) cnt[p] += p;
        if (cnt.empty()) return 0;
        long long dp0 = 0, dp1 = 0, dp2 = 0;
        long long keys[3] = {0, 0, 0};
        long long vals[3] = {0, 0, 0};
        int idx = 0;
        int i = 0;
        for (auto& [k, v] : cnt) {
            keys[idx] = k;
            vals[idx] = v;
            long long take = v;
            if (i >= 1 && keys[(idx + 2) % 3] + 2 >= k) {
                if (i >= 2 && keys[(idx + 1) % 3] + 2 >= k) {
                    if (i >= 3) take += dp2;
                } else {
                    if (i >= 2) take += dp1;
                }
            } else {
                if (i >= 1) take += dp0;
            }
            long long cur = std::max(dp0, take);
            dp2 = dp1;
            dp1 = dp0;
            dp0 = cur;
            idx = (idx + 1) % 3;
            i++;
        }
        return dp0;
    }
};

} // namespace lc3186
